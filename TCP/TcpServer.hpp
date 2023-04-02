#pragma once
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include "threadPool.hpp"

// 在初始化TCP服务器时，只有创建套接字成功、绑定成功、监听成功
// 此时TCP服务器的初始化才算完成。
// class TcpServer
// {
// public:
// 	TcpServer(int port)
// 		: _listen_sock(-1), _port(port)
// 	{
// 	}
// 	~TcpServer()
// 	{
// 		if (_listen_sock >= 0)
// 		{
// 			close(_listen_sock);
// 		}
// 	}
// 	void InitServer()
// 	{
// 		// 创建套接字,tcp使用的是流式服务SOCK_STREAM
// 		_listen_sock = socket(AF_INET, SOCK_STREAM, 0);
// 		if (_listen_sock < 0)
// 		{
// 			std::cerr << "socket fail" << std::endl;
// 			exit(2);
// 		}
// 		std::cout << "tcp socket create success!" << std::endl;
// 		// 绑定
// 		struct sockaddr_in local;
// 		memset(&local, 0, sizeof(local));
// 		local.sin_family = AF_INET;
// 		local.sin_port = htons(_port);
// 		local.sin_addr.s_addr = INADDR_ANY;
// 		if (bind(_listen_sock, (struct sockaddr *)&local, sizeof(local)) < 0)
// 		{
// 			std::cerr << "bind fail" << std::endl;
// 			exit(3);
// 		}
// 		std::cout << "bind success!" << std::endl;
// 		// TCP是面向连接的协议，服务端和客户端在进行TCP通信之前必须先建立连接
// 		// 所以TCP必须时刻处以监听状态,监听是否有新的客户端连接到来
// 		// 将服务端创建的套接字设置为监听状态
// 		// backlog：全连接队列的最大长度,该参数代表的就是这个全连接队列的最大长度，
// 		// 一般不要设置太大，设置为5或10即可。
// 		if (listen(_listen_sock, 5) < 0) // 监听成功返回0，监听失败返回-1，同时错误码会被设置。
// 		{
// 			std::cerr << "listen fail" << std::endl;
// 			exit(4);
// 		}
// 		std::cout << "listen success!" << std::endl;
// 	}
// 	// 回声服务器
// 	void Server(int sock, std::string client_ip, int client_port)
// 	{
// 		char buffer[128];
// 		while (true)
// 		{
// 			// 读取客户端发送过来的消息再回显给客户端
// 			ssize_t s = read(sock, buffer, sizeof(buffer) - 1);
// 			// 读取成功，回显给客户端
// 			if (s > 0)
// 			{
// 				buffer[s] = '\0';
// 				std::cout << "get a new link->" << sock << " [" << client_ip << "]:"
// 						  << client_port << " client send->" << buffer << std::endl;
// 				//write(sock, buffer, sizeof(buffer));
// 				write(sock, buffer, s);
// 			}
// 			// 对端关闭
// 			else if (s == 0)
// 			{
// 				std::cout << client_ip << ":" << client_port << " close!"
// 						  << std::endl;
// 				break;
// 			}
// 			// 读取失败
// 			else
// 			{
// 				std::cerr << "server read error" << std::endl;
// 				break;
// 			}
// 			// 注意服务完毕后一定要归还提供服务的文件描述符，否者服务端可用的文件描述符会越来越少
// 			// 这段代码存在一个问题，即在服务端处理完一个客户端的请求后，会立即关闭套接字，
// 			// 而不是在所有请求都处理完成后再关闭。这会导致服务端可用的文件描述符会越来越少，
// 			// 最终导致服务端无法为新客户端提供服务。
// 			// 呜呜呜，应该写在循环外
// 			// close(sock);
// 			// std::cout << client_ip << ":" << client_port << " server done" << std::endl;
// 		}
// 		close(sock);
// 		std::cout << client_ip << ":" << client_port << " server done" << std::endl;
// 	}
// 	void Start()
// 	{
// 		while (true)
// 		{
// 			// 获取客户端对端网络信息,对端的sockaddr_in信息
// 			struct sockaddr_in peer;
// 			memset(&peer, 0, sizeof(peer));
// 			socklen_t len = sizeof(peer);
// 			// 监听套接字只负责获取客户端发过来的连接请求
// 			// 真正为客户端提供服务的是accept函数获取连接返回的套接字
// 			int sock = accept(_listen_sock, (struct sockaddr *)&peer, &len);
// 			if (sock < 0)
// 			{
// 				std::cerr << "accept error, continue next" << std::endl;
// 				// accept函数获取连接时可能会失败，但TCP服务器不会因为获取某个连接失败而退出
// 				// 因此服务端获取连接失败后应该继续获取连接。
// 				continue;
// 			}
// 			// 客户端的ip地址和端口号
// 			std::string client_ip = inet_ntoa(peer.sin_addr);
// 			int client_port = ntohs(peer.sin_port);
// 			std::cout << "get a new link->" << sock << " [" << client_ip << "]:" << client_port << std::endl;

