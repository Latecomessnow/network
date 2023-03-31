#include "UdpServer.hpp"
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0]<< " port" << std::endl;
        return 1;
    }
    //int fd = open("test.txt", O_WRONLY | O_RDONLY | O_CREAT);
    std::string ip = "127.0.0.1"; //本地环回
    // 将ip设置为云服务的公网，而不是本地环回，此时会发现bind失败
    // 由于云服务器的IP地址是由对应的云厂商提供的，这个IP地址并不一定是真正的公网IP，
    // 这个IP地址是不能直接被绑定的，如果需要让外网访问，此时我们需要bind 0。
    // 系统当当中提供的一个INADDR_ANY，这是一个宏值，它对应的值就是0。
    //std::string ip = "119.91.112.204";
    int port = atoi(argv[1]);
    UdpServer* svr = new UdpServer(ip, port);
    svr->UdpServerInit();
    svr->Start();
    return 0;
}