/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 19:51:11 by rmarceau          #+#    #+#             */
/*   Updated: 2024/04/20 23:51:28 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.h"

class HTTPRequest {
  public:
    // Constructors & Destructors
    HTTPRequest(void);
    ~HTTPRequest(void);

    // Methods
    void clear(void);

    // Getters
    std::string getMethod(void);
    std::string getURI(void);
    std::string &getBody(void);
    std::map<std::string, std::string> &getHeaders(void);
    bool getHeaderEnd(void) const;
    size_t getContentLenght(void);
    void addToHeader(std::string key, std::string value);
    std::string &getValueByKey(std::string key);
    void appendFile(const char *buffer, int len);
    void appendHeader(const char *buffer, int len);

  private:
    // Attributes
    std::string _method;
    std::string _uri;
    std::map<std::string, std::string> _headers;
    std::string _body;
    bool _isHeaderEnd;
    std::string _headerStr;
    size_t _total;
};
