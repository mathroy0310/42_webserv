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
		Logger::get().log(DEBUG, "Max client size, %d", max_clients);
        SocketWrapper new_socket(ip_address, port, max_clients);
        new_socket.init();
        this->_multiplexer->addFd(new_socket.getSocketFd(), POLLIN);
        this->_listening_sockets.insert(std::make_pair(new_socket.getSocketFd(), new_socket));
        Logger::get().log(DEBUG, "Server socket set up on address %s and port %d", ip_address.c_str(), port);
    }
}

void Server::acceptConnections() {
    std::map<int, SocketWrapper>::iterator server = this->_listening_sockets.begin();

    for (int i = 0; server != this->_listening_sockets.end(); server++) {
        if (this->_multiplexer->canRead(server->first)) {
            int new_client = server->second.acceptSocket();
            if (new_client == -1 || new_client == std::stoi(SERVICE_UNAVAILABLE_STATUS)) {
                continue;
            }
            Logger::get().log(DEBUG, "New client connection [%d] on server on port %d", new_client, server->second.getPort());
            this->_multiplexer->addFd(new_client, POLLIN);
            this->_clients.push_back(Client(new_client, this->_config.servers[i]));
        }
		i++;
    }
}

void Server::handleRequests(void) {
    std::vector<Client>::iterator client = this->_clients.begin();
    while (client != this->_clients.end()) {
        if (this->_multiplexer->canRead(client->getSocketFd())) {
            client->readRequest();
            if (client->hasPendingOperations()) {
                this->_multiplexer->addFd(client->getSocketFd(), POLLOUT);
            }
        }
        if (this->_multiplexer->canWrite(client->getSocketFd())) {
            client->writeResponse();
            if (!client->hasPendingOperations()) {
                this->_multiplexer->removeFd(client->getSocketFd());
                Logger::get().log(DEBUG, "Disconnecting client [%d]", client->getSocketFd() - 4);
                client->disconnect();
            }
        }
        client++;
    }
}
