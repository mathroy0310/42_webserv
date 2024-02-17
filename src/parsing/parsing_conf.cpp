/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_conf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 03:40:44 by maroy             #+#    #+#             */
/*   Updated: 2024/02/17 04:02:55 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include "parsing.h"

std::string remove_comments(const std::string &line)
{
	size_t comment_pos = line.find("#");
	if (comment_pos != std::string::npos)
		return trim(line.substr(0, comment_pos));
	else
		return trim(line);
}

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
int parse_conf(const char *file)
{
	std::string line;
	std::string result;
	
	std::ifstream conf_file(file);
	if (conf_file.fail())
	{
		std::cerr << ERR_MSG_READ(file) << FILE_LINE;
		return EXIT_FAILURE;
	}
	while (std::getline(conf_file, line))
	{
		line = remove_comments(line);
		if (line.empty())
			continue;
		result += line;
	}
	if (result.empty())
	{
		std::cerr << ERR_MSG_EMPTY(file) << FILE_LINE;
		return (EXIT_FAILURE);
	}
	if (!brackets_balance(result))
	{
		std::cerr << ERR_MSG_BRACKETS << FILE_LINE;
		return (EXIT_FAILURE);
	}

	// while (result.size() > 0)
	{
		result = trim(result);
		if (trim(result.substr(0, 6)) != "server")
		{
			std::cerr << ERR_MSG_SERVER_BLOCK_MISSING << FILE_LINE;
			return (EXIT_FAILURE);
		}
		result = trim(result.substr(6, -1));
		if (result[0] != '{')
		{
			std::cerr << ERR_MSG_UNEXPECTED_TOKEN("server") << FILE_LINE;
			return (EXIT_FAILURE);
		}
		int block_len = find_matching_open_curly_brace(result);
		if (block_len == -1)
		{
			std::cerr << ERR_MSG_BRACKETS << FILE_LINE;
			return (EXIT_FAILURE);
		}
		result.erase(0, 1);
		result.erase(block_len - 1, 1);
		line = result.substr(0, block_len - 1);
	}

	std::cout << GREEN << "Result:" << RESET_COLOR << std::endl;
	std::cout << result << std::endl;
	return (EXIT_SUCCESS);
}