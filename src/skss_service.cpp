#include <server.hpp>
#include <iostream>

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: ./skss_service port_number" << std::endl;
        return -1;
    }

    ::google::InitGoogleLogging(argv[0]);
    // Log to stderr
    FLAGS_logtostderr = 1;

    int port = std::atoi(argv[1]);

    skss::Server server(port);
    server.run();

    return 0;
}