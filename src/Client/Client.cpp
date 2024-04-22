#include "Client.hpp"

Client::Client(int socket_fd, t_server server) {
    this->_socket_fd = socket_fd;
    this->_server = server;
    this->_status_code = 0;
    this->_request = NULL;
    this->_is_done_reading = true;
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

void Client::disconnect(void) {
    close(this->_socket_fd);
    this->_socket_fd = -1;
}