// 			// 处理服务请求,处理的服务的套接字是accept函数返回来的套接字
// 			Server(sock, client_ip, client_port);
// 		}
// 	}

// private:
// 	int _listen_sock; // 监听套接字
// 	int _port;
// 	// 不需要指定ip绑定了，只需绑定INADDR_ANY即可，这样就可以接受多个网卡的数据了
// };

////////////////////////////////////////////////////////////////////////////////////
// // 多进程版服务器
// class TcpServer
// {
// public:
// 	TcpServer(int port)
// 		: _listen_sock(-1), _port(port)
// 	{
// 	}
// 	~TcpServer()
// 	{
// 		if (_listen_sock >= 0)
// 		{
// 			close(_listen_sock);
// 		}
// 	}
// 	void InitServer()
// 	{
// 		// 创建套接字,tcp使用的是流式服务SOCK_STREAM
// 		_listen_sock = socket(AF_INET, SOCK_STREAM, 0);
// 		if (_listen_sock < 0)
// 		{
// 			std::cerr << "socket fail" << std::endl;
// 			exit(2);
// 		}
// 		std::cout << "tcp socket create success!" << std::endl;
// 		// 绑定
// 		struct sockaddr_in local;
// 		memset(&local, 0, sizeof(local));
// 		local.sin_family = AF_INET;
// 		local.sin_port = htons(_port);
// 		local.sin_addr.s_addr = INADDR_ANY;
// 		if (bind(_listen_sock, (struct sockaddr *)&local, sizeof(local)) < 0)
// 		{
// 			std::cerr << "bind fail" << std::endl;
// 			exit(3);
// 		}
// 		std::cout << "bind success!" << std::endl;
// 		// TCP是面向连接的协议，服务端和客户端在进行TCP通信之前必须先建立连接
// 		// 所以TCP必须时刻处以监听状态,监听是否有新的客户端连接到来
// 		// 将服务端创建的套接字设置为监听状态
// 		// backlog：全连接队列的最大长度,该参数代表的就是这个全连接队列的最大长度，
// 		// 一般不要设置太大，设置为5或10即可。
// 		if (listen(_listen_sock, 5) < 0) // 监听成功返回0，监听失败返回-1，同时错误码会被设置。
// 		{
// 			std::cerr << "listen fail" << std::endl;
// 			exit(4);
// 		}
// 		std::cout << "listen success!" << std::endl;
// 	}
// 	// 回声服务器
// 	void Server(int sock, std::string client_ip, int client_port)
// 	{
// 		char buffer[128];
// 		while (true)
// 		{
// 			// 读取客户端发送过来的消息再回显给客户端
// 			ssize_t s = read(sock, buffer, sizeof(buffer) - 1);
// 			// 读取成功，回显给客户端
// 			if (s > 0)
// 			{
// 				buffer[s] = '\0';
// 				std::cout << "get a new link->" << sock << " [" << client_ip << "]:"
// 						  << client_port << " client send->" << buffer << std::endl;
// 				// write(sock, buffer, sizeof(buffer));
// 				write(sock, buffer, s);
// 			}
// 			// 对端关闭
// 			else if (s == 0)
// 			{
// 				std::cout << client_ip << ":" << client_port << " close!"
// 						  << std::endl;
// 				break;
// 			}
// 			// 读取失败
// 			else
// 			{
// 				std::cerr << "server read error" << std::endl;
// 				break;
// 			}
// 			// 注意服务完毕后一定要归还提供服务的文件描述符，否者服务端可用的文件描述符会越来越少
// 			// 这段代码存在一个问题，即在服务端处理完一个客户端的请求后，会立即关闭套接字，
// 			// 而不是在所有请求都处理完成后再关闭。这会导致服务端可用的文件描述符会越来越少，
// 			// 最终导致服务端无法为新客户端提供服务。
// 			// 呜呜呜，应该写在循环外
// 			// close(sock);
// 			// std::cout << client_ip << ":" << client_port << " server done" << std::endl;
// 		}
// 		close(sock);
// 		std::cout << client_ip << ":" << client_port << " server done" << std::endl;
// 	}
// 	void Start()
// 	{
// 		// 父进程忽略子进程退出信号，只关注自己的任务
// 		signal(SIGCHLD, SIG_IGN); // 信号处理动作SIG_IGN
// 		while (true)
// 		{
// 			// 获取客户端对端网络信息,对端的sockaddr_in信息
// 			struct sockaddr_in peer;
// 			memset(&peer, 0, sizeof(peer));
// 			socklen_t len = sizeof(peer);
// 			// 监听套接字只负责获取客户端发过来的连接请求
// 			// 真正为客户端提供服务的是accept函数获取连接返回的套接字
// 			int sock = accept(_listen_sock, (struct sockaddr *)&peer, &len);
// 			if (sock < 0)
// 			{
// 				std::cerr << "accept error, continue next" << std::endl;
// 				// accept函数获取连接时可能会失败，但TCP服务器不会因为获取某个连接失败而退出
// 				// 因此服务端获取连接失败后应该继续获取连接。
// 				continue;
// 			}
// 			// 客户端的ip地址和端口号
// 			std::string client_ip = inet_ntoa(peer.sin_addr);
// 			int client_port = ntohs(peer.sin_port);
// 			std::cout << "get a new link->" << sock << " [" << client_ip << "]:" << client_port << std::endl;

