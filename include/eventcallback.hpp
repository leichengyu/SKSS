//
// Created by zearom32 on 17-3-19.
//

#ifndef SKSS_EVENTCALLBACK_HPP_H
#define SKSS_EVENTCALLBACK_HPP_H

#include <memory>

#include <network.hpp>

namespace skss {
    using std::shared_ptr;

    class EventLoop;

    class EventCallback {
    public:
        virtual void operator()(EventLoop *eventloop, int fd, int mask, shared_ptr<Client> data) = 0;
    };

    class AcceptTCPCallback : public EventCallback {
    public:
        virtual void operator()(EventLoop *eventloop, int fd, int mask, shared_ptr<Client> data);
    };

    class ReadQueryFromClientCallback : public EventCallback {
        virtual void operator()(EventLoop *eventloop, int fd, int mask, shared_ptr<Client> data);
    };

    static AcceptTCPCallback acceptTCPCallback;
    static ReadQueryFromClientCallback readQueryFromClientCallback;
};
#endif //SKSS_EVENTCALLBACK_HPP_H
