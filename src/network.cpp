//
// Created by zearom32 on 17-3-19.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include <network.hpp>

void skss::network::acceptTcpHandler(EventLoop *eventloop, int fd, int mask, void *data) {
    int max = skss::MAX_ACCEPTS_PER_CALL;
    int cfd, cport;
    char cip[skss::NET_IP_STR_LEN];
    while (max--) {
        cfd = tcpAccept(nullptr, fd, cip, sizeof(cip), &cport);
        if (cfd == -1) {
            if (errno != EWOULDBLOCK) {
                LOG(ERROR) << "Accepting client connection: " << cfd << " error: " << strerror(errno);
                return;
            }
        }
        LOG(INFO) << "Accepted client, handle commandHandler " << cfd;
        acceptCommandHandler(cfd, 0, cip);
    }
}

int skss::network::tcpAccept(char *err, int s, char *ip, size_t ip_len, int *port) {
    int fd;
    struct sockaddr_storage sa;
    socklen_t salen = sizeof(sa);
    if ((fd = genericAccept(err, s, (struct sockaddr *) &sa, &salen)) == -1) {
        return -1;
    }
    if (sa.ss_family == AF_INET) {
        //ipv4
        struct sockaddr_in *s = (struct sockaddr_in *) &sa;
        if (ip) inet_ntop(AF_INET, (void *) &(s->sin_addr), ip, ip_len);
        if (port) *port = ntohs(s->sin_port);
    } else {
        //ipv6
        struct sockaddr_in6 *s = (struct sockaddr_in6 *) &sa;
        if (ip) inet_ntop(AF_INET6, (void *) &(s->sin6_addr), ip, ip_len);
        if (port) *port = ntohs(s->sin6_port);
    }
    return fd;
}

int skss::network::genericAccept(char *err, int s, struct sockaddr *sa, socklen_t *len) {
    int fd;
    while (1) {
        fd = accept(s, sa, len);
        if (fd == -1) {
            if (errno == EINTR) {
                continue;
            } else {
                return -1;
            }
        }
        break;
    }
    return fd;
}

int skss::network::acceptCommandHandler(int fd, int flags, char *ip) {
    //TODO:
    auto client = skss::Client::createInstance(fd, flags, ip);
    if (!client) {
        LOG(ERROR) << "create client error, fd = " << fd <<" error message: " << strerror(errno);
        return -1;
    }

    //TODO: if set maxclient
    //TODO: if set passwd
    client->addFlags(flags);
    return 0;
}
