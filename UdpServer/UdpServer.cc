#include "UdpServer.hpp"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage:" << argv[0] << "udp_server_port" << std::endl;
        return -1;
    }
    std::string ip = "127.0.0.1";
    int port = atoi(argv[1]);
    UdpServer* svr = new UdpServer(ip, port);
    svr->Start();
    return 0;
}