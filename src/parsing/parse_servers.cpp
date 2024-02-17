/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_servers.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 15:43:13 by maroy             #+#    #+#             */
/*   Updated: 2024/02/17 16:39:52 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

void parse_server_line(std::string line, t_server &server)
{
	size_t semi_colon_pos = line.find(';');
	if (semi_colon_pos == line.npos && line.size() > 0)
	{
		std::cerr << ERR_MSG_MISSING_SEMICOLON << FILE_LINE;
		exit(EXIT_FAILURE);
	}
	std::string semi_colon_line = line.substr(0, semi_colon_pos + 1);
	size_t equal = semi_colon_line.find('=');
	if (equal == semi_colon_line.npos)
	{
		std::cerr << ERR_MSG_MISSING_EQUAL << FILE_LINE;
		exit(EXIT_FAILURE);
	}
	std::string key = trim(semi_colon_line.substr(0, equal));
	std::string value = trim(semi_colon_line.substr(equal + 1, -1));
	if (value.find("=") != value.npos)
	{
		std::cerr << ERR_MSG_MISSING_SEMICOLON << FILE_LINE;
		exit(EXIT_FAILURE);
	}
	value.erase(value.length() - 1, 1);
	if (key == "server_name")
	{
		server.server_name = value;
		std::cout << "server_name: " << server.server_name << std::endl;
	}
	// TODO: check et parse la key et la value ofc , on defrait faire des get et set pour valeurs
}

static void init(t_server &server)
{
	server.port = -1;
	server.server_name = "";
	server.root = "";
	server.index = "";
}

t_server parse_server_block(std::string line)
{
	t_server server;
	t_location location;

	line = trim(line);
	init(server);
	while (line.length() > 0)
	{
		size_t semi_colon_pos = line.find(";");
		if (semi_colon_pos == std::string::npos)
		{
			std::cerr << ERR_MSG_MISSING_SEMICOLON << FILE_LINE;
			exit(EXIT_FAILURE);
		}
		std::string semi_colon_line = line.substr(0, semi_colon_pos + 1);
		size_t first_bracket = semi_colon_line.find("{");
		if (first_bracket != semi_colon_line.npos)
		{
			size_t last_bracket = semi_colon_line.find("}");
			semi_colon_line = line.substr(0, last_bracket + 1);
			// parse location
			// doit parse les block locations
			line.erase(0, 8);
			line.erase(line.length() - 1, 1);
			line = trim(line);
			size_t bracket = line.find("{");
			if (bracket == line.npos)
			{
				std::cerr << "error location" << FILE_LINE;
				exit(EXIT_FAILURE);
			}
			std::cout << DEBUG_PREFIX << "location block found and valid ?" << std::endl;
			line = trim(line.substr(bracket + 1, -1));
			line.erase(line.find(semi_colon_line), semi_colon_line.length());
			continue;
		}
		else
		{
			semi_colon_line = trim(semi_colon_line);
			parse_server_line(semi_colon_line, server);
			line.erase(0, semi_colon_pos + 1);
		}
	}
	return (server);
}