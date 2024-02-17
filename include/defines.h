/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 21:50:28 by maroy             #+#    #+#             */
/*   Updated: 2024/02/17 03:45:00 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_H
#define DEFINE_H

#pragma once
#include <iostream>
#include <fstream>
#include <cerrno>  // for errno
#include <cstring> // for strerror
#include <cstdlib>

/**********/
/* Colors */
/**********/
#define RESET_COLOR "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define ORANGE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define WHITE "\033[1;37m"

/************/
/* Prefixes */
/************/
#define ERR_PREFIX RED "Error: "
#define WARN_PREFIX "Warning: "
#define INFO_PREFIX "Info: "
#define DEBUG_PREFIX "Debug: "

/*********************/
/* Debugging Macros */
/*********************/
#define FILE_LINE YELLOW "[" << __FILE__ << ":" << __LINE__ << "]" << RESET_COLOR << std::endl

/*********************/
/* HTTP Status Codes */
/*********************/
#define OK_STATUS 200
#define MOVED_PERMANENTLY_STATUS 301
#define FOUND_STATUS 302
#define SEE_OTHER_STATUS 303
#define NOT_MODIFIED_STATUS 304
#define BAD_REQUEST_STATUS 400
#define UNAUTHORIZED_STATUS 401
#define PAYMENT_REQUIRED_STATUS 402
#define FORBIDDEN_STATUS 403
#define NOT_FOUND_STATUS 404
#define IM_A_TEAPOT_STATUS 418			 // april fools joke from 1998 lol
#define INTERNAL_SERVER_ERROR_STATUS 500 // segfault

/******************/
/* Error Messages */
/******************/
#define RESET_NL RESET_COLOR << std::endl
#define ERR_MSG_USAGE(arg) ERR_PREFIX << "Usage: " + std::string(arg) + " [file]" << RESET_NL
#define ERR_MSG_READ(arg) ERR_PREFIX << std::string(arg) + " " + strerror(errno) << RESET_NL
#define ERR_MSG_EMPTY(arg) ERR_PREFIX << std::string(arg) + " is empty" << RESET_NL
#define ERR_MSG_BRACKETS ERR_PREFIX << "Brackets are not balanced" << RESET_NL
#define ERR_MSG_SERVER_BLOCK_MISSING ERR_PREFIX << "server block is missing" << RESET_NL
#define ERR_MSG_UNEXPECTED_TOKEN(arg) ERR_PREFIX << "Unexpected token in the " + std::string(arg) + " block" << RESET_NL

#endif // DEFINES_H
