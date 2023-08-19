#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>

class UdpClient
{
public:
    UdpClient(std::string& server_ip, int server_port)
        : _sockfd(-1), _server_ip(server_ip), _server_port(server_port)
    {
        UdpClientInit();
    }
    ~UdpClient()
    {
        if (_sockfd >= 0)
        {
            close(_sockfd);
        }
    }
    void UdpClientInit()
    {
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            std::cout << "client_socket create fail!" << std::endl;
            exit(-1);
        }
        std::cout << "client_socket create success!" << std::endl;
    }
    void Start()
    {
        struct sockaddr_in server;
        memset(&server, '0', sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = htons(_server_port);
        server.sin_addr.s_addr = inet_addr(_server_ip.c_str());
        std::string msg;
        while (true)
        {
            std::cout << "Please Enter#";
            getline(std::cin, msg);
            sendto(_sockfd, msg.c_str(), msg.size(), 0, (struct sockaddr*)&server, sizeof(server));
        }
    }
private:
    std::string _server_ip;
    int _server_port;
    int _sockfd;
};