/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 18:03:36 by maroy             #+#    #+#             */
/*   Updated: 2024/03/22 18:05:49 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "defines.h"
# include "parsing.hpp"

class Client
{
	public:

		Client(std::string ip_address, int port);
		~Client(void);

	private:

		std::string _ip_address;
		int 		_port;
		int 		_socket_fd;

};

#endif // CLIENT_HPP
