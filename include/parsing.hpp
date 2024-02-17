/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:50:28 by maroy             #+#    #+#             */
/*   Updated: 2024/02/17 16:27:49 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
#define PARSING_HPP

#pragma once

#include <vector>
#include <stack>
#include <iostream>
#include "defines.h"

/*********************/
/* directives Struct */
/*********************/

// Location directive
typedef struct s_location
{
	std::string path;
	std::string root;
	std::string index;
} t_location;

// Server directive
typedef struct s_server
{
	int port;
	std::string server_name;
	std::string root;
	std::string index;
	std::vector<t_location> locations;
} t_server;

// Config Settings
typedef struct s_config
{
	std::vector<t_server> servers;
} t_config;

/******************/
/* parse_conf.cpp */
/******************/

t_config parse_conf(const char *file);

/******************/
/* parse_conf.cpp */
/******************/

std::string remove_comments(const std::string &line);
bool can_read_file(const std::string filename, const std::ifstream &file);
bool is_empty_file(const std::string &result, const std::string &file);
std::string process_file(std::ifstream &file);

/*********************/
/* parse_servers.cpp */
/*********************/

t_server parse_server_block(std::string line);

/*******************/
/* parse_utils.cpp */
/*******************/

bool brackets_balance(const std::string &str);
std::string trim(const std::string &str);

#endif // PARSING_HPP