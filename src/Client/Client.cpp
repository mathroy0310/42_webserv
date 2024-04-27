#include "Client.hpp"

Client::Client(int socket_fd, t_server server) {
    this->_socket_fd = socket_fd;
    this->_server = server;
    this->_status_code = 0;
    this->_request = NULL;
    this->_is_done_reading = false;
    this->_response = NULL;
}

Client::~Client(void) {
    if (this->_request) {
        //delete this->_request;
    }
    if (this->_response) {
        delete this->_response;
    }
}

int Client::getSocketFd(void) const {
    return (this->_socket_fd);
}

HTTPRequest *Client::createNewRequest(void) {
    return (new HTTPRequest());
}

HTTPRequest *Client::getRequest(void) const {
    return (this->_request);
}

HTTPResponse *Client::getResponse(void) {
    if (!this->_response) {
        if (!this->_status_code)
            this->_response = new HTTPResponse(this->_request, this->_server);
        else
            this->_response = new HTTPResponse(this->_status_code, this->_server);
    }
    return (this->_response);
}

void Client::setRequest(HTTPRequest *request) {
    this->_request = request;
}

void Client::setResponse(HTTPResponse *response) {
    this->_response = response;
}

void Client::setStatus(int status_code) {
    this->_status_code = status_code;
}

void Client::disconnect(void) {
    close(this->_socket_fd);
    this->_socket_fd = -1;
}

void Client::read_socket(void) {
    if (!this->getRequest())
        this->setRequest(this->createNewRequest());

    std::vector<char> data;
    char buffer[BUFFER_SIZE + 1];
    int len = BUFFER_SIZE;

    this->_is_done_reading = false;
    while (len == BUFFER_SIZE) {
        bzero(buffer, BUFFER_SIZE + 1);
		Logger::get().log(INFO, "Reading from socket %d", this->getSocketFd());
        len = recv(this->getSocketFd(), buffer, BUFFER_SIZE, MSG_DONTWAIT);
        if (len > 0) {
            data.insert(data.end(), buffer, buffer + len);
        } else if (len == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				len = BUFFER_SIZE;
				sleep(1);
				continue;
			}
            Logger::get().log(ERROR, "Errno: %s", strerror(errno));
            this->disconnect();
            std::cout << FILE_LINE << std::endl;
            throw std::runtime_error("Client disconnected");
        } else if (len == 0) {
            Logger::get().log(ERROR, "Errno: %s", strerror(errno));
            this->disconnect();
            std::cout << FILE_LINE << std::endl;
			return;
            // throw std::runtime_error("Client disconnected");
        }
		std::cout << "len: " << len << std::endl;
    }
    this->_is_done_reading = true;
    std::string totalData(data.begin(), data.end());


    Logger::get().log(INFO, "Data received: \n%s", totalData.c_str());
	Logger::get().log(INFO, "Data received: %d", totalData.size());
    if (!this->getRequest()->getHeaderEnd()) {
        try {
            this->getRequest()->appendHeader(totalData.c_str(), totalData.size());
        } catch (int status) {
            this->setStatus(status);
        }
    } else {
        this->getRequest()->appendFile(totalData.c_str(), totalData.size());
    }
}

bool Client::write_socket(void) {
    HTTPRequest *request = this->getRequest();
    HTTPResponse *response = this->getResponse();
    bool keep_alive = request->getValueByKey(REQ_CONNECTION).empty()
                          ? true
                          : (request->getValueByKey(REQ_CONNECTION) == "keep-alive");

    std::string buffer_reponse;

    buffer_reponse = response->getRequest() ? response->buildResponse() : response->getResponse();
    if (response->getUploaded() == true) {
        Logger::get().log(DEBUG, "Response sent: %s", buffer_reponse.c_str());
        int len = send(this->getSocketFd(), buffer_reponse.c_str(), buffer_reponse.length(), 0);
        if (len < BUFFER_SIZE) {
            request->clear();
            delete request;
            this->setRequest(NULL);
            delete response;
            this->setResponse(NULL);
            if (len == -1)
                return (false);
            return (keep_alive);
        }
    }
    return (true);
}
