/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_location.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 21:09:39 by maroy             #+#    #+#             */
/*   Updated: 2024/02/18 20:59:46 by maroy            ###   ########.fr       */
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
	line.erase(0, 8);
	line.erase(line.length() - 1 , 1);
	line = trim(line);
	std::cout << INFO_PREFIX << "Location block parsed successfully" << RESET_COLOR << std::endl;
	return (location);
}
