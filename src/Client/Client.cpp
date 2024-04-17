#include "Client.hpp"

Client::Client(int socket_fd, t_server server) {
    this->_socket_fd = socket_fd;
    this->_server = server;
    this->_status_code = 0;
    this->_request = NULL;
    this->_response = NULL;
}

Client::~Client(void) {
	if (this->_request) {
		delete this->_request;
	}
	if (this->_response) {
		delete this->_response;
	}
}

int Client::getSocketFd(void) const {
    return (this->_socket_fd);
}

HTTPRequest *Client::createNewRequest(void) {
    return (new HTTPRequest());
}

HTTPRequest *Client::getRequest(void) const {
    return (this->_request);
}

HTTPResponse *Client::getResponse(void) {
    if (!this->_response) {
        if (!this->_status_code)
            this->_response = new HTTPResponse(this->_request, this->_server);
        else
            this->_response = new HTTPResponse(this->_status_code, this->_server);
    }
    return (this->_response);
}

void Client::setRequest(HTTPRequest *request) {
    this->_request = request;
}

void Client::setResponse(HTTPResponse *response) {
    this->_response = response;
}

void Client::setStatus(int status_code) {
    this->_status_code = status_code;
}

/// oLd under that

void Client::disconnect(void) {
    close(this->_socket_fd);
    this->_socket_fd = -1;
}

// void Client::readRequest(void) {
//     if (!this->getRequest()) {
//         Logger::get().log(DEBUG, "Creating new request");
//         this->setRequest(this->createNewRequest());
//     }

//     char buffer[BUFFER_SIZE + 1];

//     bzero(buffer, BUFFER_SIZE + 1);
//     ssize_t bytes_read = recv(this->_socket_fd, buffer, BUFFER_SIZE, 0);
//     if (bytes_read == -1) {
//         throw std::runtime_error("Failed to read from socket");
//     }
//     if (!this->getRequest()->getHeaderEnd()) {
//         try {
//             this->getRequest()->appendHeader(buffer, bytes_read);
//         } catch (int status) {
//             this->setStatus(status);
//         }
//     } else
//         this->getRequest()->appendFile(buffer, bytes_read);

//     if (bytes_read == 0) {
//         this->disconnect();
//     }

//     Logger::get().log(DEBUG, "Received %ld bytes", bytes_read);
//     Logger::get().log(DEBUG, "Request URI: %s", this->_request->getURI().c_str());
//     Logger::get().log(DEBUG, "Request Method: %s", this->_request->getMethod().c_str());
//     Logger::get().log(DEBUG, "Request Body: %s", this->_request->getBody().c_str());
// }

// bool Client::writeResponse(void) {
//     HTTPRequest *request = this->getRequest();
//     HTTPResponse *response = this->getResponse();
//     if (!request || !response) {
//         Logger::get().log(DEBUG, "Request or Response is NULL");
//         return (false);
//     }
//     bool keep_alive = request->getValueByKey(REQ_CONNECTION).empty()
//                           ? true
//                           : (request->getValueByKey(REQ_CONNECTION) == "keep-alive");

//     std::string response_buffer;

//     response_buffer = response->getRequest() ? response->buildResponse() : response->getResponse();
//     Logger::get().log(DEBUG, "Response: %s", response_buffer.c_str());
//     if (response->getUploaded() == true) {
//         ssize_t bytes_sent = send(this->_socket_fd, response_buffer.c_str(), response_buffer.length(), 0);

