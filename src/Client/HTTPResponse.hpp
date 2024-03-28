/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 19:51:08 by rmarceau          #+#    #+#             */
/*   Updated: 2024/03/28 13:07:20 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.h"

class HTTPResponse {
public:
    // Constructors & Destructors
    HTTPResponse(void);
    ~HTTPResponse(void);

    // Methods
    std::string serialize(void) const;

    // Setters
    void setVersion(const std::string &version);
    void setStatusCode(const std::string &status_code);
    void setStatusMessage(const std::string &reason_phrase);
    void setHeaders(const std::map<std::string, std::string> &headers);
    void setBody(const std::string &body);

    // Getters
    std::string getVersion(void) const;
    std::string getStatusCode(void) const;
    std::string getStatusMessage(void) const;
    std::map<std::string, std::string> getHeaders(void) const;
    std::string getBody(void) const;
    
private:
    // Attributes
    std::string _version;
    std::string _status_code;
    std::string _status_message;
    std::map<std::string, std::string> _headers;
    std::string _body;
};
