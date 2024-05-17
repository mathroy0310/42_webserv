/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketWrapper.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:29:54 by maroy             #+#    #+#             */
/*   Updated: 2024/05/01 19:01:12 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETWRAPPER_HPP
#define SOCKETWRAPPER_HPP

#include "defines.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>

#include <iostream>
#include <vector>
#include <stdexcept>

class SocketWrapper {
  public:
    // Constructors and destructors
    SocketWrapper(const std::string host, const int port);
    ~SocketWrapper(void);

    // Methods
    void init(void);
    int acceptSocket(void);
    void removeClient(int fd);

    // Getters
    int getSocketFd(void) const;
    int getPort(void) const;
    struct sockaddr_in getSocketAddr(void) const;
    socklen_t getAddrLen(void) const;

  private:
    // Attributes
    int _socket_fd;
    int _listen_port;
    in_addr _host;
    struct sockaddr_in _addr;
    socklen_t _addr_len;
    std::vector<int> _clients_fd;

    // Methods
    void createSocket(void);
    void bindSocket(void);
    void listenSocket(void);
    bool checkMaxClients(void);
};

#endif  // SOCKETWRAPPER_HPP
