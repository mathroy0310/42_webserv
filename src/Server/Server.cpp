#include "Server.hpp"

// Constructors and destructors

Server::Server(const t_config &config) : _config(config), _multiplexer(new Multiplexer()), _running(false) {}

Server::~Server(void) {
    this->stop();
}

void Server::run(void) {
    Logger::get().log(INFO, "Server starting...");
    try {
        this->_running = true;
        this->setupServerConnections();
        Logger::get().log(INFO, "Server connections setting up...");
        while (this->_running)
            if (this->_multiplexer->wait() > 0) {
                this->acceptConnections();
                this->handleRequests();
            }
    } catch (const std::exception &e) {
        Logger::get().log(ERROR, "Server error: %s", e.what());
        this->stop();
    }
}

void Server::stop(void) {
    if (this->_running) {
        this->_running = false;
        Logger::get().log(INFO, "Server stopping...");

        std::map<int, SocketWrapper>::iterator server = this->_listening_sockets.begin();
        for (; server != this->_listening_sockets.end(); server++) {
            close(server->first);
        }
        std::vector<Client>::iterator client = this->_clients.begin();
        for (; client != this->_clients.end(); client++) {
            client->disconnect();
        }
        delete _multiplexer;
    }
}

void Server::setupServerConnections(void) {
    for (size_t i = 0; i < this->_config.servers.size(); i++) {
        std::string ip_address = this->_config.servers[i].ip_address;
        int port = this->_config.servers[i].port;
        int max_clients = this->_config.servers[i].max_client_size;

        SocketWrapper new_socket(ip_address, port, max_clients);
        new_socket.init();
        this->_multiplexer->addFd(new_socket.getSocketFd(), POLLIN);
        this->_listening_sockets.insert(std::make_pair(new_socket.getSocketFd(), new_socket));
        Logger::get().log(DEBUG, "Server socket set up on address %s and port %d", ip_address.c_str(), port);
    }
    this->_server_count = this->_listening_sockets.size() + 3;
}

void Server::acceptConnections() {
    std::map<int, SocketWrapper>::iterator server = this->_listening_sockets.begin();

    for (int i = 0; server != this->_listening_sockets.end(); server++) {
        if (this->_multiplexer->canRead(server->first)) {
            int new_client = server->second.acceptSocket();
            if (new_client == -1 || new_client == SERVICE_UNAVAILABLE_STATUS) {
                continue;
            }
            Logger::get().log(DEBUG, "New client connection [%d] on server on port %d",
                              (new_client - this->_server_count), server->second.getPort());
            this->_multiplexer->addFd(new_client, POLLIN);
            this->_clients.push_back(Client(new_client, this->_config.servers[i]));
        }
        i++;
    }
}

void Server::handleRequests(void) {
    std::vector<Client>::iterator client = this->_clients.begin();
    while (client != this->_clients.end()) {
        std::system("sleep 0.05");
        if (this->_multiplexer->canRead(client->getSocketFd())) {
            try {
                read_socket(*client);
                this->_multiplexer->addFd(client->getSocketFd(), POLLOUT);
            } catch (const std::exception &e) {
               throw std::runtime_error("Client disconnected");
            }
        }
        if (this->_multiplexer->canWrite(client->getSocketFd())) {
            try {
                if (write_socket(*client)) {
                    this->_multiplexer->removeFd(client->getSocketFd());
                    client->disconnect();
                    Logger::get().log(DEBUG, "Disconnecting client [%d]", client->getSocketFd() + 2);
                    client = this->_clients.erase(client);
                    continue;
                }
            } catch (const std::exception &ex) {
                this->_multiplexer->removeFd(client->getSocketFd());
                client->disconnect();
                client = this->_clients.erase(client);
                continue;
            }
        }
        client++;
    }
}

void Server::read_socket(Client &client) {
    if (!client.getRequest())
        client.setRequest(client.createNewRequest());

    std::vector<char> data;
    char buffer[BUFFER_SIZE + 1];
    int len = BUFFER_SIZE;

    while (len == BUFFER_SIZE) {
        bzero(buffer, BUFFER_SIZE + 1);
        len = recv(client.getSocketFd(), buffer, BUFFER_SIZE, 0);
        if (len > 0) {
            data.insert(data.end(), buffer, buffer + len);
        } else if (len == -1) {
            client.disconnect();
        } else if (len == 0) {
            throw std::runtime_error("Client disconnected");
        }
    }

    std::string totalData(data.begin(), data.end());

	Logger::get().log(INFO, "Data received: \n%s", totalData.c_str());
    if (!client.getRequest()->getHeaderEnd()) {
        try {
            client.getRequest()->appendHeader(totalData.c_str(), totalData.size());
        } catch (int status) {
            client.setStatus(status);
        }
    } else {
        client.getRequest()->appendFile(totalData.c_str(), totalData.size());
    }
    Logger::get().log(DEBUG, "REQ_METHOD:=%s", client.getRequest()->getHeaders()[REQ_METHOD].c_str());
    Logger::get().log(DEBUG, "REQ_PATH:=%s", client.getRequest()->getHeaders()[REQ_PATH].c_str());
    Logger::get().log(DEBUG, "REQ_CONTENT_LENGTH:=%s", client.getRequest()->getHeaders()[REQ_CONTENT_LENGTH].c_str());
    Logger::get().log(DEBUG, "REQ_CONNECTION:=%s", client.getRequest()->getHeaders()[REQ_CONNECTION].c_str());
}

bool Server::write_socket(Client &client) {
    HTTPRequest *request = client.getRequest();
    HTTPResponse *response = client.getResponse();
    bool keep_alive = request->getValueByKey(REQ_CONNECTION).empty()
                          ? true
                          : (request->getValueByKey(REQ_CONNECTION) == "keep-alive");

    std::string buffer_reponse;

    buffer_reponse = response->getRequest() ? response->buildResponse() : response->getResponse();
    if (response->getUploaded() == true) {
		Logger::get().log(DEBUG, "Response sent: %s", buffer_reponse.c_str());
        int len = send(client.getSocketFd(), buffer_reponse.c_str(), buffer_reponse.length(), 0);
        if (len < BUFFER_SIZE) {
            request->clear();
            delete request;
            client.setRequest(NULL);
            delete response;
            client.setResponse(NULL);
            if (len == -1)
                return (false);
            return (keep_alive);
        }
    }
    return (true);
}
