/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 12:48:45 by rmarceau          #+#    #+#             */
/*   Updated: 2024/03/28 13:10:30 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "parsing.hpp"
#include "defines.h"
#include "Socket/Multiplexer.hpp"
#include "Socket/SocketWrapper.hpp"
#include "Client/Client.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>

class SocketWrapper;
class Client;
class Multiplexer;

class Server {
  public:
    // Constructors and destructors
    Server(const t_config &config);
    ~Server(void);

    // Methods
    void run(void);
    void stop(void);

  private:
    // Attributes
    const t_config _config;
    Multiplexer *_multiplexer;
    std::map<int, SocketWrapper> _listening_sockets;
    std::vector<Client> _clients;
    bool _running;

    // Methods
    void setupServerConnections(void);
    void acceptConnections(void);
    void handleRequests(void);
};

#endif  // SERVER_HPP
