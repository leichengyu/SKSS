#ifndef EVENT_H_
#define EVENT_H_

#include <memory>

#include <eventcallback.hpp>

namespace skss {

    class EventLoop;

    class Event {
    public:
        Event();

        virtual ~Event();

        void setEventLoop(EventLoop *eventloop) { eventloop = eventloop; }

        void setReadCallback(EventCallback *callback) { this->read_callback = callback; }

        void setWriteCallback(EventCallback *callback) { this->write_callback = callback; }

        EventCallback *getReadCallback() const { return this->read_callback; }

        EventCallback *getWriteCallback() const { return this->write_callback; }

        virtual void callRead(int fd, int mask, shared_ptr<Client> client);

        virtual void callWrite(int fd, int mask, shared_ptr<Client> client);

    protected:
        EventLoop *eventloop;
        EventCallback *read_callback;
        EventCallback *write_callback;
    };

    class FileEvent : public Event {
    public:
        FileEvent();

        ~FileEvent();

        inline int getMask() const { return this->mask; }

        inline void setMask(int mask) { this->mask = mask; }

        inline void setClient(shared_ptr<Client> client) { this->client.swap(client); }

        const shared_ptr<Client> &getClient() { return client; }

    private:
        int mask;
        shared_ptr<Client> client;
    };

    class FiredEvent : public Event {
    public:
        FiredEvent();

        ~FiredEvent();

        inline int getMask() const { return this->mask; }

        inline void setMask(int mask) { this->mask = mask; }

        inline int getFd() const { return this->fd; }

        inline void setFd(int fd) { this->fd = fd; }

    private:
        int fd;
        int mask;
    };
};
#endif