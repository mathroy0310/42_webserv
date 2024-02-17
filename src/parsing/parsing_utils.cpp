/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 03:52:24 by maroy             #+#    #+#             */
/*   Updated: 2024/02/17 03:56:39 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include "parsing.h"
#include <stack>

/**
 * @brief Check if the brackets are balanced
 *
 * @param str the string to check
 * @return `true` the brackets are balanced
 * @return `false` the brackets are not balanced
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

/**
 * @brief Remove space and tabulation from the beginning and the end of a string
 *
 * @param line the line to trim
 * @return `std::string` the trimmed line
 */
std::string trim(const std::string &str)
{
	size_t first = str.find_first_not_of(" \t");
	if (first == std::string::npos)
		return "";
	size_t last = str.find_last_not_of(" \t");
	return str.substr(first, (last - first + 1));
}