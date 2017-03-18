#include <server.hpp>
#include <iostream>
#include <cstdlib>

int main(int argc, char** argv)
{
	if (argc != 2) {
		std::cerr << "Usage: ./skss_service port_number" << std::endl;
		return -1;
	}
	int port = std::atoi(argv[1]);

	skss::Server  server(port);
	server.run();
	
	return 0;
}