#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

#include <server.hpp>

skss::Server::Server(int port) : port(port), eventloop(skss::CONFIG_DEFAULT_SETSIZE) {
    this->tcp_backlog = skss::CONFIG_DEFAULT_TCP_BACKLOG;
}

skss::Server::~Server() {
}

void skss::Server::run() {
    this->init();
    std::cout << "Simple Key-Value Store Service run on port " << port << std::endl;
    this->eventloop.main();
}

void skss::Server::init() {
    // now we have the port
    // 1. create and bind
    this->sfd = create_and_bind(std::to_string(this->port).c_str());

    // 2. make socket non blocking
    if (make_non_blocking(this->sfd) != 0) {
        std::cerr << "Error when makeing non blocking" << std::endl;
        exit(-1);
    }

    // 3. listen
    if (listen(this->sfd, this->tcp_backlog) != 0) {
        std::cerr << "Error when listen to sfd" << std::endl;
        exit(-1);
    }

    // now we listen to the port, then we poll from it
    // 4. create an event, we listen to sfd, accept clients and have new events for each client
    // We do not have client data now
    // seems do not use EPOLLET why?
    this->eventloop.createFileEvent(this->sfd, skss::READABLE, &this->acceptTCPCallback, nullptr);
}

int skss::Server::create_and_bind(const char *str_port) {
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    int s = getaddrinfo(NULL, str_port, &hints, &result);
    if (s) {
        std::cerr << "getaddrinfo error" << std::endl;
        exit(-1);
    }
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) {
            continue;
        }
        s = bind(sfd, rp->ai_addr, rp->ai_addrlen);
        if (s == 0) {
            break;
        }
        close(sfd);
    }
    if (rp == NULL) {
        std::cerr << "Cannot bind to port " << str_port << std::endl;
        exit(-1);
    }
    freeaddrinfo(result);
    return sfd;
}

int skss::Server::make_non_blocking(int sfd) {
    int flags, s;
    flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }
    flags |= O_NONBLOCK;
    s = fcntl(sfd, F_SETFL, flags);
    if (s == -1) {
        return -1;
    }
    return 0;
}