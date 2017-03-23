//
// Created by zearom32 on 17-3-20.
//

#ifndef SKSS_CLIENT_HPP
#define SKSS_CLIENT_HPP

#include <memory>
#include <string>

namespace skss {

    class Client {
    public:
        static std::shared_ptr<Client> createInstance(int fd, int flags, const char *ip) {
            return std::shared_ptr<skss::Client>(new skss::Client(fd, flags, ip));
        }

        ~Client();

        void addFlags(int flags);

        int getFd() const { return fd; }

        inline std::string &getBuf() { return this->query_buf; }

        void processInputBuffer();

    private:
        int fd;
        int flags;
        const std::string ip;
        std::string query_buf;

        Client(int fd, int flags, const char *ip);
    };
};


#endif //SKSS_CLIENT_HPP
