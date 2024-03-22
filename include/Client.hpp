/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 18:03:36 by maroy             #+#    #+#             */
/*   Updated: 2024/03/22 18:28:53 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "defines.h"
# include "parsing.hpp"

class Client
{
	public:

		Client(std::string ip_address);
		~Client(void);

	private:

		std::string _ip_address;
		// const int 		_port;
		// const int 		_socket_fd;

};

#endif // CLIENT_HPP
