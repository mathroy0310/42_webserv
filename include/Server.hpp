/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:27:28 by maroy             #+#    #+#             */
/*   Updated: 2024/03/23 01:33:56 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#define PORT "8081"
#define BACKLOG 2

# include "defines.h"
# include "parsing.hpp"

# include <exception>

# include <string>

# include <arpa/inet.h>

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

class Server
{
	public:

		Server(t_config config);
		~Server(void);

        // Member functions
        int convertAddressToIP(const std::string &address);
        int prepareSocket(void);


        // Exceptions
        class ErrorException : public std::exception {
            virtual const char *what() const throw() {
                return "ERROR!!!";
            }
        };
	private:

		Server(void);
		Server(Server const &src);
		Server	&operator=(Server const &rhs);

        // Attributes
		const t_config _config;
        addrinfo *_addressInfo;

};

#endif	// SERVER_HPP

