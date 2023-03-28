#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

class UdpServer
{
public:
    UdpServer(std::string ip, int port)
        : _sockfd(-1)
        , _ip(ip)
        , _port(port)
    {}
    bool UdpServerInit()
    {
        _sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_sockfd < 0)
        {
            std::cerr << "sock cerr" << std::endl;
            return false;
        }
        std::cout << "socket create success, sockfd: " << _sockfd << std::endl;
        // 填充网络通信相关信息
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = inet_addr(_ip.c_str());
        // 绑定
        if (bind(_sockfd, (struct sockaddr*)&local, sizeof(local)) < 0)
        {
            std::cerr << "bind error" << std::endl;
            return false;
        }
        std::cout << "bind success" << std::endl;
    }
    ~UdpServer()
    {
        if (_sockfd >= 0)
        {
            close(_sockfd);
        }
    }

private:
    int _sockfd;     // 文件描述符
    std::string _ip; // ip地址
    int _port;       // 端口号
};