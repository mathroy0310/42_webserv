/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 14:04:22 by maroy             #+#    #+#             */
/*   Updated: 2024/02/16 01:07:46 by maroy            ###   ########.fr       */
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

int main(int argc, char **argv)
{
	if (argc == 1)
		argv[1] = (char *)"./conf/def.conf";
	if (argc > 2)
	{
		std::cerr << "Usage: " << argv[0] << " [file]" << RESET_COLOR << std::endl;
		std::cerr << FILE_LINE;
		return EXIT_FAILURE;
	}

	std::ifstream conf_file(argv[1]);
	if (conf_file.fail())
	{
		std::cerr << ERR_PREFIX << argv[1] << " does not exist" << RESET_COLOR << std::endl;
		std::cerr << FILE_LINE;
		return EXIT_FAILURE;
	}
	std::string line;
	std::string result;
	while (std::getline(conf_file, line))
	{
		size_t comment_pos = line.find("#");
		if (comment_pos != std::string::npos)
			line = line.erase(comment_pos, line.length());

		// trim
		// pourrait être fait avec un regex ??? insane
		// surtout devrait etre dans une fonction
		size_t first = line.find_first_not_of(" \t");
		if (first == std::string::npos)
			continue;
		size_t last = line.find_last_not_of(" \t");
		line = line.substr(first, (last - first + 1));
		// fin de la fonction qui trim

		if (line.empty())
			continue;
		result += line;
	}
	if (result.empty())
	{
		std::cerr << ERR_PREFIX << argv[1] << " is empty" << RESET_COLOR << std::endl;
		std::cerr << FILE_LINE;
		return EXIT_FAILURE;
	}
	if (!brackets_balance(result))
	{
		std::cerr << ERR_PREFIX << "Brackets are not balanced" << RESET_COLOR << std::endl;
		std::cerr << FILE_LINE;
		return EXIT_FAILURE;
	}

	std::cout << GREEN << "Result:" << RESET_COLOR << std::endl;
	std::cout << result << std::endl;
	return EXIT_SUCCESS;
}
