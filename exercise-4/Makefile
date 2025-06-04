.PHONY: all clean server client

CXX:= g++
CXX_FLAGS:= -g -Wall -Wextra -fsanitize=address -O0
LD_FLAGS:= -fsanitize=address

all: server client

server:	build/server

client: build/client

build/server: src/tcp_echo_server.cpp build/common_utils.o | build
	$(CXX) $(CXX_FLAGS) $^ -o $@ $(LD_FLAGS)

build/client: src/tcp_echo_client.cpp build/common_utils.o | build
	$(CXX) $(CXX_FLAGS) $^ -o $@ $(LD_FLAGS)

build/common_utils.o: src/common_utils.cc src/common_utils.h | build
	$(CXX) $(CXX_FLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build