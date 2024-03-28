#include "Client.hpp"

Client::Client(int socket_fd, t_server server) : _socket_fd(socket_fd), _server(server) ,_is_writing(false) {}

Client::~Client(void) {}

void Client::disconnect(void) {
    close(this->_socket_fd);
	this->_socket_fd = -1;
}

void Client::readRequest(void) {
    char buffer[BUFFER_SIZE];

    ssize_t bytes_read = recv(this->_socket_fd, buffer, BUFFER_SIZE, 0);
    if (bytes_read == -1) {
        throw std::runtime_error("Failed to read from socket");
    }
	if (bytes_read == 0) {
		this->disconnect();
	}
    std::string buffer_str(buffer, bytes_read);
    this->_request = new HTTPRequest();
    this->_request->parse(buffer_str);

	//g_logger.log(DEBUG, "request : \n%s", buffer_str.c_str());
	Logger::get().log(DEBUG, "Received %ld bytes from client [%d]", bytes_read, this->_socket_fd - 4);
	Logger::get().log(DEBUG, "Request URI: %s", this->_request->getURI().c_str());
	Logger::get().log(DEBUG, "Request Method: %s", this->_request->getMethod().c_str());
	Logger::get().log(DEBUG, "Request Version: %s", this->_request->getVersion().c_str());
	//g_logger.log(DEBUG, "Request Body: %s", this->_request->getBody().c_str());

    if (buffer_str.find("\r\n\r\n") != std::string::npos) {
        this->_is_writing = true;
        processRequest();
    }
}

void Client::writeResponse(void) {
    if (!this->_is_writing) {
        return;
    }
	std::string response_buffer = this->_response->serialize();
	//std::string response_buffer = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello World!";
    ssize_t bytes_sent = send(this->_socket_fd, response_buffer.c_str(), response_buffer.size(), 0);
    if (bytes_sent == -1) {
        throw std::runtime_error("Failed to write to socket");
    }
    Logger::get().log(DEBUG, "Sent %ld bytes to client [%d]", bytes_sent, this->_socket_fd - 4);
    this->_is_writing = false;
}

bool Client::hasPendingOperations(void) const {
    return (this->_is_writing);
}

int Client::getSocketFd(void) const {
    return (this->_socket_fd);
}

bool isDirectory(const std::string &path) {
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0) {
        return false;
    }
    return S_ISDIR(statbuf.st_mode);
}

std::string Client::getFilePath(void) {
    std::string root = this->_server.root;
    std::string uri = this->_request->getURI();
    
    if (uri == "/") {
        uri = this->_server.index;
    }
    Logger::get().log(DEBUG, "URI: %s", uri.c_str());
    Logger::get().log(DEBUG, "Root: %s", root.c_str());
    return (root + uri);
}

void Client::serveFile(const std::string &file_path) {
	if (isDirectory(file_path))
	{
		Logger::get().log(DEBUG, "is a directory");
		std::string body = "<html><body><ul>";
		DIR *dir;
		struct dirent *ent;

        std::string file_without_root = file_path.substr(this->_server.root.size());

		if ((dir = opendir(file_path.c_str())) != NULL) {
			while ((ent = readdir(dir)) != NULL) {
				if (ent->d_name[0] == '.')
					continue;
				body += "<li><a href=\"";
				body += file_without_root + ent->d_name;
				body += "\">";
				body += ent->d_name;
				body += "</a></li>";
			}
			closedir(dir);
		} else {
			perror("could not open directory");
		}

		body += "</ul></body></html>";
		this->_response->setBody(body);
        this->_response->setStatusCode(OK_STATUS);
        this->_response->setStatusMessage("OK");
		return;
	}
    std::ifstream fileStream(file_path, std::ios::binary);
    if (fileStream.is_open()) {
        std::string fileContent((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
        this->_response->setBody(fileContent);
        this->_response->setStatusCode(OK_STATUS);
        this->_response->setStatusMessage("OK");
    } else {
        this->_response->setStatusCode(NOT_FOUND_STATUS);
        this->_response->setStatusMessage("Not Found");
        this->_response->setBody(ERR_PAGE(NOT_FOUND_STATUS, "Not Found"));
    }
}

void Client::processRequest(void) {
    this->_response = new HTTPResponse();

    std::map<std::string, std::string> headers;
    headers["Server"] = "webserv/1.0";
    headers["Encoding"] = "gzip, deflate, br";
    headers["Content-Type"] = "text/html";
    headers["Connection"] = "keep-alive";
    headers["Content-Length"] = "0";

    this->_response->setHeaders(this->_request->getHeaders());

    if (this->_request->getMethod() == "GET") {
        std::string file_path = this->getFilePath();
        this->serveFile(file_path);
    } else {
        this->_response->setStatusCode(METHOD_NOT_ALLOWED_STATUS);
        this->_response->setStatusMessage("Method Not Allowed");
        this->_response->setBody(ERR_PAGE(METHOD_NOT_ALLOWED_STATUS, "Method Not Allowed"));
    }
}
