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
#include "Protocol.hpp"
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
    // this指针CalServer* const this
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
    void ServerInit()
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
        std::cout << _listen_sock << " " << _port << std::endl;
    }
    static void *Routine(void *arg)
    {
        pthread_detach(pthread_self());
        int sock = *(int *)arg;
        delete (int *)arg;
        // 服务端不断获取客户端发送过来的操作
        while (true)
        {
            Request rq;
            // 静态成员函数不能够访问类的私有成员变量
            // 因为私有成员变量是通过this指针去访问的，而静态成员函数bu
            ssize_t s = recv(sock, &rq, sizeof(rq), 0);
            if (s > 0)
            {
                Response rp = {0, 0};
                switch (rq.op)
                {
                case '+':
                    rp.result = rq.x + rq.y;
                    break;
                case '-':
                    rp.result = rq.x - rq.y;
                    break;
                case '*':
                    rp.result = rq.x * rq.y;
                    break;
                case '/':
                    if (rq.y == 0)
                    {
                        rp.code = 1; // 除0错误
                    }
                    else
                    {
                        rp.result = rq.x / rq.y;
                    }
                    break;
                case '%':
                    if (rq.y == 0)
                    {
                        rp.code = 2; // 模0错误
                    }
                    else
                    {
                        rp.result = rq.x % rq.y;
                    }
                    break;
                default:
                    rp.code = 3; // 非法运算
                    break;
                }
                // 将结果发送给客户端
                send(sock, &rp, sizeof(rp), 0);
            }
            else if (s == 0)
            {
                std::cout << "server done!" << std::endl;
                break;
            }
            else
            {
                std::cerr << "recv error!" << std::endl;
                exit(5);
            }
        }
        close (sock);
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
            std::cout << "test" << std::endl;
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
            int *p = new int(sock);
            // 创建线程去执行例程
            pthread_t tid = 0;
            pthread_create(&tid, nullptr, Routine, (void *)p);
        }
    }

private:
    int _listen_sock;
    int _port;
};