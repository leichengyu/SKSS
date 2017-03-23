
#include <event.hpp>

skss::Event::Event() : eventloop(nullptr),
                       read_callback(nullptr),
                       write_callback(nullptr) {
}

skss::Event::~Event() {
}

void skss::Event::callRead(int fd, int mask, std::shared_ptr<Client> client) {
    if (this->read_callback) {
        this->read_callback->operator()(this->eventloop, fd, mask, client);
    }
}

void skss::Event::callWrite(int fd, int mask, std::shared_ptr<Client> client) {
    if (this->write_callback) {
        this->write_callback->operator()(this->eventloop, fd, mask, client);
    }
}

skss::FileEvent::FileEvent() {}

skss::FileEvent::~FileEvent() {}

skss::FiredEvent::FiredEvent() {}

skss::FiredEvent::~FiredEvent() {}
