/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_directives.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 20:48:46 by maroy             #+#    #+#             */
/*   Updated: 2024/02/18 20:57:45 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

std::string set_index(std::string &value, std::string &key)
{
	if (value.empty() || value == ";") {
		std::cerr << ERR_MSG_NO_VALUE(key) << FILE_LINE;
		exit(EXIT_FAILURE);
	}
	return (value);
}
