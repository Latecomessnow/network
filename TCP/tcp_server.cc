#include "TcpServer.hpp"

void Usage(std::string proc)
{
	std::cout << "Usage: " << proc << " port" << std::endl;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		Usage(argv[0]);
		exit(1);
	}
	// 绝对不能在此写成	int port = htons(atoi(argv[1])), 需要一个整型类型的端口号去构造服务器对象
	int port = atoi(argv[1]);
	TcpServer *svr = new TcpServer(port);
	svr->InitServer();
	svr->Start();
	return 0;
}