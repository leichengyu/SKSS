//
// Created by zearom32 on 17-3-20.
//

#include <client.hpp>
#include <server.hpp>

skss::Client::Client(int fd, int flags, const char *ip) : fd(fd), flags(flags), ip(ip), req_type(0){
    skss::Server::getInstance()->clientInc();
}

skss::Client::~Client() {
    skss::Server::getInstance()->clientDec();
}

void skss::Client::addFlags(int flags) {
    this->flags |= flags;
}

void skss::Client::processInputBuffer() {
    //TODO: add
    while (!this->query_buf.empty()) {
        //TODO: client is paused
        if (this->flags & CLIENT_BLOCKED) break;

        if (this->flags & (CLIENT_CLOSE_AFTER_REPLY|CLIENT_CLOSE_ASAP)) break;

        if (!this->req_type) {
            if (this->query_buf[0] == '*') {
                this->req_type = PROTO_REQ_MULTIBULK;
            } else {
                this->req_type = PROTO_REQ_INLINE;
            }
        }

        if (this->req_type == PROTO_REQ_INLINE) {
            if (this->processInlineBuffer() != SKSS_OK) break;
        } else if (this->req_type == PROTO_REQ_MULTIBULK) {
            if (this->processMultibulkBuffer() != SKSS_OK) break;
        } else {
            LOG(ERROR) << "Unknown request type";
            //TODO: should exit
            //exit(1);
            break;
        }
    }
}

int skss::Client::processInlineBuffer() {
    size_t newline = this->query_buf.find('\n');
    if (newline == std::string::npos) {
        if (this->query_buf.size() > PROTO_INLINE_MAX_SIZE) {
            //REPLY ERROR
        }
        return SKSS_ERR;
    }

    if (newline != 0 && this->query_buf[newline-1]=='\r') {
        newline--;
    }
    size_t query_len = newline;
    std::string aux = this->query_buf.substr(0, query_len);
    //TODO: get args, return SKSS_ERR if parsing error

    this->query_buf = this->query_buf.substr(newline+2);

    return SKSS_OK;
}

int skss::Client::processMultibulkBuffer() {
    //TODO:
    return SKSS_OK;
}


