.PHONY: all clean server client

CXX:= g++
CXX_FLAGS:= -s
all: server client

server:
	mkdir -p build
	$(CXX) $(CXX_FLAGS) src/tcp_echo_server.cc -o build/server

client:
	mkdir -p build
	$(CXX) $(CXX_FLAGS) src/tcp_echo_client.cc -o build/client

clean:
	rm -rf build