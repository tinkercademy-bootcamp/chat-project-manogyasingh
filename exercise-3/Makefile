.PHONY: all clean server client

CXX:= g++
CXX_FLAGS:= -g -Wall -Wextra -fsanatize=address
LD_FLAGS:= -fsanatize=address

all: server client

server:	build/server

client: build/client

build/server: src/tcp_echo_server.cc | build
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

build/client: src/tcp_echo_client.cc | build
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

build:
	mkdir -p build

clean:
	rm -rf build