//         Logger::get().log(DEBUG, "Sent %ld bytes", bytes_sent);
//         if (bytes_sent < BUFFER_SIZE) {
//             delete response;
//             this->setResponse(NULL);
//             request->clear();
//             delete request;
//             this->setRequest(NULL);
//             request = NULL;
//             if (bytes_sent == -1)
//                 return (false);
//             return (true);
//             return (keep_alive);
//         }
//     }
//     return (true);
// //     if (!this->_is_writing) {
// //         return;
// //     }
// //     std::string response_buffer = this->_response->serialize();
// //     ssize_t bytes_sent = send(this->_socket_fd, response_buffer.c_str(), response_buffer.size(), 0);
// //     if (bytes_sent == -1) {
// //         throw std::runtime_error("Failed to write to socket");
// //     }
// //     Logger::get().log(DEBUG, "Sent %ld bytes", bytes_sent);
// //     // std::cout << response_buffer << std::endl;
// //     this->_is_writing = false;
// }

// bool isDirectory(const std::string &path) {
//     struct stat statbuf;
//     if (stat(path.c_str(), &statbuf) != 0) {
//         return false;
//     }
//     return S_ISDIR(statbuf.st_mode);
// }

// std::string Client::getFilePath(void) {

//     static std::string root = this->_server.root;
//     std::string uri = this->_request->getURI();
//     std::vector<t_location>::iterator it = this->_server.locations.begin();
//     // if (uri == "/") {
//     //     if (this->_server.index != "")
//     //         uri = this->_server.index;
//     //     return (root + uri);
//     // }

//     std::cout << "root: " << root << std::endl;
//     for (; it != this->_server.locations.end(); it++) {
//         std::cout << "it->path: " << it->path << std::endl;
//         std::cout << "uri: " << uri << std::endl;

//         if (uri == it->path) {
//             std::cout << "found path!!" << std::endl;
//             std::cout << "it->root: " << it->root << std::endl;
//             std::cout << "it->index: " << it->index << std::endl;
//             root = it->root;
//             uri = it->index;
//             break;
//         }
//     }
//     return (root + "/" + uri);

//     // std::string root = this->_server.root;
//     // std::string uri = this->_request->getURI();

//     // Logger::get().log(DEBUG, "URI: %s", uri.c_str());
//     // Logger::get().log(DEBUG, "Root: %s", root.c_str());
//     // return (root + uri);
// }

// void Client::serveFile(const std::string &file_path, std::map<std::string, std::string> &headers) {
//     if (isDirectory(file_path)) {
//         Logger::get().log(DEBUG, "is a directory");
//         std::string body = "<html><body><ul>";
//         DIR *dir;
//         struct dirent *ent;

//         std::string file_without_root = file_path.substr(this->_server.root.size());

//         if ((dir = opendir(file_path.c_str())) != NULL) {
//             while ((ent = readdir(dir)) != NULL) {
//                 if (ent->d_name[0] == '.')
//                     continue;
//                 body += "<li><a href=\"";
//                 body += file_without_root + "/" + ent->d_name;
//                 body += "\">";
//                 body += ent->d_name;
//                 body += "</a></li>";
//             }
//             closedir(dir);
//         } else {
//             perror("could not open directory");
//         }

//         body += "</ul></body></html>";
//         this->_response->setBody(body);
//         this->_response->setStatusCode(OK_STATUS);
//         this->_response->setStatusMessage("OK");
//         return;
//     }
//     std::ifstream fileStream(file_path, std::ios::binary);
//     if (fileStream.is_open()) {
//         std::string fileContent((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
//         this->_response->setBody(fileContent);
//         this->_response->setStatusCode(OK_STATUS);
//         this->_response->setStatusMessage("OK");
//     } else {
//         headers["Content-Type"] = "text/html";
//         this->_response->setStatusCode(NOT_FOUND_STATUS);
//         this->_response->setStatusMessage("Not Found");
//         if (this->_server.error_pages.find(404) == this->_server.error_pages.end()) {
//             std::cout << "404 not found" << std::endl;
//             this->_response->setBody(ERR_PAGE(NOT_FOUND_STATUS, "Not Found"));
//         } else {
//             std::string error_page = this->_server.root + "/" + this->_server.error_pages[404];
//             std::cout << "error_page: " << error_page << std::endl;
//             fileStream.open(error_page, std::ios::binary);
//             if (fileStream.is_open()) {
//                 std::string fileContent((std::istreambuf_iterator<char>(fileStream)),
//                 std::istreambuf_iterator<char>()); this->_response->setBody(fileContent);
//             } else
//                 this->_response->setBody(ERR_PAGE(NOT_FOUND_STATUS, "Not Found"));
//         }
//     }
// }

