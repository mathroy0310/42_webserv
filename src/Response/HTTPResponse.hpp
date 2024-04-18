/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 19:51:08 by rmarceau          #+#    #+#             */
/*   Updated: 2024/04/18 02:51:38 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "CGIHandler/CGIHandler.hpp"
#include "Request/HTTPRequest.hpp"
#include "defines.h"

class CGIHandler;

class HTTPResponse {
  public:
    // Constructors & Destructors
    HTTPResponse(void);
    HTTPResponse(int status, t_server &server);
    HTTPResponse(HTTPRequest *request, t_server &server);
    ~HTTPResponse(void);

    // Setters
    void setVersion(const std::string &version);
    void setStatusCode(int status_code);
    void setStatusMessage(const std::string &reason_phrase);
    void setHeaders(int status);
    void setBody(const std::string &body);

    // Getters
    std::string getVersion(void) const;
    std::string getStatusCode(void) const;
    std::string getStatusMessage(void) const;
    std::map<std::string, std::string> getHeaders(void) const;
    std::string getBody(void) const;

    std::string buildResponse(void);

    HTTPRequest *getRequest(void) const;
    const std::string &getResponse(void) const;
    bool getUploaded(void) const;


  private:
    void executeCGI(void);

    void initStatusCodeMap(void);
    void setContentType(const std::string &extension);
    void listDirectory(DIR *dir);

    const t_location &getLocation(void) const;
    const t_server &getServer(void) const;

    const std::string &returnError(int status);
    void servFile(const std::string &file_path, int status, int error_status);
    std::string fileToString(const std::string &file_path, int error_status);
    std::string matching(void);
    void defaultPage(void);
    void locationExists(void);
    void methodNotAllowed(void);
    void locationRedirection(void);
    void HandlePostMethod(DIR *dir);
    bool uploadFile(std::string &upload_path);

    bool directoryRedirection();
    std::string getBoundary();
    void checkCgiHeader(void);

        // Attributes
        std::string _version;
    std::string _status_code;
    std::string _status_message;
    std::map<std::string, std::string> _headers;
    std::string _body;

    size_t _upload_file_size;
    std::string _upload_head;
    size_t _upload_pos;
    std::ofstream _upload_of;

    long long _content_length;
    int _location_index;
    std::string _s_response;
    std::string _s_header;
    std::map<int, std::string> _status_codes;
    HTTPRequest *_request;
    std::string _path;
    std::string mime_type;
    bool _is_header_done;
    bool _is_uploaded;
    bool _is_default_page_flag;
    t_server _server;
	CGIHandler *cgi;
};
