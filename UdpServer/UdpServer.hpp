#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>

class UdpServer
{
public:
    UdpServer(std::string& ip, int port)
        : _ip(ip), _port(port)
    {
        UdpServerInit();
    }
    void UdpServerInit()
    {
        // 1. 创建套接字
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            std::cout << "socket create fail!" << std::endl;
            exit(-1);
        }
        std::cout << "socket create success!" << std::endl;
        // 2. 绑定端口号
        struct sockaddr_in local;
        memset(&local, '0', sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        // local.sin_addr.s_addr = inet_addr(_ip.c_str());
        local.sin_addr.s_addr = INADDR_ANY;
        if (bind(_sockfd, (struct sockaddr*)&local, sizeof(local)) < 0)
        {
            std::cout << "udp_server bind fail!" << std::endl;
            exit(-2);
        }
        std::cout << "udp_server bind successs!" << std::endl;
    }
    void Start()
    {
        // UDP服务不需要监听, 直接启动服务器，recvfrom获取客户端发送过来的消息
        while (true)
        {
            char buffer[128] = { 0 };
            // 读取消息时也可以去获取对端的网络信息
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            ssize_t size = recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&peer, &len);
            if (size > 0)
            {
                buffer[size] = '\0';
                // 将客户端的整数ip转成点分十进制ip，使用inet_ntoa接口
                std::string client_ip = inet_ntoa(peer.sin_addr);
                int client_port = ntohs(peer.sin_port);
                std::cout << "get message->" << "client_ip:" << client_ip << " client_port: " << client_port << "#" << buffer << std::endl;
            }
            else
                std::cout << "recvfrom error!" << std::endl;
        }
    }
    ~UdpServer()
    {
        if (_sockfd >= 0) close(_sockfd);
    }
private:
    int _sockfd;     // 网络套接字文件描述符
    int _port;       // 服务器端口号
    std::string _ip; // 服务器ip地址
};