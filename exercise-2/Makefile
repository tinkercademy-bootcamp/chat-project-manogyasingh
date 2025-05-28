.PHONY: clean

all: server client

server: build/server.o
	mkdir -p build
	g++ -g src/tcp_echo_server.cc -o build/server.o

client: build/client.o
	mkdir -p build
	g++ -g src/tcp_echo_client.cc -o build/client.o

clean:
	rm -r build