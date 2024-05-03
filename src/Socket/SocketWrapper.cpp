/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketWrapper.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:29:34 by maroy             #+#    #+#             */
/*   Updated: 2024/03/25 17:31:20 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketWrapper.hpp"

SocketWrapper::SocketWrapper(const std::string host, const int port) {
    this->_host.s_addr = inet_addr(host.c_str());
    this->_listen_port = port;
    this->_addr_len = sizeof(this->_addr);
}
SocketWrapper::~SocketWrapper(void) {}

void SocketWrapper::init(void) {
    this->createSocket();
    this->bindSocket();
    this->listenSocket();
}

void SocketWrapper::createSocket(void) {
    this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket_fd == -1)
        throw std::runtime_error("Socket Creation Failed `socket()'");
}

void SocketWrapper::bindSocket(void) {
    this->_addr.sin_family = AF_INET;
    this->_addr.sin_port = htons(this->_listen_port);
    this->_addr.sin_addr.s_addr = this->_host.s_addr;
    int opt = 1;
    if (setsockopt(this->_socket_fd, SOL_SOCKET, SO_NOSIGPIPE, &opt, sizeof(int)) == -1)
    //if (setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR,&opt, sizeof(int)) == -1)
        throw std::runtime_error("Socket Option Failed `setsockopt()': " + std::string(strerror(errno)));
    if (bind(this->_socket_fd, (struct sockaddr *)&this->_addr, sizeof(_addr)) == -1)
        throw std::runtime_error("Socket Binding Failed `bind()': " + std::string(strerror(errno)));
    int flags = fcntl(this->_socket_fd, F_GETFL, 0);
    if (flags == -1)
        throw std::runtime_error("Socket Flags Failed `fcntl()': " + std::string(strerror(errno)));
    if (fcntl(this->_socket_fd, F_SETFL, flags | O_NONBLOCK) == -1)
        throw std::runtime_error("Socket Non-Blocking Failed `fcntl()': " + std::string(strerror(errno)));
}

void SocketWrapper::listenSocket(void) {
    if (listen(this->_socket_fd, SOMAXCONN) == -1)
        throw std::runtime_error("Socket Listening Failed `listen()'");
}

int SocketWrapper::acceptSocket(void) {
    int new_client_fd = accept(this->_socket_fd, (struct sockaddr *)&this->_addr, &this->_addr_len);
    if (new_client_fd == -1) {
        Logger::get().log(ERROR, "Socket Accept Failed `accept()'");
        return (-1);
    }
    this->_clients_fd.push_back(new_client_fd);
    return (new_client_fd);
}

void SocketWrapper::removeClient(int client_fd) {
    std::vector<int>::iterator it = this->_clients_fd.begin();
    while (it != this->_clients_fd.end()) {
        if (*it == client_fd) {
            this->_clients_fd.erase(it);
            break;
        }
        it++;
    }
}

struct sockaddr_in SocketWrapper::getSocketAddr(void) const {
    return (this->_addr);
}

int SocketWrapper::getSocketFd(void) const {
    return (this->_socket_fd);
}

int SocketWrapper::getPort(void) const {
    return (this->_listen_port);
}
