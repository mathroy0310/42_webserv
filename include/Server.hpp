/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 12:48:45 by rmarceau          #+#    #+#             */
/*   Updated: 2024/03/25 17:22:52 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "webserv.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>

class AcceptorSockets;

class Server {
  public:
    // Constructors and destructors
    Server(const t_config &config);
    ~Server(void);
    void runServer(void);

  private:
    // Attributes
    const t_config _config;
    std::map<int, AcceptorSockets> _acceptor_sockets;
    std::map<int, AcceptorSockets *> _clients_fd_container;

    void setupServerConnections(void);
    void acceptConnections(void);
    void clientDisconnected(int client_fd);
    void read_socket(int client_fd);
    void write_socket(int client_fd);
    void close_all_sockets(void);
};

#endif  // SERVER_HPP