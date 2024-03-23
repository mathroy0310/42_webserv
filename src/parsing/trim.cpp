/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 03:52:24 by maroy             #+#    #+#             */
/*   Updated: 2024/02/19 01:29:13 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

/**
 * @brief Remove space and tabulation from the beginning and the end of a string
 *
 * @param line the line to trim
 * @return `std::string` the trimmed line
 */
std::string trim(const std::string &str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}
