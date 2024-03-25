/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 12:47:59 by rmarceau          #+#    #+#             */
/*   Updated: 2024/03/25 17:31:12 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Constructors and destructors

Server::Server(const t_config &config) : _config(config) {}

Server::~Server(void) {}

void Server::runServer(void) {
    g_logger.log(INFO, "Server starting...");
    this->setupServerConnections();
    g_logger.log(INFO, "Server connections setting up...");
    try {
        this->acceptConnections();
    } catch (std::exception &e) {
        g_logger.log(ERROR, "Exception caught: %s", e.what());
        this->close_all_sockets();
        exit(EXIT_FAILURE);
    }
    g_logger.log(INFO, "Server running!");
}

void Server::setupServerConnections(void) {
    for (size_t i = 0; i < this->_config.servers.size(); i++) {
        in_addr addr;
        addr.s_addr = inet_addr(this->_config.servers[i].ip_address.c_str());
        AcceptorSockets acceptor(addr, this->_config.servers[i].port, 128);
        acceptor.run();
        int socket_fd = acceptor.getSocketFd();
        this->_acceptor_sockets.emplace(socket_fd, acceptor);
        g_logger.log(DEBUG, "Server socket set up on address %s and port %d",
                     this->_config.servers[i].ip_address.c_str(), this->_config.servers[i].port);
    }
}

void Server::acceptConnections() {
    std::map<int, AcceptorSockets>::iterator it;
    std::map<int, AcceptorSockets>::iterator ite = this->_acceptor_sockets.end();

    std::map<int, AcceptorSockets *>::iterator it2;
    std::map<int, AcceptorSockets *>::iterator it2e = this->_clients_fd_container.end();

    while (true) {
        std::vector<pollfd> input_event_container;

        // server fd to poll set
        for (it = this->_acceptor_sockets.begin(); it != ite; ++it) {
            pollfd tmp;
            tmp.fd = it->first;
            tmp.events = POLLIN;
            input_event_container.push_back(tmp);
        }
        // client fd to poll set
        for (it2 = this->_clients_fd_container.begin(); it2 != it2e; ++it2) {
            pollfd tmp;
            tmp.fd = it2->first;
            tmp.events = POLLIN;
            input_event_container.push_back(tmp);
        }

        errno = 0;
        int result = poll(input_event_container.data(), input_event_container.size(), -1);
        if (result == -1) {
            if (errno == EINTR)
                continue;
            throw std::runtime_error("poll() failed");
        }

        // Check for new client connections
        for (size_t i = 0; i < this->_acceptor_sockets.size(); i++) {
            if (input_event_container[i].events & POLLIN) {
                std::map<int, AcceptorSockets>::iterator it = this->_acceptor_sockets.begin();
                std::advance(it, i);
                g_logger.log(INFO, "New client connection on server");
                int new_client_fd = it->second.accept_socket();
                if (new_client_fd == -1 || new_client_fd == SERVICE_UNAVAILABLE_STATUS)
                    continue;
                this->_clients_fd_container.insert(std::pair<int, AcceptorSockets *>(new_client_fd, &(it->second)));
            }
        }
        // Handle client connections
        for (size_t j = this->_acceptor_sockets.size(); j < input_event_container.size(); j++) {
            if (input_event_container[j].revents & POLLHUP) {
                g_logger.log(DEBUG, "Client [%d] disconnected", input_event_container[j].fd);
                this->clientDisconnected(input_event_container[j].fd);
                continue;
            }

            if (input_event_container[j].revents & POLLIN) {
                read_socket(input_event_container[j].fd);
            }
        }
    }
}

void Server::clientDisconnected(int client_fd) {
   std::map<int, AcceptorSockets*>::iterator it = this->_clients_fd_container.find(client_fd);
	if (it != this->_clients_fd_container.end()) {
		it->second->remove_client(client_fd);
		this->_clients_fd_container.erase(client_fd);
	}
	close(client_fd);
}

void Server::read_socket(int client_fd) {
    char buffer[1024] = {0};
    int valread = recv(client_fd, buffer, sizeof(buffer), MSG_DONTWAIT);
    if (valread == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;
        throw std::runtime_error("recv() failed");
    } else if (valread == 0) {
        g_logger.log(DEBUG, "Client [%d] disconnected", client_fd);
        this->_clients_fd_container.erase(client_fd);
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

void Server::close_all_sockets(void) {
    for (std::map<int, AcceptorSockets *>::iterator it = this->_clients_fd_container.begin();
         it != this->_clients_fd_container.end(); it++)
        close(it->first);
    this->_clients_fd_container.clear();
    for (std::map<int, AcceptorSockets>::iterator it = this->_acceptor_sockets.begin();
         it != this->_acceptor_sockets.end(); it++) {
        close(it->first);
    }
    this->_acceptor_sockets.clear();
}