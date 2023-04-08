#pragma once

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <pthread.h>
#include <memory>

class Param
{
public:
    Param(int sock, std::string ip, int port) : _sock(sock), _ip(ip), _port(port) {}
    int _sock;
    std::string _ip;
    int _port;
};
class CalServer
{
public:
    CalServer(int port)
        : _listen_sock(-1), _port(port)
    {
    }
    ~CalServer()
    {
        if (_listen_sock >= 0)
        {
            close(_listen_sock);
        }
    }
    void Serverinit()
    {
        // 1. 创建套接字
        _listen_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (_listen_sock < 0)
        {
            std::cerr << "socket create error!" << std::endl;
            exit(2);
        }
        std::cout << "socket create success!" << std::endl;
        // 2. 绑定端口号、IP
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = INADDR_ANY;
        if (bind(_listen_sock, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            std::cerr << "bind error!" << std::endl;
            exit(3);
        }
        std::cout << "bind success!" << std::endl;
        // 3. 让服务器处于监听状态
        if (listen(_listen_sock, 5) < 0)
        {
            std::cerr << "listen error!" << std::endl;
            exit(4);
        }
        std::cout << "listen success!" << std::endl;
    }
    static void* Routine(void* arg)
    {
        pthread_detach(pthread_self());
        Param* p = (Param*)arg;
        // 服务端不断获取客户端发送过来的操作
        while (true)
        {

        }
        return nullptr;
    }
    // 启动服务器
    void Start()
    {
        while (true)
        {
            struct sockaddr_in peer;
            memset(&peer, 0, sizeof(peer));
            socklen_t len = sizeof(peer);
            // len是一个输出型参数
            int sock = accept(_listen_sock, (struct sockaddr *)&peer, &len);
            if (sock < 0)
            {
                std::cout << "accept error, continue next!" << std::endl;
                continue;
            }
            std::cout << "accept success! sock : " << sock << std::endl;
            // 提取客户端ip和port
            std::string client_ip = inet_ntoa(peer.sin_addr);
            int client_port = ntohs(peer.sin_port);
            std::cout << "client_ip: " << client_ip << " clietn_port: " << client_port << std::endl;
            // unique_ptr<Param> p(new Param(sock, client_ip, client_port));
            // Param* p =  new Param(sock, client_ip, client_port);
            int* p = new int(sock);
            // 创建线程去执行例程
            pthread_t tid = 0;
            pthread_create(&tid, nullptr, Routine, (void *)p);
        }
    }

private:
    int _listen_sock;
    int _port;
};