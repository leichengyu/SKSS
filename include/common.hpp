#ifndef COMMON_H_
#define COMMON_H_

#include <glog/logging.h>

namespace skss {

    enum STATUS {
        SKSS_ERR = -1,
        SKSS_OK = 0
    };
    //========= config ===============
    const size_t CONFIG_DEFAULT_TCP_BACKLOG = 511;
    const size_t CONFIG_MIN_RESERVED_FDS = 32;
    const size_t CONFIG_DEFAULT_MAX_CLIENTS = 100000;
    const size_t CONFIG_FDSET_INCR = CONFIG_MIN_RESERVED_FDS + 96;
    const size_t CONFIG_DEFAULT_SETSIZE = CONFIG_FDSET_INCR + CONFIG_DEFAULT_MAX_CLIENTS;

    //========== mask ==================
    const int READABLE = 0x1;
    const int WRITABLE = 0x10;

    //========== network ===============
    const int MAX_ACCEPTS_PER_CALL = 1000;
    const int NET_IP_STR_LEN = 46;

    //========== IO =============
    const size_t PROTO_IOBUF_LEN = 1024 * 16;
    const size_t PROTO_MAX_QUERYBUF_LEN = 1024 * 1024 * 1024;
};

#endif