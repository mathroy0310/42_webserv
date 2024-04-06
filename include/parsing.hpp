/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:50:28 by maroy             #+#    #+#             */
/*   Updated: 2024/04/05 21:45:02 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
#define PARSING_HPP

#pragma once

#include "defines.h"
#include "utils.h"

/*********************/
/* directives Struct */
/*********************/

// Location directive
typedef struct s_location {
    int redirect_code;
    std::string redirect_to;
    bool is_autoindex;
    unsigned long long max_body_size;
    std::string path;
    std::string root;
    std::string index;
    std::vector<std::string> allowed_methods;
    std::map<unsigned int, std::string> error_pages;
} t_location;

// Server directive
typedef struct s_server {
    int redirect_code;
    std::string redirect_to;
    int port;
    std::string ip_address;
    bool is_autoindex;
    unsigned long long max_body_size;
    size_t max_client_size;
    std::string server_name;
    std::string root;
    std::string index;
    std::vector<std::string> allowed_methods;
    std::map<unsigned int, std::string> error_pages;
    std::vector<t_location> locations;
} t_server;

// Config Settings
typedef struct s_config {
    std::vector<t_server> servers;
} t_config;

/******************/
/* parse_conf.cpp */
/******************/

t_config parse_conf(const char *file);

/******************/
/* parse_conf_utils.cpp */
/******************/

bool are_brackets_balanced(const std::string &str);
std::string remove_comments(const std::string &line);
bool can_read_file(const std::string filename, const std::ifstream &file);
bool is_empty_file(const std::string &result, const std::string &file);
std::string process_file(std::ifstream &file);

/*********************/
/* parse_servers.cpp */
/*********************/

t_server parse_server_block(std::string line);

/***********************/
/* parse_locations.cpp */
/***********************/

t_location parse_location_block(std::string line);

/***********************/
/* parse_directives.cpp */
/***********************/

std::vector<std::string> set_allowed_methods(std::string &value, const std::string &key);
std::string set_index(std::string &value, const std::string &key);
bool set_autoindex(std::string &value, const std::string &key);
std::string set_root(std::string &value, const std::string &key);
std::string set_server_name(std::string &value, const std::string &key);
int set_port_and_ip_address(std::string &value, const std::string &key, std::string &ip_address);
void set_error_pages(std::string &value, const std::string &key, std::map<unsigned int, std::string> &error_pages);
size_t set_max_client_size(std::string &value, const std::string &key);
void set_redirect(std::string &value, const std::string &key, int &redirect_code, std::string &redirect_to);
unsigned long long set_max_body_size(std::string &value, const std::string &key);

/***********************/
/* parse_request.cpp */
/***********************/
void check_request(std::string buffer);
void parse_start_line(std::map<std::string, std::string> &headermap, std::string line);
void parse_header(std::map<std::string, std::string> &headermap, std::vector<std::string> lines);
void set_key_and_value(std::string line, std::map<std::string, std::string> &headermap);
void check_path(std::string path, std::string connection);

#endif  // PARSING_HPP
