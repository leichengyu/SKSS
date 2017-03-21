#ifndef SERVER_H_
#define SERVER_H_

#include <mutex>

#include <common.hpp>
#include <event.hpp>
#include <eventloop.hpp>
#include <client.hpp>

namespace skss {
    class Server {
    public:

        static Server* getInstance() {
            static Server instance;
            return &instance;
        }

        void run();
        ~Server();

        void clientInc();
        void clientDec();
        int getClientNumber() const;
        void init(int port);

    private:
        Server():initialized(false){}

        Server(const Server&);
        Server& operator=(const Server&);

        bool initialized;
        int port;
        //TODO: array of TCP fd
        int sfd;
        //TODO: add unix socket here
        int tcp_backlog;
        int num_client;
        std::mutex mtx;

        std::shared_ptr<EventLoop> eventLoop;

        AcceptTCPCallback acceptTCPCallback;

        int create_and_bind(const char *str_port);

        int make_non_blocking(int sfd);
    };

};


#endif