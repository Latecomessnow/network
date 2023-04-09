#include "CalServer.hpp"

void Usage(std::string proc)
{
    std::cout << proc << " server port" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    int port = htons(atoi(argv[1]));
    CalServer* svr = new CalServer(port);
    svr->ServerInit();
    svr->Start();
    delete svr;
    return 0;
}