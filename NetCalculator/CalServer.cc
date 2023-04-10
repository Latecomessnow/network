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
    // *** 严重错误，输入的端口号是字符串类型，此时需要的是将字符串类型转成int类型
    // 然后使用int类型去传参构造服务器,不能在此转成网络字节序
    //int port = htons(atoi(argv[1]));
    int port = atoi(argv[1]);
    CalServer* svr = new CalServer(port);
    svr->ServerInit();
    svr->Start();
    delete svr;
    return 0;
}