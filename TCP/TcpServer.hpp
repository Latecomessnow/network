#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <stdlib.h>

// 在初始化TCP服务器时，只有创建套接字成功、绑定成功、监听成功
// 此时TCP服务器的初始化才算完成。
class TcpServer
{
public:
	TcpServer(int port)
		: _listen_sock(-1), _port(port)
	{
	}
	~TcpServer()
	{
		if (_listen_sock >= 0)
		{
			close(_listen_sock);
		}
	}
	void InitServer()
	{
		// 创建套接字,tcp使用的是流式服务SOCK_STREAM
		_listen_sock = socket(AF_INET, SOCK_STREAM, 0);
		if (_listen_sock < 0)
		{
			std::cerr << "socket fail" << std::endl;
			exit(2);
		}
		std::cout << "tcp socket create success!" << std::endl;
		// 绑定
		struct sockaddr_in local;
		memset(&local, 0, sizeof(local));
		local.sin_family = AF_INET;
		local.sin_port = htons(_port);
		local.sin_addr.s_addr = INADDR_ANY;
		if (bind(_listen_sock, (struct sockaddr *)&local, sizeof(local)) < 0)
		{
			std::cerr << "bind fail" << std::endl;
			exit(3);
		}
		std::cout << "bind success!" << std::endl;
		// TCP是面向连接的协议，服务端和客户端在进行TCP通信之前必须先建立连接
		// 所以TCP必须时刻处以监听状态,监听是否有新的客户端连接到来
		// 将服务端创建的套接字设置为监听状态
		// backlog：全连接队列的最大长度,该参数代表的就是这个全连接队列的最大长度，
		// 一般不要设置太大，设置为5或10即可。
		if (listen(_listen_sock, 5) < 0) // 监听成功返回0，监听失败返回-1，同时错误码会被设置。
		{
			std::cerr << "listen fail" << std::endl;
			exit(4);
		}
		std::cout << "listen success!" << std::endl;
	}
	// 回声服务器
	void Server(int sock, std::string client_ip, int client_port)
	{
		char buffer[128];
		while (true)
		{
			// 读取客户端发送过来的消息再回显给客户端
			ssize_t s = read(sock, buffer, sizeof(buffer) - 1);
			// 读取成功，回显给客户端
			if (s > 0)
			{
				buffer[s] = '\0';
				std::cout << "get a new link->" << sock << " [" << client_ip << "]:"
						  << client_port << " client send->" << buffer << std::endl;
				//write(sock, buffer, sizeof(buffer));
				write(sock, buffer, s);
			}
			// 对端关闭
			else if (s == 0)
			{
				std::cout << client_ip << ":" << client_port << " close!"
						  << std::endl;
				break;
			}
			// 读取失败
			else
			{
				std::cerr << "server read error" << std::endl;
				break;
			}
			// 注意服务完毕后一定要归还提供服务的文件描述符，否者服务端可用的文件描述符会越来越少
			// 这段代码存在一个问题，即在服务端处理完一个客户端的请求后，会立即关闭套接字，
			// 而不是在所有请求都处理完成后再关闭。这会导致服务端可用的文件描述符会越来越少，
			// 最终导致服务端无法为新客户端提供服务。
			// 呜呜呜，应该写在循环外
			// close(sock);
			// std::cout << client_ip << ":" << client_port << " server done" << std::endl;
		}
		close(sock);
		std::cout << client_ip << ":" << client_port << " server done" << std::endl;
	}	
	void Start()
	{
		while (true)
		{
			// 获取客户端对端网络信息,对端的sockaddr_in信息
			struct sockaddr_in peer;
			memset(&peer, 0, sizeof(peer));
			socklen_t len = sizeof(peer);
			// 监听套接字只负责获取客户端发过来的连接请求
			// 真正为客户端提供服务的是accept函数获取连接返回的套接字
			int sock = accept(_listen_sock, (struct sockaddr *)&peer, &len);
			if (sock < 0)
			{
				std::cerr << "accept error, continue next" << std::endl;
				// accept函数获取连接时可能会失败，但TCP服务器不会因为获取某个连接失败而退出
				// 因此服务端获取连接失败后应该继续获取连接。
				continue;
			}
			// 客户端的ip地址和端口号
			std::string client_ip = inet_ntoa(peer.sin_addr);
			int client_port = ntohs(peer.sin_port);
			std::cout << "get a new link->" << sock << " [" << client_ip << "]:" << client_port << std::endl;

			// 处理服务请求,处理的服务的套接字是accept函数返回来的套接字
			Server(sock, client_ip, client_port);
		}
	}

private:
	int _listen_sock; // 监听套接字
	int _port;
	// 不需要指定ip绑定了，只需绑定INADDR_ANY即可，这样就可以接受多个网卡的数据了
};