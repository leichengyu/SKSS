#include <cstring>
#include <sys/socket.h>

#include <server.hpp>

skss::Server::~Server() {
}

void skss::Server::run() {
    LOG(INFO) << "Simple Key-Value Store Service run on port " << this->port;
    this->eventLoop->main();
}

void skss::Server::init(int port) {
    //make sure only initialized once
    this->mtx.lock();
    if (!this->initialized) {

        this->initialized = true;
        this->port = port;
        this->tcp_backlog = skss::CONFIG_DEFAULT_TCP_BACKLOG;
        this->eventLoop = std::make_shared<skss::EventLoop>(skss::CONFIG_DEFAULT_SETSIZE);

        // now we have the port
        LOG(INFO) << "Start init server";
        // 1. create and bind
        this->sfd = skss::net::createAndBind(std::to_string(this->port).c_str());

        // 2. make socket non blocking
        if (skss::net::setNonBlock(this->sfd, 1) != skss::SKSS_OK) {
            LOG(ERROR) << "Error when makeing non blocking";
            exit(1);
        }

        // 3. listen
        if (listen(this->sfd, this->tcp_backlog) != 0) {
            LOG(ERROR) << "Error when listen to sfd";
            exit(1);
        }

        // now we listen to the port, then we poll from it
        // 4. create an event, we listen to sfd, accept clients and have new events for each client
        // We do not have client data now
        // seems do not use EPOLLET why?
        this->eventLoop->createFileEvent(this->sfd, skss::READABLE, &skss::acceptTCPCallback, nullptr);
    }
    this->clients.resize(skss::CONFIG_DEFAULT_SETSIZE);
    this->eventLoop->addBeforeSleepCallback(std::bind(&Server::beforeSleep, this));
    this->num_client = 0;
    this->total_client = 0;
    mtx.unlock();
}

void skss::Server::clientDec() { this->num_client--; }

void skss::Server::clientInc() {
    this->num_client++;
    this->total_client++;
}

int skss::Server::getClientNumber() const { return num_client; }

void skss::Server::addClient(shared_ptr<Client> client) {
    // using std::move to reduce the copy operation(+1 then -1 on reference count)
    int fd = client->getFd();
    if (fd < 0 || fd >= skss::CONFIG_DEFAULT_MAX_CLIENTS) return;
    this->clients[fd].swap(client);
}

std::shared_ptr<skss::EventLoop> skss::Server::getEventLoop() {
    return this->eventLoop;
}

void skss::Server::deleteClient(shared_ptr<Client> client) {
    //TODO: remove the callback?
    int fd = client->getFd();
    if (fd < 0 || fd >= skss::CONFIG_DEFAULT_MAX_CLIENTS) return;
    DLOG(INFO) <<"client reference: " << this->clients[fd].use_count();
    this->clients[fd] = nullptr;
    this->eventLoop->deleteFileEvent(fd, skss::READABLE | skss::WRITABLE);
    // Don't forget to close the fd
    close(fd);
}

void skss::Server::beforeSleep() {
   //TODO:
    DLOG(INFO) << "clients: " << this->getClientNumber() << " total clients: " << this->total_client;
}