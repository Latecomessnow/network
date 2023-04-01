#include "TcpClient.hpp"

void Usage(std::string str)
{
    std::cout << "Usage: " << str << " server_ip server_port" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }
    std::string server_ip = argv[1];
    int server_port = atoi(argv[2]);
    TcpClient* clt = new TcpClient(server_ip, server_port);
    clt->InitClient();
    clt->Start();
    return 0;
}