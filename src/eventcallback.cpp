//
// Created by zearom32 on 17-3-19.
//

#include <eventloop.hpp>
#include <server.hpp>

void skss::AcceptTCPCallback::operator()(EventLoop *eventloop,
                                         int fd,
                                         int mask,
                                         std::shared_ptr<Client> client) {
    network::acceptTcpHandler(eventloop, fd, mask, client);
}

void skss::ReadQueryFromClientCallback::operator()(EventLoop *eventloop,
                                                   int fd,
                                                   int mask,
                                                   std::shared_ptr<Client> client) {
    ssize_t nread = 0;
    size_t read_len = skss::PROTO_IOBUF_LEN;

    // Redis use sds, we use std::string here
    std::string &buf = client->getBuf();
    size_t buf_len = buf.length();
    buf.resize(buf_len + read_len);
    nread = read(fd, (void *) (buf.data() + buf_len), read_len);
    if (nread == -1) {
        if (errno == EAGAIN) {
            return;
        } else {
            LOG(INFO) << "Reading from client: " << strerror(errno);
            Server::getInstance()->deleteClient(client);
            return;
        }
    } else if (nread == 0) {
        LOG(INFO) << "Client closed connection";
        Server::getInstance()->deleteClient(client);
        return;
    }
    buf.resize(buf_len + nread);
    //TODO: add time here
    if (buf.size() > skss::PROTO_MAX_QUERYBUF_LEN) {
        LOG(WARNING) << "Closing client that reached max query buffer length: " << buf.size();
        Server::getInstance()->deleteClient(client);
        return;
    }

    client->processInputBuffer();
}
