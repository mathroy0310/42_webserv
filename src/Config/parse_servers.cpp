/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_servers.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 15:43:13 by maroy             #+#    #+#             */
/*   Updated: 2024/04/13 00:12:22 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

void parse_server_directive(std::string &key, std::string &value, t_server &server) {
    value.erase(value.length() - 1, 1);
    if (key == "index")
        server.index = set_index(value, key);
    else if (key == "root")
        server.root = set_root(value, key);
    else if (key == "server_name")
        server.server_name = set_server_name(value, key);
    else if (key == "autoindex")
        server.is_autoindex = set_autoindex(value, key);
    else if (key == "listen")
        server.port = set_port_and_ip_address(value, key, server.ip_address);
    else if (key == "max_client_size")
        server.max_client_size = set_max_client_size(value, key);
    else if (key == "error_pages")
        set_error_pages(value, key, server.error_pages);
    else if (key == "allowed_methods")
        server.allowed_methods = set_allowed_methods(value, key);
	else if (key == "max_body_size")
        server.max_body_size = set_max_body_size(value, key);
    else {
        std::cerr << ERR_MSG_INVALID_DIRECTIVE(key) << FILE_LINE;
        exit(EXIT_FAILURE);
    }
}

void parse_server_line(std::string line, t_server &server) {
    size_t semi_colon_pos;
    size_t equal;

    semi_colon_pos = line.find(';');
    if (semi_colon_pos == line.npos && line.size() > 0) {
        std::cerr << ERR_MSG_MISSING_SEMICOLON << FILE_LINE;
        exit(EXIT_FAILURE);
    }
    std::string semi_colon_line = line.substr(0, semi_colon_pos + 1);
    equal = semi_colon_line.find('=');
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
    parse_server_directive(key, value, server);
}

static void init(t_server &server) {
    server.max_body_size = 0;
    server.max_client_size = MAX_CLIENTS;
    server.is_autoindex = false;
    server.port = -1;
    server.server_name = "";
    server.root = "";
    server.index = "";
    server.upload_path = "";
    server.redirect_code = -1;
}

t_server parse_server_block(std::string line) {
    t_server server;
    size_t semi_colon_pos;
    size_t first_bracket;
    size_t last_bracket;

    std::vector<t_location> locations;
    line = trim(line);
    init(server);
    while (line.length() > 0) {
        semi_colon_pos = line.find(";");
        if (semi_colon_pos == std::string::npos && line.size() > 0) {
            std::cerr << ERR_MSG_MISSING_SEMICOLON << FILE_LINE;
            exit(EXIT_FAILURE);
        }
        std::string semi_colon_line = line.substr(0, semi_colon_pos + 1);
        first_bracket = semi_colon_line.find('{');
        if (first_bracket != semi_colon_line.npos) {
            last_bracket = line.find('}');
            semi_colon_line = line.substr(0, last_bracket + 1);
            locations.push_back(parse_location_block(trim(semi_colon_line)));
            line.erase(line.find(semi_colon_line), semi_colon_line.length());
            continue;
        } else {
            semi_colon_line = trim(semi_colon_line);
            parse_server_line(semi_colon_line, server);
            line.erase(0, semi_colon_pos + 1);
        }
    }
    server.locations = locations;
    return (server);
}
