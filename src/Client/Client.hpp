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
    void setSocketTimeout(int seconds);
    HTTPRequest *createNewRequest(void);
    HTTPRequest *getRequest(void) const;
    HTTPResponse *getResponse(void);
    int getSocketFd(void) const;
    bool getIsDoneReading(void) const { return _is_done_reading; }
    //

    // Methods
    void disconnect(void);
    void read_socket(void);
    bool write_socket(void);

  private:
    bool _is_done_reading;
    // Attributes
    int _socket_fd;
    int _status_code;
    t_server _server;
    HTTPRequest *_request;
    HTTPResponse *_response;
};
