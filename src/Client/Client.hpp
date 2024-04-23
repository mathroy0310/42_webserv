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
    ~Client(void);  // new stuff here
    void setRequest(HTTPRequest *request);
    void setResponse(HTTPResponse *response);
    void setStatus(int status_code);
    HTTPRequest *createNewRequest(void);
    HTTPRequest *getRequest(void) const;
    HTTPResponse *getResponse(void);
    int getSocketFd(void) const;
    //

    // Methods
    void disconnect(void);
    void read_socket(void);
    bool write_socket(void);

    bool _is_done_reading;
  private:
    // Attributes
    int _socket_fd;
    int _status_code;
    t_server _server;
    HTTPRequest *_request;
    HTTPResponse *_response;
};
