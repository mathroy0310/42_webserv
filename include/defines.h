#ifndef DEFINES_H
#define DEFINES_H

#include <algorithm>
#include <arpa/inet.h>
#include <cerrno>  // for errno
#include <cstdlib>
#include <cstring>  // for strerror
#include <dirent.h>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <sstream>
#include <stack>
#include <string>
#include <sys/stat.h>
#include <unistd.h>  // for close
#include <vector>

/**********/
/* Logger */
/**********/
#include "Logger/Logger.hpp"
/***********/
/*  Macros */
/***********/
#define MAX_CLIENTS SOMAXCONN
// #define BUFFER_SIZE 16384
#define BUFFER_SIZE 4096
/**********/
/* Colors */
/**********/
#define RESET_COLOR "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define ORANGE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define GREY "\033[30;1m"
#define CYAN "\033[1;36m"
#define WHITE "\033[1;37m"

/************/
/* Prefixes */
/************/
#define ERR_PREFIX RED "Error: "
#define WARN_PREFIX YELLOW "Warning: "
#define INFO_PREFIX CYAN "Info: "
#define DEBUG_PREFIX MAGENTA "Debug: "

/*********************/
/* Debugging Macros */
/*********************/
#define FILE_LINE YELLOW "[" << __FILE__ << ":" << __LINE__ << "]" << RESET_COLOR << std::endl

/**************************/
/* HTTP Request Constants */
/**************************/
#define REQ_METHOD "Request-Method"
#define REQ_PATH "REQUEST_URI"
#define REQ_HTTP_VERSION "Server-Protocol"
#define REQ_SERVER_NAME "Server-Name"
#define REQ_PORT "Server-Port"
#define REQ_QUERY_STRING "QUERY_STRING"
#define REQ_ENTITY_POST "Entity_Post"
#define REQ_CONTENT_TYPE "Content-Type"
#define REQ_TRANSFER "Transfer-Encoding"
#define REQ_CONTENT_LENGTH "Content-Length"
#define REQ_CONNECTION "Connection"
#define REQ_COOKIES "Cookie"

#define ALLOWED_URI_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%"

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
#define METHOD_NOT_ALLOWED_STATUS 405
#define CONTENT_TOO_LARGE_STATUS 413
#define IM_A_TEAPOT_STATUS 418            // april fools joke from 1998 lol
#define INTERNAL_SERVER_ERROR_STATUS 500  // segfault
#define NOT_IMPLEMENTED_STATUS 501
#define BAD_GATEWAY_STATUS 502
#define SERVICE_UNAVAILABLE_STATUS 503
#define HTTP_VERSION_NOT_SUPPORTED_STATUS 505

/******************/
/* Error Messages */
/******************/
#define RESET_NL RESET_COLOR << std::endl
#define ERR_MSG_USAGE(arg) ERR_PREFIX << "Usage: " + std::string(arg) + " [file]" << RESET_NL
#define ERR_MSG_READ(arg) ERR_PREFIX << std::string(arg) + " " + strerror(errno) << RESET_NL
#define ERR_MSG_EMPTY(arg) ERR_PREFIX << std::string(arg) + " is empty" << RESET_NL
#define ERR_MSG_BRACKETS ERR_PREFIX << "Brackets are not balanced" << RESET_NL
#define ERR_MSG_SERVER_BLOCK_MISSING ERR_PREFIX << "server block is missing" << RESET_NL
#define ERR_MSG_UNEXPECTED_TOKEN(arg) \
    ERR_PREFIX << "Unexpected token before the " + std::string(arg) + " block" << RESET_NL
#define ERR_MSG_MISSING_SEMICOLON ERR_PREFIX << "Expected ';' at the end of declaration" << RESET_NL
#define ERR_MSG_MISSING_EQUAL ERR_PREFIX << "Expected '=' after directive " << RESET_NL
#define ERR_MSG_INVALID_LOCATION ERR_PREFIX << "Invalid location block" << RESET_NL
#define ERR_MSG_NO_LOCATION_PATH ERR_PREFIX << "No path for location" << RESET_NL
#define ERR_MSG_NO_VALUE(arg) ERR_PREFIX << "No value for " + std::string(arg) << RESET_NL
#define ERR_MSG_INVALID_VALUE(arg, value) \
    ERR_PREFIX << "Invalid value for " + std::string(arg) + ": " + std::string(value) << RESET_NL
#define ERR_MSG_INVALID_METHOD(arg) ERR_PREFIX << "Invalid method: " + std::string(arg) << RESET_NL
#define ERR_MSG_INVALID_DIRECTIVE(arg) ERR_PREFIX << "Invalid directive: " + std::string(arg) << RESET_NL

#define ERR_PAGE(err, name)                                                                                    \
    "<html><head><title>" + std::string(err) + "</title></head><body><center><h1>" + std::string(err) + \
        "</h1></center><hr><center>" + name + "</center></body></html>"
#define DEFAULT_PAGE                                                                                                   \
    "<!DOCTYPE html><html><head><title>Welcome to "                                                                    \
    "Webserv!</"                                                                                                       \
    "title><style>body{font-family:Arial,sans-serif;background-color:#f4f4f4;color:#333;margin:20px;}h1{color:#"       \
    "2e8b57;}p{color:#555;}ul{list-style-type:none;padding:0;}li{margin-bottom:10px;}a{color:#0066cc;text-decoration:" \
    "none;font-weight:bold;}a:hover{text-decoration:underline;}</style></head><body><h1>Welcome to "                   \
    "Webserv!</h1></body></html>"

#endif  // DEFINES_H
