#ifndef COMMON_H_
#define COMMON_H_

namespace skss {
	const int CONFIG_DEFAULT_TCP_BACKLOG = 511;
	const int CONFIG_MIN_RESERVED_FDS = 32;
	const int CONFIG_DEFAULT_MAX_CLIENTS = 100000;
	const int CONFIG_FDSET_INCR = CONFIG_MIN_RESERVED_FDS + 96;
	const int CONFIG_DEFAULT_SETSIZE = CONFIG_FDSET_INCR + CONFIG_DEFAULT_MAX_CLIENTS;


	//========== mask ==================
	const int READABLE = 0x1;
	const int WRITABLE = 0x10;
};

#endif