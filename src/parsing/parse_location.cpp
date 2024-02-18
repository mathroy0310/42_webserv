/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_location.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 21:09:39 by maroy             #+#    #+#             */
/*   Updated: 2024/02/17 21:28:55 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

static void init(t_location &location)
{
	location.index = "";
	location.root = "";
	location.path = "";
}

t_location parse_location_block(std::string line)
{
	t_location location;

	init(location);
	if (line.substr(0, 8) != "location")
	{
		std::cerr << ERR_MSG_INVALID_LOCATION << FILE_LINE;
		exit(EXIT_FAILURE);
	}
	return (location);
}