// 			// 处理服务请求,处理的服务的套接字是accept函数返回来的套接字
// 			// Server(sock, client_ip, client_port);

// 			// 1. 创建子进程去处理服务，并让父进程忽略子进程的退出信号，父进程就不要关心子进程了(无需wait)
// 			// pid_t id = fork();
// 			// if (id == 0) // child
// 			// {
// 			// 	// 处理请求
// 			// 	Server(sock, client_ip, client_port);
// 			// 	// 子进程提供完服务后退出
// 			// 	exit(0);
// 			// }

// 			// 2. 创建孙子进程去替工服务，爸爸进程立即退出让爷爷进程立即能够等待成功
// 			pid_t id = fork(); // 创建爸爸进程
// 			if (id == 0)	   // 爸爸进程
// 			{
// 				close(_listen_sock); // child关闭监听套接字
// 				// 创建孙子进程后，爸爸进程立即退出
// 				if (fork() > 0)
// 				{
// 					// 爸爸进程退出
// 					exit(0);
// 				}
// 				// 孙子进程提供服务
// 				Server(sock, client_ip, client_port);
// 				exit(0); // 孙子进程提供完服务退出
// 			}
// 			close(sock);			 // father关闭为连接提供服务的套接字
// 			waitpid(id, nullptr, 0); // 等待爸爸进程（会立刻等待成功）
// 		}
// 	}

// private:
// 	int _listen_sock; // 监听套接字
// 	int _port;
// 	// 不需要指定ip绑定了，只需绑定INADDR_ANY即可，这样就可以接受多个网卡的数据了
// };

