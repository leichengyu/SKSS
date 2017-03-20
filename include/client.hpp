//
// Created by zearom32 on 17-3-20.
//

#ifndef SKSS_CLIENT_HPP
#define SKSS_CLIENT_HPP

#include <memory>

namespace skss {

    class Client {
    public:
        static std::unique_ptr<Client> createInstance(int fd, int flags,const char* ip) {
            return std::unique_ptr<Client>(new Client(fd, flags, ip));
        }

    private:
        Client(int fd, int flags, const char* ip);
    };
};


#endif //SKSS_CLIENT_HPP
