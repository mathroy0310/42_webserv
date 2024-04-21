/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 23:04:36 by rmarceau          #+#    #+#             */
/*   Updated: 2024/04/21 00:38:27 by maroy            ###   ########.fr       */
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

const std::string getExtension(const std::string &path) {
    std::string extension;
    size_t dot = path.find_last_of('.');
    if (dot != path.npos) {
        extension = path.substr(dot, path.length());
    }
    return (extension);
}

HTTPResponse::HTTPResponse(void) : _version("HTTP/1.1") {}

void HTTPResponse::initStatusCodeMap(void) {
    this->_status_codes[200] = "200 OK";
    this->_status_codes[201] = "201 Created";
    this->_status_codes[202] = "202 Accepted";
    this->_status_codes[204] = "204 No Content";
    this->_status_codes[301] = "301 Moved Permanently";
    this->_status_codes[302] = "302 Found";
    this->_status_codes[303] = "303 See Other";
    this->_status_codes[304] = "304 Not Modified";
    this->_status_codes[400] = "400 Bad Request";
    this->_status_codes[401] = "401 Unauthorized";
    this->_status_codes[402] = "402 Payment Required";
    this->_status_codes[403] = "403 Forbidden";
    this->_status_codes[404] = "404 Not Found";
    this->_status_codes[405] = "405 Method Not Allowed";
    this->_status_codes[413] = "413 Content Too Large";
    this->_status_codes[415] = "415 Unsupported Media Type";
    this->_status_codes[418] = "418 I'm a teapot";
    this->_status_codes[500] = "500 Internal Server Error";
    this->_status_codes[501] = "501 Not Implemented";
    this->_status_codes[502] = "502 Bad Gateway";
    this->_status_codes[503] = "503 Service Unavailable";
    this->_status_codes[504] = "504 Gateway Timeout";
    this->_status_codes[505] = "505 HTTP Version Not Supported";
}

