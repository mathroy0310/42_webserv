/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AcceptorSockets.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:29:34 by maroy             #+#    #+#             */
/*   Updated: 2024/03/25 17:31:20 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AcceptorSockets.hpp"

AcceptorSockets::AcceptorSockets(in_addr host, int port, int max_clients) {
    this->_listen_port = port;
    this->_host = host;
    this->_max_clients = max_clients;
    this->_addr_len = sizeof(this->_addr);
}
AcceptorSockets::~AcceptorSockets(void) {}

void AcceptorSockets::run(void) {
    this->create_socket();
    this->bind_socket();
    this->listen_socket();
}

void AcceptorSockets::create_socket(void) {
    _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket_fd == -1) {
        throw std::runtime_error("Socket Creation Failed `socket()'");
    }
}

void AcceptorSockets::bind_socket(void) {
    this->_addr.sin_family = AF_INET;
    this->_addr.sin_port = htons(this->_listen_port);
    this->_addr.sin_addr.s_addr = this->_host.s_addr;
    int opt = 1;
    if (setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1) {
        throw std::runtime_error("Socket Option Failed `setsockopt()'");
    }
    if (bind(this->_socket_fd, (struct sockaddr *)&this->_addr, sizeof(_addr)) == -1) {
        throw std::runtime_error("Socket Binding Failed `bind()': " + std::string(strerror(errno)));
    }
    if (fcntl(this->_socket_fd, F_SETFL, O_NONBLOCK) == -1) {
        throw std::runtime_error("Socket Non-Blocking Failed `fcntl()'");
    }
}

void AcceptorSockets::listen_socket(void) {
    if (listen(this->_socket_fd, this->_max_clients) == -1) {
        throw std::runtime_error("Socket Listening Failed `listen()'");
    }
}

int AcceptorSockets::accept_socket(void) {
    int new_client_fd = accept(this->_socket_fd, (struct sockaddr *)&this->_addr, &this->_addr_len);
    if (new_client_fd == -1) {
		return (-1);
    }
    if (!this->check_max_clients()) {
        close(new_client_fd);
        return (SERVICE_UNAVAILABLE_STATUS);
    }
    this->_clients_fd.push_back(new_client_fd);
    return (new_client_fd);
}

bool AcceptorSockets::check_max_clients(void) {
    if (this->_max_clients <= this->_clients_fd.size()) {
        std::cerr << ERR_PREFIX "Max clients reached\n" << FILE_LINE;
        return (false);
    }
    return (true);
}

void AcceptorSockets::remove_client(int client_fd) {
    std::vector<int>::iterator it = this->_clients_fd.begin();
    while (it != this->_clients_fd.end()) {
        if (*it == client_fd) {
            this->_clients_fd.erase(it);
            break;
        }
        it++;
    }
}

int AcceptorSockets::getSocketFd(void) const {
    return (this->_socket_fd);
}

struct sockaddr_in AcceptorSockets::getSocketAddr(void) const {
    return (this->_addr);
}

socklen_t AcceptorSockets::getAddrLen(void) const {
    return (this->_addr_len);
}