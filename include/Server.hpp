/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 12:48:45 by rmarceau          #+#    #+#             */
/*   Updated: 2024/03/23 16:58:59 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define PORT 8080
#define BACKLOG SOMAXCONN
#define BUFFER_SIZE 1024

#include "defines.h"
#include "parsing.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Server {
public:
    // Constructors and destructors
    Server(void);
    Server(const t_config &config);
    Server(const Server &src);
    ~Server(void);

    // Operators
    Server &operator=(const Server &rhs);

    // Member functions
    void start(void);

    // Base class for Server exceptions
    class ServerException : public std::exception {
    protected:
        std::string _message;
    public:
        ServerException(const std::string &message) : 
            _message(message + ":" + strerror(errno)) {}
        virtual const char *what() const throw() {
            return this->_message.c_str();
        }
    };
private:
    // Attributes
    const t_config _config;

    int _socket;
    int _client_socket;
    struct sockaddr_in _address;
    struct sockaddr_in _client_address;
    socklen_t _client_address_len;   

    // Member functions
    int createSocket(void);
    void bindSocket(int socket);
    void listenConnection(int socket);
    bool acceptConnection(int socket);
    void handleRequest(int client_socket);
    void closeSocket(void);
    
    // Getters
    int getSocket(void) const;
    int getClientSocket(void) const;
    struct sockaddr_in getAddress(void) const;
    struct sockaddr_in getClientAddress(void) const;
    socklen_t getClientAddressLen(void) const;
    
    // Setters
    void setSocket(int socket);
    void setClientSocket(int client_socket);
    void setAddress(struct sockaddr_in address);
    void setClientAddress(struct sockaddr_in client_address);
    void setClientAddressLen(socklen_t client_address_len);
};
// Path: include/Server.hpp