HTTPResponse::HTTPResponse(int status, t_server &server) : _server(server) {
    this->cgi = NULL;
    this->_request = NULL;
    this->_is_header_done = false;
    this->_is_uploaded = true;
    this->_content_length = 0;
    this->_location_index = -1;
    this->_upload_file_size = -1;
    this->_is_default_page_flag = false;
    this->initStatusCodeMap();
    try {
        std::cerr << ERR_PREFIX << "request " << status << FILE_LINE;
    } catch (std::exception &e) {
    }
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
    this->cgi = NULL;
    this->_is_uploaded = true;
    this->_is_default_page_flag = false;
    this->_content_length = 0;
    this->_location_index = -1;
    this->_upload_file_size = -1;
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

HTTPResponse::~HTTPResponse(void) {
    if (this->cgi)
        delete this->cgi;
}

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
        this->_server.server_name.empty() ? "" : this->_s_header += "Server: " + this->_server.server_name + "\r\n";
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
        throw std::runtime_error(ERR_PAGE(this->_status_codes[error_status], this->_server.server_name));
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
        if (error_status == OK_STATUS)
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

    if (directoryRedirection())
        return;
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
    if (!is_autoindex)
        throw std::runtime_error(this->returnError(FORBIDDEN_STATUS));
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

bool HTTPResponse::directoryRedirection() {
    std::string request_path = this->_request->getValueByKey(REQ_PATH);
    if (request_path.length() > 1 && request_path[request_path.length() - 1] != '/') {
        if (!this->_is_header_done) {
            this->_s_header = "HTTP/1.1 ";
            this->_s_header += this->_status_codes[MOVED_PERMANENTLY_STATUS] + "\r\n";
            this->_s_header += "Content-Lenght: 0\r\n";
            this->_s_header += "Location: " + request_path + "/\r\n";
            this->_s_header += "\r\n";
            this->_is_header_done = true;
        }
        this->_s_response = this->_s_header;
        return (true);
    }
    return (false);
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

#define WRITE_BUFFER_SIZE 1000000

std::string HTTPResponse::getBoundary() {
    std::string tmp = "boundary=";
    const std::string &req_con_type = this->_request->getValueByKey(REQ_CONTENT_TYPE);
    size_t index = req_con_type.find(tmp);
    if (index != std::string::npos)
        return ("--" + req_con_type.substr(index + tmp.length(), -1));
    throw std::runtime_error(this->returnError(BAD_REQUEST_STATUS));
}

static std::string getFileName(const std::string &head) {
    size_t filenamePos = head.find("filename=");
    if (filenamePos != std::string::npos) {
        std::string filenameSubstring = head.substr(filenamePos + 10);
        size_t quotePos = filenameSubstring.find(head[filenamePos + 9]);
        if (quotePos != std::string::npos) {
            std::string filename = filenameSubstring.substr(0, quotePos);
            return (filename);
        } else
            throw(BAD_REQUEST_STATUS);
    } else {
        return ("");
    }
}

bool HTTPResponse::uploadFile(std::string &upload_path) {
    std::string &upload_body = this->_request->getBody();
    std::string boundary = getBoundary();

    if (this->_upload_head.empty()) {
        std::size_t index = upload_body.find("\r\n\r\n");
        if (index != std::string::npos) {
            this->_upload_head.reserve(index + 4);
            this->_upload_head = upload_body.substr(0, index + 4);
            upload_body.erase(0, index + 4);
            this->_upload_pos = 0;
        }
        std::string filename = getFileName(this->_upload_head);
        if (this->_upload_head.find("filename") != std::string::npos) {
            this->_upload_of.open(upload_path + (upload_path[upload_path.length() - 1] == '/' ? "" : "/") + filename,
                                  std::ios::out | std::ios::binary);
        }
        this->_upload_file_size = upload_body.find("\r\n" + boundary);
    }
    if (this->_upload_file_size != std::string::npos) {
        size_t pos = static_cast<size_t>(this->_upload_of.tellp());
		Logger::get().log(DEBUG, "upload_file_size: %lu", this->_upload_file_size);
		Logger::get().log(DEBUG, "pos : %lu ", pos);
        if (pos < this->_upload_file_size) {
            size_t write_size = this->_upload_file_size;

            Logger::get().log(INFO, "%lu Bytes Uploaded", write_size);
            this->_upload_of.write(upload_body.c_str() + this->_upload_pos, write_size);
            this->_upload_pos += write_size;
            this->_upload_of.flush();
            return true;
        } else {
            this->_upload_of.close();
            upload_body.erase(0, this->_upload_file_size + ("\r\n" + boundary).length());
            this->_upload_pos = 0;
            this->_upload_head.clear();
        }
    }
    if (upload_body == "--\r\n") {
        return false;
    }
    return false;
}

#include <fcntl.h>
#include <sys/wait.h>

void HTTPResponse::executeCGI(void) {

    if (cgi)
        return;
    Logger::get().log(DEBUG, "executeCGI");
    std::string cgi_exec;

    t_location location = this->getLocation();
    std::string cgi_ext = getExtension(this->_path);
    if (!this->_server.cgi.empty()) {
        cgi_exec = this->_server.cgi[cgi_ext];
    } else {
        cgi_exec = location.cgi[cgi_ext];
    }
    if (cgi_exec.empty() || cgi_ext.empty())
        throw std::runtime_error(this->returnError(NOT_IMPLEMENTED_STATUS));

    int fd[2];
    if (pipe(fd) == -1) {
        std::cerr << FILE_LINE;
        throw std::runtime_error(this->returnError(INTERNAL_SERVER_ERROR_STATUS));
    }

    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << FILE_LINE;
        throw std::runtime_error(this->returnError(INTERNAL_SERVER_ERROR_STATUS));
    }
    this->cgi = new CGIHandler(this->_request, this->_server, this->_path + cgi_exec);
    cgi->setFdIn(fd[0]);
    cgi->setFdOut(fd[1]);
    cgi->setPid(pid);
    if (pid == 1) {
        if (write(fd[1], this->_request->getBody().c_str(), this->_request->getBody().length()) == -1) {
            std::cerr << FILE_LINE;
            throw std::runtime_error(this->returnError(INTERNAL_SERVER_ERROR_STATUS));
        }
    }
    if (pid == 0) {
        dup2(fd[0], STDIN_FILENO);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        char *const argv[] = {const_cast<char *>(cgi_exec.c_str()), const_cast<char *>(this->_path.c_str()), NULL};
        execve(cgi_exec.c_str(), argv, cgi->getEnv());
        perror("execve : ");
        exit(1);
    }

    int status;
    clock_t start_time = clock() / CLOCKS_PER_SEC;
    int res = 0;
    do {
        clock_t current_time = clock() / CLOCKS_PER_SEC;
        if (current_time - start_time > 5)
            break;
        res = waitpid(cgi->getPid(), &status, WNOHANG);
    } while (res == 0);

    if (res == 0) {
        throw std::runtime_error(this->returnError(GATEWAY_TIMEOUT_STATUS));
    }
    if (WIFEXITED(status))
        status = WEXITSTATUS(status);
    if (status) {
        std::cout << "exit with !0 status\n";
        if (access(this->_path.c_str(), F_OK) < 0)
            throw std::runtime_error(this->returnError(NOT_FOUND_STATUS));
        std::cerr << FILE_LINE;
        throw std::runtime_error(this->returnError(INTERNAL_SERVER_ERROR_STATUS));
    }
    usleep(100000);

    char buffer[BUFFER_SIZE + 1];
    bzero(buffer, BUFFER_SIZE + 1);
    fcntl(cgi->getFdIn(), F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    std::cout << "fdIn: " << cgi->getFdIn() << std::endl;
    int b = read(cgi->getFdIn(), buffer, BUFFER_SIZE);
    if (b == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // No data available yet, you may want to retry later
            std::cerr << "No data available yet." << std::endl;
        } else {
            // Some other error occurred
            std::cerr << "Error reading from file descriptor: " << strerror(errno) << std::endl;
            throw std::runtime_error(this->returnError(INTERNAL_SERVER_ERROR_STATUS));
        }
    }
    this->_body.clear();
    this->_body.append(buffer, b);
    Logger::get().log(DEBUG, "CGI response: %s", this->_body.c_str());
    Logger::get().log(DEBUG, "this->_is_header_done = %d", this->_is_header_done);
    if (!this->_is_header_done) {
		this->_s_header += "HTTP/1.1 200 OK\r\n";
        Logger::get().log(DEBUG, "this->_s_header: %s", this->_s_header.c_str());
        size_t find_end_of_head = this->_body.find("\r\n\r\n");
        if (find_end_of_head != this->_body.npos) {
            this->_s_header.insert(0, this->_body.substr(0, find_end_of_head + 4));
            this->_body = this->_body.substr(find_end_of_head + 4, -1);
        }
        Logger::get().log(DEBUG, "this->_body: %s", this->_body.c_str());
        this->_is_header_done = true;
        this->_s_response = this->_s_header + this->_body;
        Logger::get().log(DEBUG, "this->_s_response: %s", this->_s_response.c_str());
    }
}

void HTTPResponse::HandlePostMethod(DIR *dir) {
    Logger::get().log(DEBUG, "HandlePostMethod");
    std::string cgi_ext;
    std::string cgi_exec;
    std::string index;
    std::string upload_path;
    size_t requested_content_lenght = this->_request->getContentLenght();
    size_t max_content_lenght = 0;

    cgi_ext = getExtension(this->_path);
    if (this->_location_index > -1) {
        t_location location = this->getLocation();
        cgi_exec = location.cgi[cgi_ext];
        index = location.index;
        upload_path = location.upload_path;
        max_content_lenght = location.max_body_size;
        if (max_content_lenght == 0)
            max_content_lenght = this->_server.max_body_size;
    } else {
        cgi_exec = this->_server.cgi[cgi_ext];
        index = this->_server.index;
        upload_path = this->_server.upload_path;
        max_content_lenght = this->_server.max_body_size;
    }

    if (requested_content_lenght > max_content_lenght) {
        Logger::get().log(ERROR, "Content too large. Config: %lu | Request: %lu", max_content_lenght,
                          requested_content_lenght);
        throw std::runtime_error(this->returnError(CONTENT_TOO_LARGE_STATUS));
    }
    if (this->_location_index > -1 && this->getLocation().is_cgi == true) {
        this->executeCGI();
    }
    if (!upload_path.empty()) {
		Logger::get().log(INFO, "upload_path: %s", upload_path.c_str());
        DIR *upload_dir = opendir(upload_path.c_str());
        if (!upload_dir)
            throw std::runtime_error(this->returnError(NOT_FOUND_STATUS));
        closedir(upload_dir);
        if (this->uploadFile(upload_path)) {
			
            std::string body = UPLOADED_DEFAULT_PAGE;
            this->_content_length = body.length();
            this->setContentType(".html");
            this->setHeaders(OK_STATUS);
            this->_s_response = this->_s_header + body;
            this->_is_uploaded = true;
            return;
        }
        std::string body = UPLOADED_FAILED_PAGE;
        this->_content_length = body.length();
        this->setContentType(".html");
        this->setHeaders(OK_STATUS);
        this->_s_response = this->_s_header + body;
        this->_is_uploaded = true;
        //this->_is_uploaded = false;
        return;
    } else {
        if (access(this->_path.c_str(), F_OK) < 0) {
            Logger::get().log(ERROR, "File not found: %s", this->_path.c_str());
            throw std::runtime_error(this->returnError(NOT_FOUND_STATUS));
        }
        CGIHandler cgi(this->_request, this->_server, this->_path + index);
        if (dir) {
            if (directoryRedirection())
                return;
            if (!index.empty() && cgi_exec.empty())
                this->executeCGI();
        } else {
            if (!cgi_exec.empty()) {
                this->executeCGI();
            }
        }
    }
    Logger::get().log(ERROR, "Forbidden status");
    throw std::runtime_error(this->returnError(FORBIDDEN_STATUS));
}

std::string HTTPResponse::buildResponse(void) {
    if (this->_is_default_page_flag == true)
        return (this->_s_response);
    DIR *dir = opendir(this->_path.c_str());
    t_location location = this->getLocation();
    try {
        std::string requested_method = this->_request->getValueByKey(REQ_METHOD);
        methodNotAllowed();
        if (!this->_server.redirect_to.empty() ||
            (this->_location_index > -1 && !this->getLocation().redirect_to.empty()))
            this->locationRedirection();
        else if (requested_method == "POST")
            this->HandlePostMethod(dir);
        else if (dir)
            this->listDirectory(dir);
        else if (this->_location_index > -1 && location.is_cgi == true) {
            this->executeCGI();
        } else {
            this->servFile(this->_path, OK_STATUS, NOT_FOUND_STATUS);
        }
    } catch (const std::exception &e) {
    }
    if (dir)
        closedir(dir);
    return (this->_s_response);
}