// while :; do ps axj | head -1 && ps axj | grep tcp_server | grep -v grep;echo "######################";sleep 1;done
// 此时可以看到，一开始没有客户端连接该服务器，此时服务进程只有一个，
// 该服务进程就是不断获取新连接的进程，
// 而获取到新连接后也是由该进程创建子进程为对应客户端提供服务的。

////////////////////////////////////////////////////////////////////////////////
// 多线程版服务器

// 注意Handler类的定义需要放在Task类定义前
class Handler
{
public:
	Handler()
	{
	}
	~Handler()
	{
	}
	void operator()(int sock, std::string client_ip, int client_port)
	{
		char buffer[1024];
		while (true)
		{
			ssize_t size = read(sock, buffer, sizeof(buffer) - 1);
			if (size > 0)
			{ // 读取成功
				buffer[size] = '\0';
				std::cout << client_ip << ":" << client_port << "# " << buffer << std::endl;

				write(sock, buffer, size);
			}
			else if (size == 0)
			{ // 对端关闭连接
				std::cout << client_ip << ":" << client_port << " close!" << std::endl;
				break;
			}
			else
			{ // 读取失败
				std::cerr << sock << " read error!" << std::endl;
				break;
			}
		}
		close(sock); // 归还文件描述符
		std::cout << client_ip << ":" << client_port << " service done!" << std::endl;
	}
};
class Task
{
public:
	Task()
	{
	}
	Task(int sock, std::string client_ip, int client_port)
		: _sock(sock), _client_ip(client_ip), _client_port(client_port)
	{
	}
	~Task()
	{
	}
	// 任务处理函数
	void Run()
	{
		_handler(_sock, _client_ip, _client_port); // 调用仿函数
	}

private:
	int _sock;				// 套接字
	std::string _client_ip; // IP地址
	int _client_port;		// 端口号
	Handler _handler;		// 处理方法
};

class Param // 给新线程传入的参数
{
public:
	Param(int sock, std::string ip, int port)
		: _sock(sock), _ip(ip), _port(port)
	{
	}
	int _sock;
	std::string _ip;
	int _port;
};
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
		if (listen(_listen_sock, 5) < 0) // 监听成功返回0，监听失败返回-1，同时错误码会被设置。
		{
			std::cerr << "listen fail" << std::endl;
			exit(4);
		}
		std::cout << "listen success!" << std::endl;
		// 创建线程池
		_tp = new ThreadPool<Task>(); // 构造线程池对象
	}
	// 回声服务器
	static void Server(int sock, std::string client_ip, int client_port)
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
				// write(sock, buffer, sizeof(buffer));
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
		}
		close(sock);
		std::cout << client_ip << ":" << client_port << " server done" << std::endl;
	}
	void Start()
	{
		_tp->ThreadPoolInit(); // 初始化线程池
		// 父进程忽略子进程退出信号，只关注自己的任务
		// signal(SIGCHLD, SIG_IGN); // 信号处理动作SIG_IGN
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

			// 1. 多线程版
			// // 创建线程去执行任务例程
			// Param* p = new Param(sock, client_ip, client_port);
			// pthread_t tid;
			// pthread_create(&tid, nullptr, Handler, (void*)p);

			// 2. 线程池版
			Task task(sock, client_ip, client_port); // 构造任务
			_tp->Push(task);						 // 将任务Push进任务队列
		}
	}
	// 创建线程执行的例程必须是一个参数和返回值都是void*的函数
	// 所以将该类内成员函数设置为静态成员函数去掉第一个this指针参数
	static void *Handler(void *arg)
	{
		pthread_detach(pthread_self()); // 分离线程
		Param *p = (Param *)arg;
		Server(p->_sock, p->_ip, p->_port);
		delete p;
		return nullptr;
	}

private:
	int _listen_sock;
	int _port;
	ThreadPool<Task> *_tp;
};