#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/epoll.h>
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

    struct epoll_event event;
    struct epoll_event *events;

    int efd = epoll_create1(0);

    event.data.fd = this->sfd;
    event.events = EPOLLIN | EPOLLET;
    int s = epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event);
    if (s == -1) {
        std::cerr << "Error when register an epoll event" << std::endl;
        exit(-1);
    }

    events = new epoll_event[10];
    while (1) {
        int n = epoll_wait(efd, events, 10, -1);
        for (int i = 0; i < n; i++) {
            if ((events[i].events & EPOLLERR) ||
                (events[i].events & EPOLLHUP) ||
                !(events[i].events & EPOLLIN)) {
                // error occured
                std::cerr << "epoll error" << std::endl;
                close(events[i].data.fd);
                continue;
            } else if (this->sfd == events[i].data.fd) {
                //new clients
                while (1) {
                    struct sockaddr in_addr;
                    socklen_t in_len;
                    int infd;
                    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
                    in_len = sizeof(in_addr);
                    infd = accept(sfd, &in_addr, &in_len);
                    if (infd == -1) {
                        if ((errno == EAGAIN) ||
                            (errno == EWOULDBLOCK)) {
                            break;
                        } else {
                            std::cerr << "accept error" << std::endl;
                            break;
                        }
                    }

                    s = getnameinfo(&in_addr, in_len,
                                    hbuf, sizeof(hbuf),
                                    sbuf, sizeof(sbuf),
                                    NI_NUMERICHOST | NI_NUMERICSERV);

                    if (s == 0) {
                        std::cout << "Accepted connection on descriptor " << infd << " host=" << hbuf << " port="
                                  << sbuf << std::endl;
                    }

                    s = make_non_blocking(infd);
                    if (s != 0) {
                        std::cerr << "make non blocking error" << std::endl;
                        break;
                    }
                    event.data.fd = infd;
                    event.events = EPOLLIN | EPOLLET;
                    s = epoll_ctl(efd, EPOLL_CTL_ADD, infd, &event);
                    if (s == -1) {
                        std::cerr << "register new client error" << std::endl;
                    }
                }
                continue;
            } else {
                int done = 0;
                while (1) {
                    ssize_t count;
                    char buf[512];

                    count = read(events[i].data.fd, buf, sizeof(buf));
                    if (count == -1) {
                        if (errno != EAGAIN) {
                            done = 1;
                        }
                        break;
                    } else if (count == 0) {
                        done = 1;
                        break;
                    }
                    s = write(1, buf, count);
                }
                if (done) {
                    std::cout << "close connecton with " << events[i].data.fd << std::endl;
                    close(events[i].data.fd);
                }
            }

        }
    }
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