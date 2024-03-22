/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:27:28 by maroy             #+#    #+#             */
/*   Updated: 2024/03/22 16:27:50 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "defines.h"
# include "parsing.hpp"

# include <exception>

# include <string>

# include <netinet/in.h>
# include <netdb.h> 

struct addrinfo {
    int              ai_flags;
    int              ai_family;
    int              ai_socktype;
    int              ai_protocol;
    size_t           ai_addrlen;
    struct sockaddr *ai_addr;
    char            *ai_canonname;
    struct addrinfo *ai_next;
};

class Server
{
	public:

		Server(t_config config);
		~Server(void);

        // Member functions
        int convertIPToInt(const std::string &IPAddress);    


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
        int _ipAddress;
};

#endif	// SERVER_HPP

