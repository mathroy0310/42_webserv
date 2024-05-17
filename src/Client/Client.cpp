#include "Client.hpp"

Client::Client(int socket_fd, t_server server) {
    this->_socket_fd = socket_fd;
    this->_server = server;
    this->_status_code = 0;
    this->_request = NULL;
    this->_is_done_reading = false;
    this->_response = NULL;
}

Client::~Client(void) {}

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

void Client::setSocketTimeout(int seconds) {
    struct timeval tv;
    tv.tv_sec = seconds;
    tv.tv_usec = 0;
    setsockopt(this->_socket_fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof tv);
}

void Client::disconnect(void) {
    close(this->_socket_fd);
    this->_socket_fd = -1;
    if (this->_request) {
        delete this->_request;
        this->_request = NULL;
    }
    if (this->_response) {
        delete this->_response;
        this->_response = NULL;
    }
}

static std::string decode_chunked(const std::string &chunked_string) {
    std::string decoded_string;
    std::istringstream iss(chunked_string);

    while (true) {
        std::string chunk_size_str;
        std::getline(iss, chunk_size_str, '\r');
        iss.ignore();  // ignore '\n'

        std::istringstream size_stream(chunk_size_str);
        size_stream >> std::hex >> std::ws;
        size_t chunk_size;
        size_stream >> chunk_size;

        if (chunk_size == 0)
            break;

        std::string chunk_data(chunk_size, '\0');
        iss.read(&chunk_data[0], chunk_size);

        iss.ignore(2);  // ignore '\r\n'
        Logger::get().log(DEBUG, "Chunk data: %s", chunk_data.c_str());
        Logger::get().log(DEBUG, "Chunk size: %d", chunk_size);
        decoded_string += chunk_data;
    }
    return decoded_string;
}

void Client::read_socket(void) {
    if (!this->getRequest())
        this->setRequest(this->createNewRequest());

    std::vector<char> data;
    char buffer[BUFFER_SIZE + 1];
    int len = BUFFER_SIZE;

    this->_is_done_reading = false;
    this->setSocketTimeout(10);
    do {
        memset(buffer, 0, BUFFER_SIZE + 1);
        Logger::get().log(INFO, "Reading from socket %d", this->getSocketFd());
        len = recv(this->getSocketFd(), buffer, BUFFER_SIZE, MSG_DONTWAIT);
        if (len > 0) {
            Logger::get().log(INFO, "Read %d bytes", len);
            data.insert(data.end(), buffer, buffer + len);
        } else if (len == -1) {
            Logger::get().log(INFO, "Timeout reached or other error");
            continue;
        } else if (len == 0) {
            Logger::get().log(INFO, "Whole data received");
            std::cout << FILE_LINE << std::endl;
            this->disconnect();
            return;
            // throw std::runtime_error("Client disconnected");
        } else if (len < BUFFER_SIZE) {
            Logger::get().log(INFO, "Received less than BUFFER_SIZE");
            continue;
        }
    } while (len <= BUFFER_SIZE && len > 0);
    this->_is_done_reading = true;
    std::string totalData(data.begin(), data.end());

    Logger::get().log(INFO, "Data received: \n\n%s", totalData.c_str());
    Logger::get().log(DEBUG, "Data received: %d", totalData.size());
    if (!this->getRequest()->getHeaderEnd()) {
        try {
            this->getRequest()->appendHeader(totalData.c_str(), totalData.size());
        } catch (int status) {
            std::cout << FILE_LINE << std::endl;
            this->setStatus(status);
        }
    } else {
        this->getRequest()->appendFile(totalData.c_str(), totalData.size());
    }
    if (this->getRequest()->getValueByKey(REQ_TRANSFER) == "chunked") {
        Logger::get().log(DEBUG, "Decoding chunked data");
        std::string tmp = decode_chunked(this->getRequest()->getBody());
        this->getRequest()->setBody(tmp);
    }

    if (!this->getRequest()->getHeaderEnd()) {
        std::cout << FILE_LINE << std::endl;
        Logger::get().log(DEBUG, "Header not ended");
        this->setStatus(BAD_REQUEST_STATUS);
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
        Logger::get().log(INFO, "Data sent: \n\n%s", buffer_reponse.c_str());
        int len = send(this->getSocketFd(), buffer_reponse.c_str(), buffer_reponse.length(), 0);
        if (len < BUFFER_SIZE) {
            this->disconnect();
            request->clear();
            if (len == -1)
                return (false);
            return (keep_alive);
        }
    }
    return (true);
}
