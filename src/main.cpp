/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 14:04:22 by maroy             #+#    #+#             */
/*   Updated: 2024/03/22 17:11:26 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include "parsing.hpp"
//#include "Server.hpp"
#include "Logger.hpp"

int main(int argc, char **argv)
{
	if (argc == 1)
		argv[1] = (char *)"./conf/def.conf";
	if (argc > 2)
	{
		std::cerr << ERR_MSG_USAGE(argv[0]) << FILE_LINE;
		return EXIT_FAILURE;
	}
	Logger logger("webserv.log");
	logger.log("Logger is Enabled");

	t_config config = parse_conf(argv[1]);

	logger.log("Config file parsed successfully");
	for (size_t i = 0; i < config.servers.size(); i++)
	{
		logger.log("Server " + std::to_string(i) + " name: ", config.servers[i].server_name);
		logger.log("Server " + std::to_string(i) + " port: ", std::to_string(config.servers[i].port));
		logger.log("Server " + std::to_string(i) + " root: ", config.servers[i].root);
		logger.log("Server " + std::to_string(i) + " index: ", config.servers[i].index);
		logger.log("Server " + std::to_string(i) + " autoindex: ", config.servers[i].is_autoindex ? "true" : "false");
		logger.log("Server " + std::to_string(i) + " max body size: ", std::to_string(config.servers[i].max_body_size));
		for (size_t j = 0; j < config.servers[i].locations.size(); j++)
		{
			logger.log("Location " + std::to_string(j) + " path: ", config.servers[i].locations[j].path);
			logger.log("Location " + std::to_string(j) + " root: ", config.servers[i].locations[j].root);
			logger.log("Location " + std::to_string(j) + " index: ", config.servers[i].locations[j].index);
			logger.log("Location " + std::to_string(j) + " autoindex: ", config.servers[i].locations[j].is_autoindex ? "true" : "false");
			logger.log("Location " + std::to_string(j) + " max body size: ", std::to_string(config.servers[i].locations[j].max_body_size));
		}	
	}
	
	(void)config;
	// Server server(config);

	// server.convertIPToInt(argv[1]);
	return (0);
}
