#include "UdpClient.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << "ip " << "port" << std::endl;
    }
    std::string server_ip = argv[1];
    int server_port = atoi(argv[2]);
    UdpClient* up = new UdpClient(server_ip, server_port);
    // 创建套接字
    up->InitClient();
    // 启动客户端，客户端不需要去绑定端口号，只需传入服务端的ip地址和端口号
    // 启动后，向服务端发送数据
    up->Start();
    return 0;
}