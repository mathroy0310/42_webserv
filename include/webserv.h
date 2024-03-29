/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:55:02 by maroy             #+#    #+#             */
/*   Updated: 2024/03/25 22:21:46 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_H
# define WEBSERV_H

# include "defines.h"
# include "parsing.hpp"
# include "classes/Server.hpp"
# include "classes/Client.hpp"
# include "classes/Multiplexer.hpp"
# include "classes/HTTPResponse.hpp"
# include "classes/HTTPRequest.hpp"
# include "classes/SocketWrapper.hpp"
# include "classes/Logger.hpp"

extern Logger g_logger;

#endif // WEBSERV_H