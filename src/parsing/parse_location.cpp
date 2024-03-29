/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_location.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 21:09:39 by maroy             #+#    #+#             */
/*   Updated: 2024/03/22 17:11:48 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

void parse_location_directives(std::string &key, std::string &value, t_location &location) {
    value.erase(value.length() - 1, 1);
    if (key == "index")
        location.index = set_index(value, key);
    else if (key == "root")
        location.root = set_root(value, key);
    else if (key == "autoindex")
        location.is_autoindex = set_autoindex(value, key);
    else if (key == "error_pages")
        set_error_pages(value, key, location.error_pages);
    else {
        std::cerr << ERR_MSG_INVALID_DIRECTIVE(key) << FILE_LINE;
        exit(EXIT_FAILURE);
    }
}

void parse_location_lines(std::string &line, t_location &location) {
    while (line.length()) {
        size_t semi_colon_pos = line.find(';');
        if (semi_colon_pos == line.npos && line.size() > 0) {
            std::cerr << ERR_MSG_MISSING_SEMICOLON << FILE_LINE;
            exit(EXIT_FAILURE);
        }
        std::string semi_colon_line = line.substr(0, semi_colon_pos + 1);
        size_t equal = semi_colon_line.find('=');
        if (equal == semi_colon_line.npos) {
            std::cerr << ERR_MSG_MISSING_EQUAL << FILE_LINE;
            exit(EXIT_FAILURE);
        }
        std::string key = trim(semi_colon_line.substr(0, equal));
        std::string value = trim(semi_colon_line.substr(equal + 1, -1));
        if (value.find("=") != value.npos) {
            std::cerr << ERR_MSG_MISSING_SEMICOLON << FILE_LINE;
            exit(EXIT_FAILURE);
        }
        parse_location_directives(key, value, location);
        line.erase(0, semi_colon_pos + 1);
    }
}

static void init(t_location &location) {
    location.is_autoindex = false;
    location.max_body_size = 0;
    location.index = "";
    location.root = "";
    location.path = "";
}

t_location parse_location_block(std::string line) {
    t_location location;
    size_t bracket;

    init(location);
    if (line.substr(0, 8) != "location") {
        std::cerr << ERR_MSG_INVALID_LOCATION << FILE_LINE;
        exit(EXIT_FAILURE);
    }
    line.erase(0, 8);
    line.erase(line.length() - 1, 1);
    line = trim(line);
    bracket = line.find('{');
    if (bracket == 0) {
        std::cerr << ERR_MSG_NO_LOCATION_PATH << FILE_LINE;
        exit(EXIT_FAILURE);
    }
    location.path = trim(line.substr(0, bracket));

    line = trim(line.substr(bracket + 1, -1));
    parse_location_lines(line, location);
    return (location);
}
