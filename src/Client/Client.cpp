/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 18:03:23 by maroy             #+#    #+#             */
/*   Updated: 2024/03/27 16:44:46 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/Client.hpp"

Client::Client(int socket_fd) : _socket_fd(socket_fd), _is_writing(false) {}

Client::~Client(void) {
}

void Client::disconnect(void) {
    close(_socket_fd);
    delete _request;
    delete _response;
}

void Client::readRequest(void) {
    char buffer[BUFFER_SIZE];

    ssize_t bytes_read = recv(_socket_fd, buffer, BUFFER_SIZE, 0);
    if (bytes_read == -1) {
        throw std::runtime_error("Failed to read from socket");
    }
    std::string buffer_str(buffer, bytes_read);
    this->_request = new HTTPRequest();
    this->_request->parse(buffer_str);

    if (buffer_str.find("\r\n\r\n") != std::string::npos) {
        this->_is_writing = true;
        processRequest();
    }
}

void Client::writeResponse(void) {
    if (!_is_writing || this->_response == nullptr) {
        return;
    }
    std::string response_buffer = this->_response->serialize();

    ssize_t bytes_sent = send(_socket_fd, response_buffer.c_str(), response_buffer.size(), 0);
    if (bytes_sent == -1) {
        throw std::runtime_error("Failed to write to socket");
    }
    g_logger.log(DEBUG, "Sent %ld bytes to client %d", bytes_sent, _socket_fd - 5);
    std::cout << response_buffer << std::endl;
    _is_writing = false;
}


bool Client::hasPendingOperations(void) const {
    return this->_is_writing || this->_request == nullptr;
}

int Client::getSocketFd(void) const {
    return _socket_fd;
}

void Client::processRequest(void) {
    this->_response = new HTTPResponse();
    this->_response->setVersion("HTTP/1.1");
    this->_response->setStatusCode("200");
    this->_response->setStatusMessage("OK");
    this->_response->setHeaders({{"Content-Type", "text/plain"}});
    this->_response->setBody(this->_request->getBody());

}