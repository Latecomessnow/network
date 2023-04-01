#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

class TcpClient
{
public:
    TcpClient(std::string server_ip, int server_port)
        : _client_sock(-1), _server_ip(server_ip), _server_port(server_port)
    {
    }
    ~TcpClient()
    {
        if (_client_sock >= 0)
        {
            close(_client_sock);
        }
    }
    // 由于客户端不需要绑定，也不需要监听，因此当客户端创建完套接字后就可以向服务端发起连接请求
    void InitClient()
    {
        // 创建套接字
        _client_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (_client_sock < 0)
        {
            std::cerr << "client socket eror" << std::endl;
            exit(2);
        }
        std::cout << "client socket success" << std::endl;
    }
    void Request()
    {
        std::string msg;
        char buffer[128];
        // 向服务端发送数据并回显到客户端
        while (true)
        {
            // std::string msg;
            std::cout << "Please Enter#";
            getline(std::cin, msg);
            write(_client_sock, msg.c_str(), msg.size());

            ssize_t s = read(_client_sock, buffer, sizeof(buffer) - 1);
            if (s > 0)
            {
                buffer[s] = '\0';
                std::cout << "server echo#" << buffer << std::endl;
            }
            else if (s == 0)
            {
                // 服务端关闭了写
                std::cout << "server close" << std::endl;
                break;
            }
            else
            {
                std::cerr << "client read error!" << std::endl;
                break;
            }
        }
    }
    void Start()
    {
        struct sockaddr_in peer;
        memset(&peer, 0, sizeof(peer));
        // fill in the server network information
        peer.sin_family = AF_INET;
        peer.sin_port = htons(_server_port);
        peer.sin_addr.s_addr = inet_addr(_server_ip.c_str());

        // connect server success return 0
        if (connect(_client_sock, (struct sockaddr *)&peer, sizeof(peer)) == 0)
        {
            std::cout << "client connect success..." << std::endl;
            // send request to tcp server
            Request(); // only read and write in the socket, no need parameters
        }
        else
        {   // connect error
            std::cerr << "connect failed..." << std::endl;
            exit(3);
        }
    }

private:
    int _client_sock;       // 客户端套接字
    std::string _server_ip; // 服务端ip地址
    int _server_port;       // 服务端端口号
};