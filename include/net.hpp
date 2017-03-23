//
// Created by zearom32 on 17-3-23.
//

#ifndef SKSS_NET_HPP
#define SKSS_NET_HPP

#include <common.hpp>

namespace skss {
    namespace net {
        int setNonBlock(int fd, int non_block);

        int createAndBind(const char *str_port);
    };
};
#endif //SKSS_NET_HPP
