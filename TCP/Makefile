.PHONY:all
all:tcp_server tcp_client

tcp_server:tcp_server.cc
	g++ -o $@ $^ -std=c++11
tcp_client:tcp_client.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm tcp_server tcp_client