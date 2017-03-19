#ifndef EVENTLOOP_H_
#define EVENTLOOP_H_

#include <sys/epoll.h>
#include <vector>

#include <event.hpp>
#include <eventcallback.hpp>

namespace skss {

    class FiredEvent;

    class EventLoop {
    public:
        EventLoop(int setsize);

        ~EventLoop();

        void main();

        int createFileEvent(int fd, int mask, EventCallback *callback, void *clientData);

        void deleteFileEvent(int fd, int delmask);

        int craateTimeEvent();

        int getSetsize() const { return setsize; }

        void stopLoop() { this->stop = true; }

        int Poll();

        int Wait(int fd, int mask, long long milliseconds);

    private:
        int setsize;
        int efd;
        bool stop;
        int maxfd;
        std::vector<epoll_event> epoll_events;
        std::vector<FileEvent> file_events;
        std::vector<FiredEvent> fired_events;

        void beforeSleep();

        int processEvents();
    };
};
#endif