/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AcceptorSockets.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:29:54 by maroy             #+#    #+#             */
/*   Updated: 2024/03/23 19:32:30 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACCEPTORSOCKETS_HPP
#define ACCEPTORSOCKETS_HPP

#include "webserv.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>

class AcceptorSockets {
  public:
    AcceptorSockets(in_addr host, int port, int max_clients);
    ~AcceptorSockets(void);

    void run(void);
    int accept_socket(void);

    // getters
    int getSocketFd(void) const;
    struct sockaddr_in getSocketAddr(void) const;
    socklen_t getAddrLen(void) const;

  private:
    int _socket_fd;
    int _listen_port;
    in_addr _host;
    struct sockaddr_in _addr;
    socklen_t _addr_len;
    size_t _max_clients;
    std::vector<int> _clients_fd;

    void create_socket(void);
    void bind_socket(void);
    void listen_socket(void);
    bool check_max_clients(void);
};

#endif  // ACCEPTORSOCKETS_HPP