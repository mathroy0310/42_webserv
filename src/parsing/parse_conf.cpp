/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_conf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 03:40:44 by maroy             #+#    #+#             */
/*   Updated: 2024/02/17 16:28:05 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

ssize_t find_matching_open_curly_brace(const std::string &str)
{
	ssize_t nesting_level = 0;

	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == '{')
			nesting_level++;
		else if (str[i] == '}')
		{
			nesting_level--;
			if (nesting_level == 0)
				return (i); // Found the matching open curly brace
		}
	}
	return (-1); // No matching open curly brace found
}

// ne devrait pas retourner int mais genre struct ou classe parse
t_config parse_conf(const char *file)
{
	std::string line;
	std::string result;
	t_config config;

	std::ifstream conf_file(file);
	if (!can_read_file(file, conf_file))
		exit(EXIT_FAILURE);
	result = process_file(conf_file);
	if (is_empty_file(result, file))
		exit(EXIT_FAILURE);
	if (!brackets_balance(result))
	{
		std::cerr << ERR_MSG_BRACKETS << FILE_LINE;
		exit(EXIT_FAILURE);
	}

	// while (result.size() > 0)
	{
		result = trim(result);
		if (trim(result.substr(0, 6)) != "server")
		{
			std::cerr << ERR_MSG_SERVER_BLOCK_MISSING << FILE_LINE;
			exit(EXIT_FAILURE);
		}
		result = trim(result.substr(6, -1));
		if (result[0] != '{')
		{
			std::cerr << ERR_MSG_UNEXPECTED_TOKEN("server") << FILE_LINE;
			exit(EXIT_FAILURE);
		}
		int block_len = find_matching_open_curly_brace(result);
		if (block_len == -1)
		{
			std::cerr << ERR_MSG_BRACKETS << FILE_LINE;
			exit(EXIT_FAILURE);
		}
		result.erase(0, 1);
		result.erase(block_len - 1, 1);
		line = result.substr(0, block_len - 1);
		config.servers.push_back(parse_server_block(line));
		result.erase(result.find(line), line.length());
	}
	return (config);
}