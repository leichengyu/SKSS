//
// Created by zearom32 on 17-3-19.
//

#ifndef SKSS_EVENTCALLBACK_HPP_H
#define SKSS_EVENTCALLBACK_HPP_H

#include <network.hpp>

namespace skss {

    class EventLoop;

    class EventCallback {
    public:
        virtual void operator()(EventLoop *eventloop, int fd, int mask, void *data) = 0;
    };

    class AcceptTCPCallback : public EventCallback {
    public:
        virtual void operator()(EventLoop *eventloop, int fd, int mask, void *data);
    };
};
#endif //SKSS_EVENTCALLBACK_HPP_H
