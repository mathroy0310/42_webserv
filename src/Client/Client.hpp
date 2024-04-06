#pragma once

#include "Request/HTTPRequest.hpp"
#include "Response/HTTPResponse.hpp"
#include "defines.h"
#include "parsing.hpp"

class HTTPRequest;
class HTTPResponse;

class Client {
  public:
    // Constructors & Destructors
    Client(int socket_fd, t_server server);
    // new stuff here
    void setRequest(HTTPRequest *request);
    void setResponse(HTTPResponse *response);
    void setStatus(int status_code);
    HTTPRequest *createNewRequest(void);
    HTTPRequest *getRequest(void) const;
    HTTPResponse *getResponse(void);
    int getSocketFd(void) const;
    //

    // Methods
    void readRequest(void);
    bool writeResponse(void);
    bool hasPendingOperations(void) const;
    void disconnect(void);

  private:
    // Attributes
    int _socket_fd;
    int _status_code;
    t_server _server;
    bool _is_writing;
    HTTPRequest *_request;
    HTTPResponse *_response;

    // Methods
    void processRequest(void);
    std::string getFilePath(void);
    void serveFile(const std::string &file_path, std::map<std::string, std::string> &headers);
    std::string getMimeType(const std::string &file_path);
};