// void Client::processRequest(void) {
//     this->_response = new HTTPResponse();

//     std::map<std::string, std::string> headers;
//     headers["Access-Control-Allow-Origin"] = "*";
//     headers["Cache-Control"] = "no-cache";
//     headers["Content-Language"] = "en-US";
//     headers["Connection"] = "close";
//     headers["Server"] = this->_server.server_name;
//     headers["Location"] = this->_request->getURI();
//     headers["Referrer-Policy"] = "no-referrer";

//     if (this->_request->getMethod() == "GET") {
//         std::string file_path = this->getFilePath();
//         headers["Content-Type"] = this->getMimeType(file_path);
//         this->serveFile(file_path, headers);
//     } else if (this->_request->getMethod() == "POST") {
//         this->_response->setStatusCode(NOT_IMPLEMENTED_STATUS);
//         this->_response->setStatusMessage("Not Implemented");
//         this->_response->setBody(ERR_PAGE(NOT_IMPLEMENTED_STATUS, "Not Implemented"));
//     } else if (this->_request->getMethod() == "DELETE") {
//         this->_response->setStatusCode(NOT_IMPLEMENTED_STATUS);
//         this->_response->setStatusMessage("Not Implemented");
//         this->_response->setBody(ERR_PAGE(NOT_IMPLEMENTED_STATUS, "Not Implemented"));
//     } else {
//         this->_response->setStatusCode(METHOD_NOT_ALLOWED_STATUS);
//         this->_response->setStatusMessage("Method Not Allowed");
//         this->_response->setBody(ERR_PAGE(METHOD_NOT_ALLOWED_STATUS, "Method Not Allowed"));
//     }
//     headers["Content-Length"] = std::to_string(this->_response->getBody().size());
//     this->_response->setHeaders(headers);

//     // Debugging
//     const std::map<std::string, std::string> &response_headers = this->_response->getHeaders();
//     for (std::map<std::string, std::string>::const_iterator it = response_headers.begin(); it !=
//     response_headers.end();
//          it++) {
//         Logger::get().log(DEBUG, "Response Header: %s: %s", it->first.c_str(), it->second.c_str());
//     }
// }

// std::string Client::getMimeType(const std::string &file_path) {
//     std::map<std::string, std::string> mimeTypeMap;

//     mimeTypeMap[".txt"] = "text/plain";
//     mimeTypeMap[".html"] = "text/html";
//     mimeTypeMap[".css"] = "text/css";
//     mimeTypeMap[".js"] = "application/javascript";
//     mimeTypeMap[".json"] = "application/json";
//     mimeTypeMap[".jpg"] = "image/jpeg";
//     mimeTypeMap[".jpeg"] = "image/jpeg";
//     mimeTypeMap[".png"] = "image/png";
//     mimeTypeMap[".gif"] = "image/gif";
//     mimeTypeMap[".ico"] = "image/x-icon";
//     mimeTypeMap[".svg"] = "image/svg+xml";
//     mimeTypeMap[".webp"] = "image/webp";
//     mimeTypeMap[".mp3"] = "audio/mpeg";
//     mimeTypeMap[".wav"] = "audio/wav";
//     mimeTypeMap[".mp4"] = "video/mp4";
//     mimeTypeMap[".mpeg"] = "video/mpeg";

//     std::string extension = file_path.substr(file_path.find_last_of('.'));
//     if (mimeTypeMap.find(extension) != mimeTypeMap.end()) {
//         return (mimeTypeMap[extension]);
//     }
//     return ("text/html");
// }
