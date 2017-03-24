#ifndef EVENTLOOP_H_
#define EVENTLOOP_H_

#include <functional>
#include <sys/epoll.h>
#include <vector>

#include <event.hpp>
#include <eventcallback.hpp>

namespace skss {

    class EventLoop {
    public:
        EventLoop(int setsize);

        ~EventLoop();

        void main();

        int createFileEvent(int fd, int mask, EventCallback *callback, std::shared_ptr<Client> client);

        void deleteFileEvent(int fd, int delmask);

        int craateTimeEvent();

        int getSetsize() const { return setsize; }

        void stopLoop() { this->stop = true; }

        int Poll();

        int Wait(int fd, int mask, long long milliseconds);

        void addBeforeSleepCallback(std::function<void()> f);

    private:
        int setsize;
        int efd;
        bool stop;
        int maxfd;
        std::vector<epoll_event> epoll_events;
        std::vector<FileEvent> file_events;
        std::vector<FiredEvent> fired_events;
        std::vector<std::function<void()>> beforeCallbacks;

        void beforeSleep();

        int processEvents();
    };
};
#endif