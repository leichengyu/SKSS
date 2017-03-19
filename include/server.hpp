#ifndef SERVER_H_
#define SERVER_H_

#include <common.hpp>
#include <eventloop.hpp>
#include <eventcallback.hpp>

namespace skss {
    class Server {
    public:
        Server(int port);

        void run();

        ~Server();

    private:
        int port;
        int sfd;
        int tcp_backlog;
        EventLoop eventloop;

        AcceptTCPCallback acceptTCPCallback;

        void init();

        int create_and_bind(const char *str_port);

        int make_non_blocking(int sfd);
    };
};


#endif