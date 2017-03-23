//
// Created by zearom32 on 17-3-23.
//

#include <net.hpp>
#include <fcntl.h>
#include <cstring>
#include <netdb.h>

int skss::net::setNonBlock(int fd, int non_block) {
    int flags, s;
    flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        LOG(ERROR) << "fcntl(F_GETFL): " << strerror(errno);
        return SKSS_ERR;
    }
    if (non_block) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }
    s = fcntl(fd, F_SETFL, flags);
    if (s == -1) {
        LOG(ERROR) << "fcntl(F_SETFL): " << strerror(errno);
        return SKSS_ERR;
    }
    return SKSS_OK;
}

int skss::net::createAndBind(const char *str_port) {
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    int s = getaddrinfo(NULL, str_port, &hints, &result);
    if (s) {
        LOG(ERROR) << "getaddrinfo: " << strerror(errno);
        exit(1);
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
        LOG(ERROR) << "Cannot bind to port " << str_port;
        exit(1);
    }
    freeaddrinfo(result);
    return sfd;
}

