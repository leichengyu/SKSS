//
// Created by zearom32 on 17-3-20.
//

#include <client.hpp>
#include <server.hpp>

skss::Client::Client(int fd, int flags, const char *ip) : fd(fd), flags(flags), ip(ip) {
    skss::Server::getInstance()->clientInc();
}

skss::Client::~Client() {
    skss::Server::getInstance()->clientDec();
}

void skss::Client::addFlags(int flags) {
    this->flags |= flags;
}

void skss::Client::processInputBuffer() {
    LOG(WARNING) << this->query_buf;
}
