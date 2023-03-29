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

class UdpClient
{
public:
    // 客户端需要连接服务器，一开始就要构造好服务器的ip和port
    // 可以从main函数参数获取服务端的这些数据
    UdpClient(std::string server_ip, int server_port)
        : _sockfd(-1), _server_ip(server_ip), _server_port(server_port)
    {
    }
    ~UdpClient()
    {
        if (_sockfd >= 0)
        {
            close(_sockfd);
        }
    }
    bool InitClient()
    {
        // 创建套接字
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            std::cerr << "socket create error" << std::endl;
            return false;
        }
        std::cout << "Client socket create success, sockfd: " << _sockfd << std::endl;
    }
    void Start()
    {
        // 从标准输入中读取数据再发送到服务端
        std::string msg;
        struct sockaddr_in peer;
        memset(&peer, 0, sizeof(peer));
        peer.sin_family = AF_INET;
        peer.sin_port = htons(_server_port);
        peer.sin_addr.s_addr = inet_addr(_server_ip.c_str());
        while (true)
        {
            std::cout << "Please Enter# ";
            getline(std::cin, msg);
            // 此处不能是sizeof(msg)
            sendto(_sockfd, msg.c_str(), msg.size(), 0, (struct sockaddr *)&peer, sizeof(peer));
        }
    }

private:
    int _sockfd;            // 文件描述符
    int _server_port;       // 服务端端口号
    std::string _server_ip; // 服务端ip地址
};