#pragma once

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include "Protocol.hpp"

class CalClient
{
public:
    CalClient(std::string server_ip, int server_port)
        : _server_ip(server_ip), _server_port(server_port) {}
    ~CalClient()
    {
        if (_client_sock >= 0)
        {
            close(_client_sock);
        }
    }
    void ClientInit()
    {
        // 1. 创建按套接字
        _client_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (_client_sock < 0)
        {
            std::cerr << "socket error!" << std::endl;
            exit(2);
        }
        std::cout << "socket create success!" << std::endl;
    }
    void Start()
    {
        struct sockaddr_in peer;
        memset(&peer, 0, sizeof(peer));
        peer.sin_family = AF_INET;
        peer.sin_port = htons(_server_port);
        peer.sin_addr.s_addr = inet_addr(_server_ip.c_str());
        std::cout << _client_sock << " " << _server_ip << " " << _server_port << std::endl;
        // 2. 连接服务器
        if (connect(_client_sock, (struct sockaddr *)&peer, sizeof(peer)) < 0)
        {
            std::cerr << "connect error!" << std::endl;
            exit(3);
        }
        std::cout << "connect success!" << std::endl;
        // 获取服务器连接成功，开始发起请求任务
        while (true)
        {
            // 构建请求
            Request rq;
            std::cout << "请输入左操作数# ";
            std::cin >> rq.x;
            std::cout << "请输入右操作数# ";
            std::cin >> rq.y;
            std::cout << "请输入操作符# ";
            std::cin >> rq.op;
            send(_client_sock, &rq, sizeof(rq), 0);

            // 接受请求响应
            Response rp;
            recv(_client_sock, &rp, sizeof(rp), 0);
            std::cout << "status: " << rp.code << std::endl;
            std::cout << rq.x << " " << rq.op << " = " << rp.result << std::endl;
        }
    }

private:
    int _client_sock;       // 客户端套接字
    std::string _server_ip; // 服务器ip
    int _server_port;       // 服务器端口号
};