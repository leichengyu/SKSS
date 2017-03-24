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
        enum FLAG {
            CLIENT_SLAVE = 1 << 0,
            CLIENT_MASTER = 1 << 1,
            CLIENT_MONITOR = 1 << 2,
            CLIENT_MULTI = 1 << 3,
            CLIENT_BLOCKED = 1 << 4,
            CLIENT_DIRTY_CAS = 1 << 5,
            CLIENT_CLOSE_AFTER_REPLY = 1 << 6,
            CLIENT_UNBLOCKED = 1 << 7,
            CLIENT_LUA = 1 << 8,
            CLIENT_ASKING = 1 << 9,
            CLIENT_CLOSE_ASAP = 1 << 10,
            CLIENT_UNIX_SOCKET = 1 << 11,
            CLIENT_DIRTY_EXEC = 1 << 12,
            CLIENT_MASTER_FORCE_REPLY = 1 << 13,  /* Queue replies even if is master */
            CLIENT_FORCE_AOF = 1 << 14, /* Force AOF propagation of current cmd. */
            CLIENT_FORCE_REPL = 1 << 15,  /* Force replication of current cmd. */
            CLIENT_PRE_PSYNC = 1 << 16,   /* Instance don't understand PSYNC. */
            CLIENT_READONLY = 1 << 17,    /* Cluster client is in read-only state. */
            CLIENT_PUBSUB = 1 << 18,      /* Client is in Pub/Sub mode. */
            CLIENT_PREVENT_AOF_PROP = 1 << 19,  /* Don't propagate to AOF. */
            CLIENT_PREVENT_REPL_PROP = 1 << 20,  /* Don't propagate to slaves. */
            CLIENT_PREVENT_PROP = CLIENT_PREVENT_AOF_PROP | CLIENT_PREVENT_REPL_PROP,
            CLIENT_PENDING_WRITE = 1 << 21, /* Client has output to send but a write
                                     handler is yet not installed. */
            CLIENT_REPLY_OFF = 1 << 22,   /* Don't send replies to client. */
            CLIENT_REPLY_SKIP_NEXT = 1 << 23,  /* Set CLIENT_REPLY_SKIP for next cmd */
            CLIENT_REPLY_SKIP = 1 << 24,  /* Don't send just this reply. */
            CLIENT_LUA_DEBUG = 1 << 25,  /* Run EVAL in debug mode. */
            CLIENT_LUA_DEBUG_SYNC = 1 << 26,  /* EVAL debugging without fork() */
            CLIENT_MODULE = 1 << 27 /* Non connected client used by some module. */
        };

        enum REQTYPE {
            PROTO_REQ_INLINE = 1,
            PROTO_REQ_MULTIBULK = 2
        };
        static std::shared_ptr<Client> createInstance(int fd, int flags, const char *ip) {
            return std::shared_ptr<skss::Client>(new skss::Client(fd, flags, ip));
        }

        ~Client();

        void addFlags(int flags);

        int getFd() const { return fd; }

        inline std::string &getBuf() { return this->query_buf; }

        void processInputBuffer();

        int processInlineBuffer();
        int processMultibulkBuffer();

    private:
        int fd;
        int flags;
        int req_type;
        const std::string ip;
        std::string query_buf;

        Client(int fd, int flags, const char *ip);
    };
};


#endif //SKSS_CLIENT_HPP
