/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_conf_utils.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 15:38:25 by maroy             #+#    #+#             */
/*   Updated: 2024/02/18 20:02:45 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

/// @brief remove comments from a line
/// @param line line to process
/// @return without comments and trimmed 
std::string remove_comments(const std::string &line)
{
	size_t comment_pos = line.find("#");
	if (comment_pos != std::string::npos)
		return trim(line.substr(0, comment_pos));
	else
		return trim(line);
}

/// @brief Basic check to see if the file can be read
/// @param filename the name of the file
/// @param file the file to check
/// @return `bool` true if the file can be read false otherwise
bool can_read_file(const std::string filename, const std::ifstream &file)
{
	if (file.fail())
	{
		std::cerr << ERR_MSG_READ(filename) << FILE_LINE;
		return (false);
	}
	return (true);
}

/// @brief Check if the file is empty 
/// @param result the file content
/// @param file the filename to check
/// @return true if the file is empty false otherwise
bool is_empty_file(const std::string &result, const std::string &file)
{
	if (result.empty())
	{
		std::cerr << ERR_MSG_EMPTY(file) << FILE_LINE;
		return (true);
	}
	return (false);
}

/// @brief Remove comments from a file line by line
/// @param file the file to process
/// @return `std::string` the file without comments
std::string process_file(std::ifstream &file)
{
	std::string line;
	std::string result;

	while (std::getline(file, line))
	{
		line = remove_comments(line);
		if (!line.empty())
			result += line;
	}
	return result;
}

/**
 * @brief Check if the brackets are balanced
 *
 * @param str the string to check
 * @return `true` the brackets are balanced
 * @return `false` the brackets are not balanced
 */
bool are_brackets_balanced(const std::string &str)
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
			{
				std::cerr << ERR_MSG_BRACKETS << FILE_LINE;
				return (false);
			}
			char top = s.top();
			if ((c == '}' && top == '{') || (c == ']' && top == '['))
				s.pop();
			else
			{
				std::cerr << ERR_MSG_BRACKETS << FILE_LINE;
				return (false);
			}
		}
	}
	return (s.empty());
}
