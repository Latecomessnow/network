#pragma once
#include <iostream>
#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>
class UdpServer
{
public:
    UdpServer(std::string ip, int port)
        : _sockfd(-1), _ip(ip), _port(port)
    {
    }
    bool UdpServerInit()
    {
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            std::cerr << "sock cerr" << std::endl;
            return false;
        }
        std::cout << "Server socket create success, sockfd: " << _sockfd << std::endl;
        // 填充网络通信相关信息
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = inet_addr(_ip.c_str());
        // 绑定
        if (bind(_sockfd, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            std::cerr << "bind error" << std::endl;
            return false;
        }
        std::cout << "bind success" << std::endl;
    }
    // 启动服务器，服务器读取客户端发送过来的的信息
    void Start()
    {
        char buffer[128];
        // 服务器是死循环，一直在启动着
        while (true)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            // 0表示阻塞式读取
            ssize_t s = recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&peer, &len);
            if (s > 0)
            {
                buffer[s] = '\0';
                int port = ntohs(peer.sin_port);
                std::string ip = inet_ntoa(peer.sin_addr);
                std::cout << "recvfrom success: "
                          << "ip: " << ip << " port: " << port << "# " << buffer << std::endl;
            }
            else
            {
                std::cerr << "recvfrom fail" << std::endl;
            }
        }
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