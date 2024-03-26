/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 18:03:23 by maroy             #+#    #+#             */
/*   Updated: 2024/03/26 16:12:34 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/Client.hpp"

Client::Client(int socket_fd) : _socket_fd(socket_fd), _is_writing(false) {}

Client::~Client(void) {
}

void Client::readRequest(void) {
    char buffer[BUFFER_SIZE];

    ssize_t bytes_read = recv(_socket_fd, buffer, BUFFER_SIZE, 0);
    if (bytes_read == -1) {
        throw std::runtime_error("Failed to read from socket");
    }
    _request_buffer.append(buffer, bytes_read);

    g_logger.log(DEBUG, "Received %ld bytes from client %d", bytes_read, _socket_fd - 5);
    std::cout << _request_buffer << std::endl;

    if (_request_buffer.find("\r\n\r\n") != std::string::npos) {
        handleRequest();
    }
}

void Client::writeResponse(void) {
    if (!_is_writing || _response_buffer.empty()) {
        return;
    }
    
    ssize_t bytes_sent = send(_socket_fd, _response_buffer.c_str(), _response_buffer.size(), 0);
    if (bytes_sent == -1) {
        throw std::runtime_error("Failed to write to socket");
    }
    g_logger.log(DEBUG, "Sent %ld bytes to client %d", bytes_sent, _socket_fd - 5);
    std::cout << _response_buffer << std::endl;
    
    _response_buffer.erase(0, bytes_sent);
    if (_response_buffer.empty()) {
        _is_writing = false;
    }
}

bool Client::hasPendingOperations(void) const {
    return this->_is_writing || !this->_response_buffer.empty();
}

void Client::disconnect(void) {
    close(_socket_fd);
}

int Client::getSocketFd(void) const {
    return _socket_fd;
}

void Client::handleRequest(void) {
    this->_response_buffer = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World!";
    this->_is_writing = true;
}