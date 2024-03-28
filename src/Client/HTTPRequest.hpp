/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 19:51:11 by rmarceau          #+#    #+#             */
/*   Updated: 2024/03/28 13:08:42 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.h"

class HTTPRequest {
public:
    // Constructors & Destructors
    HTTPRequest(void);
    ~HTTPRequest(void);

    // Operators
    HTTPRequest &operator=(HTTPRequest const &rhs);

    // Methods
    void parse(const std::string &raw_request);
    void clear(void);

    // Getters
    std::string getMethod(void) const;
    std::string getURI(void) const;
    std::string getVersion(void) const;
    std::map<std::string, std::string> getHeaders(void) const;
    std::string getBody(void) const;

private:
    // Attributes
    std::string _method;
    std::string _uri;
    std::string _version;
    std::map<std::string, std::string> _headers;
    std::string _body;

    // Methods
    void parseRequestLine(const std::string &line);
    void parseHeaders(const std::string &line);
    void parseBody(const std::string &line);
};
