//
// Created by zearom32 on 17-3-19.
//

#include <eventloop.hpp>

void skss::AcceptTCPCallback::operator()(EventLoop *eventloop, int fd, int mask, void *data) {
    network::acceptTcpHandler(eventloop, fd, mask, data);
}
