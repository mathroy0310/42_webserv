/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 14:04:22 by maroy             #+#    #+#             */
/*   Updated: 2024/02/16 13:58:34 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include <cstdlib>
#include <stack>

/**
 * @brief Check if the brackets are balanced
 *
 * @param str the string to check
 * @return true the brackets are balanced
 * @return false the brackets are not balanced
 */
bool brackets_balance(const std::string &str)
{
	std::stack<char> s;
	for (size_t i = 0; i < str.length(); i++)
	{
		char c = str[i];
		if (c == '{' || c == '[')
			s.push(c);
		else if (c == '}' || c == ']')
		{
			if (s.empty())
				return (false);
			char top = s.top();
			if ((c == '}' && top == '{') || (c == ']' && top == '['))
				s.pop();
			else
				return (false);
		}
	}
	return (s.empty());
}

std::string trim(const std::string &str)
{
	size_t first = str.find_first_not_of(" \t");
	if (first == std::string::npos)
		return "";
	size_t last = str.find_last_not_of(" \t");
	return str.substr(first, (last - first + 1));
}

std::string remove_comments(const std::string &line)
{
	size_t comment_pos = line.find("#");
	if (comment_pos != std::string::npos)
		return trim(line.substr(0, comment_pos));
	else
		return trim(line);
}

int find_matching_open_curly_brace(const std::string &str)
{
	int nesting_level = 0;

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

int main(int argc, char **argv)
{
	if (argc == 1)
		argv[1] = (char *)"./conf/def.conf";
	if (argc > 2)
	{
		std::cerr << ERR_MSG_USAGE(argv[0]) << FILE_LINE;
		return EXIT_FAILURE;
	}

	std::ifstream conf_file(argv[1]);
	if (conf_file.fail())
	{
		std::cerr << ERR_MSG_READ(argv[1]) << FILE_LINE;
		return EXIT_FAILURE;
	}
	std::string line;
	std::string result;
	while (std::getline(conf_file, line))
	{
		line = remove_comments(line);
		if (line.empty())
			continue;
		result += line;
	}
	if (result.empty())
	{
		std::cerr << ERR_MSG_EMPTY(argv[1]) << FILE_LINE;
		return (EXIT_FAILURE);
	}
	if (!brackets_balance(result))
	{
		std::cerr << ERR_MSG_BRACKETS << FILE_LINE;
		return (EXIT_FAILURE);
	}

	// while(result.size() > 0)
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
	return EXIT_SUCCESS;
}
