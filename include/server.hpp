#ifndef SERVER_H_
#define SERVER_H_

#include <list>
#include <mutex>
#include <memory>

#include <common.hpp>
#include <event.hpp>
#include <eventloop.hpp>
#include <client.hpp>
#include <net.hpp>

namespace skss {
    using std::vector;
    using std::shared_ptr;

    class Server {
    public:

        static Server *getInstance() {
            static Server instance;
            return &instance;
        }

        void run();

        ~Server();

        void clientInc();

        void clientDec();

        int getClientNumber() const;

        void init(int port);

        void addClient(shared_ptr<Client> client);

        void deleteClient(shared_ptr<Client> client);

        shared_ptr<EventLoop> getEventLoop();

    private:
        Server() : initialized(false) {}

        Server(const Server &);

        Server &operator=(const Server &);

        std::shared_ptr<EventLoop> eventLoop;

        bool initialized;
        int port;
        //TODO: array of TCP fd
        int sfd;
        //TODO: add unix socket here
        int tcp_backlog;
        int num_client;
        std::mutex mtx;
        vector<shared_ptr<Client>> clients;
    };

};


#endif