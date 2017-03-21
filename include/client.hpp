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
        static std::unique_ptr<Client> createInstance(int fd, int flags,const char* ip) {
            return std::unique_ptr<skss::Client>(new skss::Client(fd, flags, ip));
        }
        ~Client();

        void addFlags(int flags);

    private:
        int fd;
        int flags;
        const std::string ip;
        Client(int fd, int flags, const char* ip);
    };
};


#endif //SKSS_CLIENT_HPP
