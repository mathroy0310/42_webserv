/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 18:03:23 by maroy             #+#    #+#             */
/*   Updated: 2024/03/28 12:29:56 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/Client.hpp"

Client::Client(int socket_fd) : _socket_fd(socket_fd), _is_writing(false) {}

Client::~Client(void) {}

void Client::disconnect(void) {
    close(this->_socket_fd);
    delete this->_request;
    delete this->_response;
}

void Client::readRequest(void) {
    char buffer[BUFFER_SIZE];

    ssize_t bytes_read = recv(this->_socket_fd, buffer, BUFFER_SIZE, 0);
    if (bytes_read == -1) {
        throw std::runtime_error("Failed to read from socket");
    }
    std::string buffer_str(buffer, bytes_read);
    this->_request = new HTTPRequest();
    this->_request->parse(buffer_str);

	//g_logger.log(DEBUG, "request : \n%s", buffer_str.c_str());
	g_logger.log(DEBUG, "Received %ld bytes from client [%d]", bytes_read, this->_socket_fd - 5);
	g_logger.log(DEBUG, "Request URI: %s", this->_request->getURI().c_str());
	g_logger.log(DEBUG, "Request Method: %s", this->_request->getMethod().c_str());
	g_logger.log(DEBUG, "Request Version: %s", this->_request->getVersion().c_str());
	//g_logger.log(DEBUG, "Request Body: %s", this->_request->getBody().c_str());

    if (buffer_str.find("\r\n\r\n") != std::string::npos) {
        this->_is_writing = true;
        processRequest();
    }
}

void Client::writeResponse(void) {
    if (!this->_is_writing || this->_response == nullptr) {
        return;
    }
	std::string response_buffer = this->_response->serialize();
	//std::string response_buffer = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello World!";
    ssize_t bytes_sent = send(this->_socket_fd, response_buffer.c_str(), response_buffer.size(), 0);
    if (bytes_sent == -1) {
        throw std::runtime_error("Failed to write to socket");
    }
    g_logger.log(DEBUG, "Sent %ld bytes to client [%d]", bytes_sent, this->_socket_fd - 5);
    this->_is_writing = false;
}

bool Client::hasPendingOperations(void) const {
    return (this->_is_writing || this->_request == nullptr);
}

int Client::getSocketFd(void) const {
    return (this->_socket_fd);
}

void Client::processRequest(void) {
    this->_response = new HTTPResponse();
    this->_response->setVersion("HTTP/1.1");
    this->_response->setStatusCode("200");
    this->_response->setStatusMessage("OK");
    this->_response->setHeaders({{"Content-Type", "text/html"}});
    this->_response->setBody(this->_request->getBody());
}
