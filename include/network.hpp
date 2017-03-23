//
// Created by zearom32 on 17-3-19.
//

#ifndef SKSS_NETWORK_HPP
#define SKSS_NETWORK_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <memory>

#include <common.hpp>
#include <client.hpp>


namespace skss {

    class EventLoop;

    namespace network {

        void acceptTcpHandler(skss::EventLoop *eventloop, int fd, int mask, std::shared_ptr<Client> client);

        int tcpAccept(char *err, int s, char *ip, size_t ip_len, int *port);

        int genericAccept(char *err, int s, struct sockaddr *sa, socklen_t *len);

        int acceptCommandHandler(int fd, int flags, char *ip);

    };
};

#endif //SKSS_NETWORK_HPP
