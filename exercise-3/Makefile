.PHONY: all clean server client

CXX:= g++
CXX_FLAGS:= -g -Wall -Wextra -fsanitize=address -O0
LD_FLAGS:= -fsanitize=address

all: server client

server:	build/server

client: build/client

build/server: src/tcp_echo_server.cc | build
	$(CXX) $(CXX_FLAGS) $< -o $@ $(LDFLAGS)

build/client: src/tcp_echo_client.cc | build
	$(CXX) $(CXX_FLAGS) $< -o $@ $(LDFLAGS)

build:
	mkdir -p build

clean:
	rm -rf build