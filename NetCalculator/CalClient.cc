#include "CalClient.hpp"

void Usage(std::string proc)
{
    std::cout << proc << " server_ip server_port" << std::endl;
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
    CalClient* clt = new CalClient(server_ip, server_port);
    clt->ClientInit();
    clt->Start();
    delete clt;
    return 0;
}