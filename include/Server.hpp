/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 12:48:45 by rmarceau          #+#    #+#             */
/*   Updated: 2024/03/23 19:44:02 by maroy            ###   ########.fr       */
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
    std::vector<int> clients_fd_container;

    void setupServerConnections(void);
    void acceptConnections(void);
    void read_socket(int client_fd);
    void write_socket(int client_fd);
};

#endif  // SERVER_HPP