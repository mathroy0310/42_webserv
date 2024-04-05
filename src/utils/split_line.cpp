/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_line.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 02:24:54 by maroy             #+#    #+#             */
/*   Updated: 2024/04/05 03:10:48 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"

/**
 * @brief Split a string into a vector of string using a delimiter
 *
 * @param line the line to split
 * @param delim the delimiter to use
 * @return `std::vector<std::string>` the vector of the line splitted
 */
std::vector<std::string> split_line(std::string line, std::string delim) {
    std::vector<std::string> result;
    std::string tmp_line;
    size_t end;

    end = 0;
    while (line.size()) {
        end = line.find(delim);
        if (end == std::string::npos)
            end = line.length();
        if (end > 0) {
            tmp_line = line.substr(0, end);
            result.push_back(tmp_line);
        }
        if (end + delim.size() > line.size())
            break;
        line = &line[end + delim.size()];
    }
    return (result);
}