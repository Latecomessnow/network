.PHONY:all
all:server client

server:CalServer.cc
	g++ -o $@ $^ -std=c++11 -lpthread
client:CalClient.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f server client