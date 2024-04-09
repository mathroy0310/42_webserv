/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 23:04:36 by rmarceau          #+#    #+#             */
/*   Updated: 2024/04/08 21:46:08 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

static void correctPath(std::string &path) {
    size_t tmp = path.find_last_not_of('/');
    if (tmp != path.length() - 1) {
        path.erase(tmp + 2, -1);
    }
}

static bool isMethodAllowed(const std::string &method, const std::vector<std::string> &allowed_methods) {
    return (std::find(allowed_methods.begin(), allowed_methods.end(), method) != allowed_methods.end());
}

static bool findAllowedMethod(const std::string &method, const t_server &server, const t_location *location) {
    if (location == NULL)
        return (isMethodAllowed(method, server.allowed_methods));
    else {
        bool in_location = isMethodAllowed(method, location->allowed_methods);
        bool in_server = isMethodAllowed(method, server.allowed_methods);
        return (in_location || (location->allowed_methods.empty() && in_server));
    }
}

HTTPResponse::HTTPResponse(void) : _version("HTTP/1.1") {}

void HTTPResponse::initStatusCodeMap(void) {
    this->_status_codes[200] = "200 OK";
    this->_status_codes[201] = "201 Created";
    this->_status_codes[204] = "204 No Content";
    this->_status_codes[301] = "301 Moved Permanently";
    this->_status_codes[304] = "304 Not Modified";
    this->_status_codes[400] = "400 Bad Request";
    this->_status_codes[403] = "403 Forbidden";
    this->_status_codes[404] = "404 Not Found";
    this->_status_codes[405] = "405 Method Not Allowed";
    this->_status_codes[409] = "409 Conflict";
    this->_status_codes[413] = "413 Content Too Large";
    this->_status_codes[415] = "415 Unsupported Media Type";
    this->_status_codes[418] = "418 I'm a teapot";
    this->_status_codes[500] = "500 Internal Server Error";
    this->_status_codes[501] = "501 Not Implemented";
    this->_status_codes[504] = "504 Gateway Timeout";
    this->_status_codes[505] = "505 HTTP Version Not Supported";
}

HTTPResponse::HTTPResponse(int status, t_server &server) : _server(server) {
    this->_request = NULL;
    this->_is_header_done = false;
    this->_is_uploaded = true;
    this->_content_length = 0;
    this->_location_index = -1;
    this->_is_default_page_flag = false;
    this->initStatusCodeMap();
    try {
        std::cerr << ERR_PREFIX << "request " << status << FILE_LINE;
    } catch (std::exception &e) {
    }
}

const std::string getExtension(const std::string &path) {
    std::string extension;
    size_t dot = path.find_last_of('.');
    if (dot != path.npos) {
        extension = path.substr(dot, path.length());
    }
    return (extension);
}

void HTTPResponse::defaultPage(void) {
    this->setContentType(".html");
    this->_body = DEFAULT_PAGE;
    this->_content_length = this->_body.length();
    this->setHeaders(OK_STATUS);
    this->_s_response = this->_s_header + this->_body;
    this->_s_header.clear();
    this->_is_default_page_flag = true;
}

static void correctPath(std::string &path);

HTTPResponse::HTTPResponse(HTTPRequest *request, t_server &server)
    : _request(request), _is_header_done(false), _server(server) {
    this->_is_uploaded = true;
    this->_is_default_page_flag = false;
    this->_content_length = 0;
    this->_location_index = -1;
    if (this->_request)
        this->setContentType(getExtension(this->_request->getValueByKey(REQ_PATH)));
    this->initStatusCodeMap();
    this->_path = this->matching();
    correctPath(this->_path);
}

void HTTPResponse::locationExists() {
    std::vector<t_location> locations = this->_server.locations;
    std::vector<std::string> location_paths;

    for (size_t i = 0; i < locations.size(); i++)
        location_paths.push_back(locations[i].path);
    std::string look_for = this->_request->getValueByKey(REQ_PATH);
    while (look_for.size() > 0) {
        Logger::get().log(DEBUG, "look_for: %s", look_for.c_str());
        std::vector<std::string>::iterator it = std::find(location_paths.begin(), location_paths.end(), look_for);
        if (it != location_paths.end()) {
            this->_location_index = std::distance(location_paths.begin(), it);
            return;
        }
        size_t last = look_for.find_last_of('/');
        if (last == look_for.npos)
            break;
        look_for.erase(last, -1);
    }
    if (this->_request->getValueByKey(REQ_PATH)[0] == '/' && look_for.empty()) {
        std::vector<std::string>::iterator it = std::find(location_paths.begin(), location_paths.end(), "/");
        if (it != location_paths.end()) {
            this->_location_index = std::distance(location_paths.begin(), it);
            return;
        }
    }
}

std::string HTTPResponse::matching() {
    std::string req_path = this->_request->getValueByKey(REQ_PATH);
    std::string root = this->_server.root;

    this->locationExists();
    Logger::get().log(DEBUG, "location index: %d", this->_location_index);
    if (this->_location_index == -1) {
        if (root.empty() && req_path.find_first_not_of('/') == req_path.npos)
            this->defaultPage();
        Logger::get().log(DEBUG, "root: %s", root.c_str());
        Logger::get().log(DEBUG, "req_path: %s", req_path.c_str());
        return (root + req_path);
    }
    t_location location = this->getLocation();
    if (!location.root.empty())
        root = location.root;
    Logger::get().log(DEBUG, "root: %s", root.c_str());
    Logger::get().log(DEBUG, "req_path: %s", req_path.c_str());
    req_path = req_path.substr(location.path.length(), req_path.length());

    Logger::get().log(DEBUG, "root + req_path: %s", (root + req_path).c_str());
    return (root + req_path);
}

void HTTPResponse::setContentType(const std::string &extension) {
    static std::map<std::string, std::string> content_type_map;
    static bool done = false;

    if (!done) {
        content_type_map[".aac"] = "audio/aac";
        content_type_map[".abw"] = "application/x-abiword";
        content_type_map[".arc"] = "application/x-freearc";
        content_type_map[".avif"] = "image/avif";
        content_type_map[".avi"] = "video/x-msvideo";
        content_type_map[".azw"] = "application/vnd.amazon.ebook";
        content_type_map[".bin"] = "application/octet-stream";
        content_type_map[".bmp"] = "image/bmp";
        content_type_map[".bz"] = "application/x-bzip";
        content_type_map[".bz2"] = "application/x-bzip2";
        content_type_map[".cda"] = "application/x-cdf";
        content_type_map[".csh"] = "application/x-csh";
        content_type_map[".css"] = "text/css";
        content_type_map[".csv"] = "text/csv";
        content_type_map[".doc"] = "application/msword";
        content_type_map[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
        content_type_map[".eot"] = "application/vnd.ms-fontobject";
        content_type_map[".epub"] = "application/epub+zip";
        content_type_map[".gz"] = "application/gzip";
        content_type_map[".gif"] = "image/gif";
        content_type_map[".htm"] = "text/html";
        content_type_map[".html"] = "text/html";
        content_type_map[".ico"] = "image/vnd.microsoft.icon";
        content_type_map[".ics"] = "text/calendar";
        content_type_map[".jar"] = "application/java-archive";
        content_type_map[".jpeg"] = "image/jpeg";
        content_type_map[".jpg"] = "image/jpeg";
        content_type_map[".js"] = "text/javascript";
        content_type_map[".json"] = "application/json";
        content_type_map[".jsonld"] = "application/ld+json";
        content_type_map[".midi"] = "audio/midi";
        content_type_map[".mid"] = "audio/x-midi";
        content_type_map[".mjs"] = "text/javascript";
        content_type_map[".mp3"] = "audio/mpeg";
        content_type_map[".mp4"] = "video/mp4";
        content_type_map[".mpeg"] = "video/mpeg";
        content_type_map[".mpkg"] = "application/vnd.apple.installer+xml";
        content_type_map[".odp"] = "application/vnd.oasis.opendocument.presentation";
        content_type_map[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
        content_type_map[".odt"] = "application/vnd.oasis.opendocument.text";
        content_type_map[".oga"] = "audio/ogg";
        content_type_map[".ogv"] = "video/ogg";
        content_type_map[".ogx"] = "application/ogg";
        content_type_map[".opus"] = "audio/opus";
        content_type_map[".otf"] = "font/otf";
        content_type_map[".png"] = "image/png";
        content_type_map[".pdf"] = "application/pdf";
        content_type_map[".php"] = "application/x-httpd-php";
        content_type_map[".ppt"] = "application/vnd.ms-powerpoint";
        content_type_map[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
        content_type_map[".rar"] = "application/vnd.rar";
        content_type_map[".rtf"] = "application/rtf";
        content_type_map[".sh"] = "application/x-sh";
        content_type_map[".svg"] = "image/svg+xml";
        content_type_map[".tar"] = "application/x-tar";
        content_type_map[".tif"] = "image/tiff";
        content_type_map[".tiff"] = "image/tiff";
        content_type_map[".ts"] = "video/mp2t";
        content_type_map[".ttf"] = "font/ttf";
        content_type_map[".txt"] = "text/plain";
        content_type_map[".vsd"] = "application/vnd.visio";
        content_type_map[".wav"] = "audio/wav";
        content_type_map[".weba"] = "audio/webm";
        content_type_map[".webm"] = "video/webm";
        content_type_map[".webp"] = "image/webp";
        content_type_map[".woff"] = "font/woff";
        content_type_map[".woff2"] = "font/woff2";
        content_type_map[".xhtml"] = "application/xhtml+xml";
        content_type_map[".xls"] = "application/vnd.ms-excel";
        content_type_map[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
        content_type_map[".xml"] = "application/xml";
        content_type_map[".xul"] = "application/vnd.mozilla.xul+xml";
        content_type_map[".zip"] = "application/zip";
        content_type_map[".3gp"] = "video/3gpp";
        content_type_map[".3g2"] = "video/3gpp2";
        content_type_map[".7z"] = "application/x-7z-compressed";
        content_type_map[".ico"] = "image/x-icon";
        done = true;
    }
    std::string tmp = content_type_map[extension];
    if (tmp.empty()) {
        tmp = "text/plain";
    }
    this->mime_type = tmp;
}
const std::string &HTTPResponse::getResponse(void) const {
    return (this->_s_response);
}

HTTPRequest *HTTPResponse::getRequest(void) const {
    return (this->_request);
}

bool HTTPResponse::getUploaded(void) const {
    return (this->_is_uploaded);
}

HTTPResponse::~HTTPResponse(void) {}

void HTTPResponse::setVersion(const std::string &version) {
    this->_version = version;
}

void HTTPResponse::setStatusCode(int status_code) {
    this->_status_code = status_code;
}

void HTTPResponse::setStatusMessage(const std::string &status_status_message) {
    this->_status_message = status_status_message;
}

void HTTPResponse::setHeaders(int status) {
    if (!this->_is_header_done) {
        this->_s_header += "HTTP/1.1 ";
        this->_s_header += this->_status_codes[status] + "\r\n";
        this->_s_header += "Connection: keep-alive\r\n";
        this->_s_header += "Accept-Ranges: bytes\r\n";
        this->_s_header += "Content-Type: " + this->mime_type + "\r\n";
        this->_s_header += "Content-Length: " + std::to_string(this->_content_length) + "\r\n";
        this->_s_header += "\r\n";
        this->_is_header_done = true;
    }
}

void HTTPResponse::setBody(const std::string &body) {
    this->_body = body;
}

std::string HTTPResponse::getVersion(void) const {
    return (this->_version);
}

std::string HTTPResponse::getStatusCode(void) const {
    return (this->_status_code);
}

std::string HTTPResponse::getStatusMessage(void) const {
    return (this->_status_message);
}

std::map<std::string, std::string> HTTPResponse::getHeaders(void) const {
    return (this->_headers);
}

std::string HTTPResponse::getBody(void) const {
    return (this->_body);
}

const t_location &HTTPResponse::getLocation(void) const {
    if (this->_location_index < 0)
        throw std::out_of_range("getLocation()");
    return (this->_server.locations[this->_location_index]);
}

const t_server &HTTPResponse::getServer(void) const {
    return (this->_server);
}

std::string HTTPResponse::fileToString(const std::string &file_path, int error_status) {
	std::ifstream current_file;
    if (!current_file.is_open()) {
        current_file.open(file_path.c_str());
        struct stat fs;
        Logger::get().log(DEBUG, "file_path: %s", file_path.c_str());
        if (stat(file_path.c_str(), &fs) == 0)
            this->_content_length = fs.st_size;
	}
    if (!current_file.is_open()) {
        throw std::runtime_error(ERR_PAGE(this->_status_codes[error_status]));
    }

    char buffer[BUFFER_SIZE + 1];
    bzero(buffer, BUFFER_SIZE + 1);
    current_file.read(buffer, BUFFER_SIZE);
    std::string result(buffer, current_file.gcount());
	while (current_file.gcount() > 0) {
		bzero(buffer, BUFFER_SIZE + 1);
		current_file.read(buffer, BUFFER_SIZE);
		result += std::string(buffer, current_file.gcount());
	}
    return (result);
}

const std::string &HTTPResponse::returnError(int status) {
    if (this->_location_index > -1) {
        t_location location = this->getLocation();
        try {
            this->_body = fileToString(location.error_pages[status], status);
        } catch (const std::exception &e) {
            try {
                this->_body = fileToString(this->_server.error_pages[status], status);
            } catch (const std::exception &ex) {
                this->_body = ex.what();
                this->_content_length = this->_body.length();
            }
        }
    } else {
        try {
            this->_body = fileToString(this->_server.error_pages[status], status);
        } catch (const std::exception &ex) {
            this->_body = ex.what();
            this->_content_length = this->_body.length();
        }
    }
    this->setContentType(".html");
    this->setHeaders(status);
    this->_s_response = this->_s_header + this->_body;
    this->_s_header.clear();
    return (this->_body);
}
void HTTPResponse::servFile(const std::string &file_path, int status, int error_status) {
    try {
        this->_body = fileToString(file_path, error_status);
        this->setHeaders(status);
        this->_s_response = this->_s_header + this->_body;
        this->_s_header.clear();
    } catch (std::exception &e) {
        if (error_status == 200)
            throw;
        throw std::runtime_error(this->returnError(NOT_FOUND_STATUS));
    }
}

void HTTPResponse::listDirectory(DIR *dir) {
    std::string index;
    bool is_autoindex;

    if (this->_location_index > -1) {
        index = this->getLocation().index;
        is_autoindex = this->getLocation().is_autoindex;
    } else {
        index = this->getServer().index;
        is_autoindex = this->getServer().is_autoindex;
    }


	std::string indexFile = this->_path + (this->_path[this->_path.length() - 1] != '/' ? "/" : "") + "index.html";
	Logger::get().log(DEBUG, "indexFile: %s", indexFile.c_str());
	Logger::get().log(DEBUG, "index: %s", index.c_str());
	if (index.empty()) {
		try {
			this->setContentType(".html");
			this->servFile(indexFile, OK_STATUS, 0);
			return;
		} catch (const std::exception &e) {
			this->_is_header_done = false;
		}
	} else if (!index.empty()) {
		if (index[0] == '/')
			throw std::runtime_error(this->returnError(NOT_FOUND_STATUS));
		indexFile = this->_path + index;
		this->setContentType(getExtension(indexFile));
		Logger::get().log(DEBUG, "indexFile: %s", indexFile.c_str());
		return (servFile(indexFile, OK_STATUS, NOT_FOUND_STATUS));
	}
	Logger::get().log(DEBUG, "directory_listing");
	this->_body = directory_listing(dir, this->_request->getValueByKey(REQ_PATH));
	this->_content_length = this->_body.length();
	this->setContentType(".html");
	this->setHeaders(OK_STATUS);
	this->_s_response = this->_s_header + this->_body;
	this->_s_header.clear();
}

void HTTPResponse::methodNotAllowed(void) {
    t_location *location = NULL;
    const std::string &method = this->_request->getValueByKey(REQ_METHOD);

    if (this->_location_index > -1)
        location = &this->_server.locations[this->_location_index];
    if (method != "GET" && method != "POST" && method != "DELETE")
        throw std::runtime_error(this->returnError(NOT_IMPLEMENTED_STATUS));
    else if (findAllowedMethod(method, this->_server, location) == false)
        throw std::runtime_error(this->returnError(METHOD_NOT_ALLOWED_STATUS));
}

void HTTPResponse::locationRedirection() {
    t_server server = this->_server;
    std::string redir_to;
    int redir_code;

    if (!this->_server.redirect_to.empty()) {
        redir_to = this->_server.redirect_to;
        redir_code = this->_server.redirect_code;
    } else {
        t_location location = this->getLocation();
        redir_to = location.redirect_to;
        redir_code = location.redirect_code;
    }
    if (!this->_is_header_done) {
        this->_s_header += "HTTP/1.1 ";
        this->_s_header += this->_status_codes[redir_code] + "\r\n";
        this->_s_header += "Content-Lenght: 0\r\n";
        this->_s_header += "Location: " + redir_to;
        this->_s_header += "\r\n";
        this->_is_header_done = true;
    }
    this->_s_response = this->_s_header;
    this->_s_header.clear();
}

std::string HTTPResponse::buildResponse(void) {
    if (this->_is_default_page_flag == true)
        return (this->_s_response);
    DIR *dir = opendir(this->_path.c_str());
    try {
        std::string requested_method = this->_request->getValueByKey(REQ_METHOD);
        methodNotAllowed();
        if (!this->_server.redirect_to.empty() ||
            (this->_location_index > -1 && !this->getLocation().redirect_to.empty())) {
            this->locationRedirection();
        } else if (dir) {
            Logger::get().log(DEBUG, "listDirectory");
            this->listDirectory(dir);
        } else {
            this->servFile(this->_path, OK_STATUS, NOT_FOUND_STATUS);
        }
    } catch (const std::exception &e) {
    }
    if (dir)
        closedir(dir);
    return (this->_s_response);
}
