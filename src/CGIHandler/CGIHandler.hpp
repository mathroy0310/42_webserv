/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarceau <rmarceau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:58:56 by rmarceau          #+#    #+#             */
/*   Updated: 2024/04/10 17:02:01 by rmarceau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.h"
#include "Request/HTTPRequest.hpp"

class HTTPRequest;

class CGIHandler {
public:
    CGIHandler(HTTPRequest *request, t_server &server, std::string _path);
    ~CGIHandler(void);

    std::string executeCGI(void);

private:
    void setupEnvironment(void);

    HTTPRequest *_request;
    t_server _server;
    std::string _path;
};