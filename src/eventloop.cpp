#include <unistd.h>
#include <poll.h>
#include <memory.h>

#include <eventloop.hpp>

skss::EventLoop::EventLoop(int setsize) : setsize(setsize),
                                          maxfd(-1),
                                          efd(0),
                                          stop(false),
                                          epoll_events(setsize),
                                          file_events(setsize),
                                          fired_events(setsize) {
    this->efd = epoll_create(1024); // 1024 is just a hint
}

skss::EventLoop::~EventLoop() {
    if (efd) {
        close(efd);
    }
}

void skss::EventLoop::main() {
    this->stop = false;
    while (!this->stop) {
        this->beforeSleep();
        this->processEvents();
    }
}

void skss::EventLoop::beforeSleep() {}

void skss::EventLoop::processEvents() {
    int processed = 0;
    int num_events = this->Poll();
    for (int j = 0; j < num_events; j++) {
        FileEvent &fe = this->file_events[this->fired_events[j].getFd()];
    }
}


int skss::EventLoop::createFileEvent(int fd, int mask, void *clientData) {
    if (fd >= this->setsize) {
        return -1;
    }

    FileEvent &event = this->file_events[fd];

    // if have mask, modify else add
    int op = event.getMask() ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;
    struct epoll_event ee = {0};
    ee.events = 0;
    mask |= event.getMask();
    if (mask & skss::READABLE) ee.events |= EPOLLIN;
    if (mask & skss::WRITABLE) ee.events |= EPOLLOUT;
    ee.data.fd = fd;

    if (epoll_ctl(this->efd, op, fd, &ee) == -1) return -1;
    //set mask only if successfully epoll_ctl
    event.setMask(mask);
    event.setEventLoop(this);
    //TODO: proc
    this->maxfd = std::max(this->maxfd, fd);
    return 0;
}

// delete a event, can delete only one mask or delete the event entirely
void skss::EventLoop::deleteFileEvent(int fd, int delmask) {
    if (fd >= this->setsize) return;
    FileEvent &event = this->file_events[fd];
    int mask = event.getMask();
    if (!mask) return;

    struct epoll_event ee = {0};
    mask = mask & (~delmask);
    ee.events = 0;
    if (mask & skss::READABLE) ee.events |= EPOLLIN;
    if (mask & skss::WRITABLE) ee.events |= EPOLLOUT;
    ee.data.fd = fd;

    int op = mask ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;
    epoll_ctl(this->efd, op, fd, &ee);

    event.setMask(mask);
    if (fd == this->maxfd && !mask) {
        int j = fd;
        for (j = this->maxfd - 1; j >= 0; j--) {
            if (this->file_events[j].getMask()) break;
        }
        this->maxfd = j;
    }
}

int skss::EventLoop::Poll() {
    int retval = epoll_wait(this->efd, &this->epoll_events[0], this->setsize, -1);
    if (retval > 0) {
        for (int j = 0; j < retval; j++) {
            int mask = 0;
            struct epoll_event &e = this->epoll_events[j];
            if (e.events & EPOLLIN) mask |= skss::READABLE;
            if (e.events & EPOLLOUT) mask |= skss::WRITABLE;
            if (e.events & EPOLLERR) mask |= skss::WRITABLE;
            if (e.events & EPOLLHUP) mask |= skss::WRITABLE;
            this->fired_events[j].setFd(e.data.fd);
            this->fired_events[j].setMask(mask);
        }
    }
    return retval;
}

int skss::EventLoop::Wait(int fd, int mask, long long milliseconds) {
    struct pollfd pfd;
    int retmask = 0, retval;

    memset(&pfd, 0, sizeof(pfd));
    pfd.fd = fd;
    if (mask & skss::READABLE) pfd.events |= POLLIN;
    if (mask & skss::WRITABLE) pfd.events |= POLLOUT;

    if ((retval = poll(&pfd, 1, milliseconds)) == 1) {
        if (pfd.revents & POLLIN) retmask |= skss::READABLE;
        if (pfd.revents & POLLOUT) retmask |= skss::WRITABLE;
        if (pfd.revents & POLLERR) retmask |= skss::WRITABLE;
        if (pfd.revents & POLLHUP) retmask |= skss::WRITABLE;
        return retmask;
    } else {
        return retval;
    }
}
