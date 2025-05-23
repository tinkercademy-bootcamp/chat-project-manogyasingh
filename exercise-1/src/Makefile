.PHONY: clean

all: server client

server:
	mkdir -p build
	g++ tcp_echo_server.cc -o build/server.o

client:
	mkdir -p build
	g++ tcp_echo_client.cc -o build/client.o

clean:
	rm -r build