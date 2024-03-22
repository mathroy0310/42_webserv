/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:27:28 by maroy             #+#    #+#             */
/*   Updated: 2024/03/22 17:40:48 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "defines.h"
# include "parsing.hpp"

# include <exception>

# include <string>

# include <netinet/in.h>

# include <arpa/inet.h>

# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h> 

# include <sys/socket.h>

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
        // int _ipAddress;
};

#endif	// SERVER_HPP

