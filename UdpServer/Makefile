all:server client
server: UdpServer.cc
	g++ -o $@ $^ -std=c++11
client: UdpClient.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm client server
