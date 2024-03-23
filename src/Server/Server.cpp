/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 12:47:59 by rmarceau          #+#    #+#             */
/*   Updated: 2024/03/23 19:47:44 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Constructors and destructors

Server::Server(const t_config &config) : _config(config) {}

Server::~Server(void) {}

void Server::runServer(void) {
    g_logger.log(INFO, "Server starting...");
    this->setupServerConnections();
    g_logger.log(INFO, "Server connections set up");
    this->acceptConnections();
}

void Server::setupServerConnections(void) {
    for (size_t i = 0; i < this->_config.servers.size(); i++) {
        in_addr addr;
        addr.s_addr = inet_addr(this->_config.servers[i].ip_address.c_str());
        AcceptorSockets acceptor(addr, this->_config.servers[i].port, 128);
        acceptor.run();
        int socket_fd = acceptor.getSocketFd();
        this->_acceptor_sockets.insert(std::pair<int, AcceptorSockets>(socket_fd, acceptor));
        g_logger.log(DEBUG, "Server socket set up on address %s and port %d",
                     this->_config.servers[i].ip_address.c_str(), this->_config.servers[i].port);
    }
}

void Server::acceptConnections() {
    std::map<int, AcceptorSockets>::iterator it = this->_acceptor_sockets.begin();
    std::map<int, AcceptorSockets>::iterator ite = this->_acceptor_sockets.end();

    while (true) {
        std::vector<pollfd> input_event_container;

        size_t i = 0;
        // client fd to poll set
        for (; i < clients_fd_container.size(); i++) {
            pollfd tmp;
            tmp.fd = clients_fd_container[i];
            tmp.events = POLLIN;
            input_event_container.push_back(tmp);
        }

        // server fd to poll set
        for (it = this->_acceptor_sockets.begin(); it != ite; ++it) {
            pollfd tmp;
            tmp.fd = it->first;
            tmp.events = POLLIN;
            input_event_container.push_back(tmp);
            ++i;
        }

        errno = 0;
        int result = poll(input_event_container.data(), input_event_container.size(), -1);
        if (result == -1) {
            if (errno == EINTR)
                continue;
            g_logger.log(ERROR, "poll() failed");
            exit(EXIT_FAILURE);
        }

        // Check for new client connections
        for (it = this->_acceptor_sockets.begin(); it != ite; ++it) {
            int serverFD = it->first;
            AcceptorSockets &acceptor = it->second;

            for (size_t k = 0; k < input_event_container.size(); k++) {
                if (input_event_container[k].fd == serverFD && input_event_container[k].revents & POLLIN) {
                    int new_client = acceptor.accept_socket();
                    if (new_client == -1 || new_client == SERVICE_UNAVAILABLE_STATUS)
                        continue;
                    clients_fd_container.push_back(new_client);
                }
            }
        }
        // Handle client connections
        for (size_t j = 0; j < input_event_container.size() - _acceptor_sockets.size(); j++) {
            if (input_event_container[j].revents & POLLIN) {
                read_socket(input_event_container[j].fd);
            }
        }
    }
}

void Server::read_socket(int client_fd) {
    char buffer[1024] = {0};
    int valread = read(client_fd, buffer, sizeof(buffer));
    if (valread == -1) {
        g_logger.log(ERROR, "read() failed");
        exit(EXIT_FAILURE);
    }
    if (valread == 0) {
        for (std::vector<int>::iterator it = this->clients_fd_container.begin(); it != this->clients_fd_container.end();
             it++) {
            if (*it == client_fd) {
                this->clients_fd_container.erase(it);
                break;
            }
        }
        close(client_fd);
    } else {
        g_logger.log(DEBUG, "Received message from client %d: %s", client_fd - 5, buffer);
        write_socket(client_fd);
    }
}

void Server::write_socket(int client_fd) {
    std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello, World!";
    send(client_fd, response.c_str(), response.length(), 0);
}