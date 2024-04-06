/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 12:48:45 by rmarceau          #+#    #+#             */
/*   Updated: 2024/04/05 19:43:43 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client/Client.hpp"
#include "Socket/Multiplexer.hpp"
#include "Socket/SocketWrapper.hpp"
#include "defines.h"
#include "parsing.hpp"

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
    size_t _server_count;
    bool _running;

    // Methods
    void setupServerConnections(void);
    void acceptConnections(void);
    void handleRequests(void);

    void read_socket(Client &client);
    bool write_socket(Client &client);
};

#endif  // SERVER_HPP
