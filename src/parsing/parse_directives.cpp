/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_directives.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 20:48:46 by maroy             #+#    #+#             */
/*   Updated: 2024/02/19 01:29:20 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

std::string set_index(std::string &value, const std::string &key) {
    if (value.empty() || value == ";") {
        std::cerr << ERR_MSG_NO_VALUE(key) << FILE_LINE;
        exit(EXIT_FAILURE);
    }
    return (value);
}

bool set_autoindex(std::string &value, const std::string &key) {
    if (value.empty() || value == ";") {
        std::cerr << ERR_MSG_NO_VALUE(key) << FILE_LINE;
        exit(EXIT_FAILURE);
    }
    if (value == "0" || value == "false" || value == "FALSE" || value == "off" || value == "OFF")
        return (false);
    else if (value == "1" || value == "true" || value == "TRUE" || value == "on" || value == "ON")
        return (true);
    else {
        std::cerr << ERR_MSG_INVALID_VALUE(key, value) << FILE_LINE;
        exit(EXIT_FAILURE);
    }
}

std::string set_root(std::string &value, const std::string &key) {
    value = trim(value);
    if (value.empty() || value == ";") {
        std::cerr << ERR_MSG_NO_VALUE(key) << FILE_LINE;
        exit(EXIT_FAILURE);
    }
    return (value);
}

std::string set_server_name(std::string &value, const std::string &key) {
    value = trim(value);
    if (value.empty() || value == ";") {
        std::cerr << ERR_MSG_NO_VALUE(key) << FILE_LINE;
        exit(EXIT_FAILURE);
    }
    return (value);
}
