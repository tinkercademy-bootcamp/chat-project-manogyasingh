#include "xtchat-server.h"

#include <spdlog/spdlog.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

xtc::server::Server::Server(int port)
    : server_socket_fd_(xtc::net::create_socket()),
      server_address_(xtc::net::create_address(port)),
      port_(port) {
  opt_bind_listen_epoll();
}

xtc::server::Server::~Server() {
  close(epoll_fd_);
  close(server_socket_fd_);
}

// the one function to set socket options, address options,
// bind, start listening on, and add to epoll
// (the server socket file descriptor)
void xtc::server::Server::opt_bind_listen_epoll() {
  int err_code;

  // set the socket option to allow reusing address and port
  int opt = 1;
  err_code = setsockopt(server_socket_fd_, SOL_SOCKET,
                        SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
  xtc::check_error(err_code < 0, "setsockopt() error\n");

  // allow any address to connect
  server_address_.sin_addr.s_addr = INADDR_ANY;

  // bind socket to the address
  err_code = bind(server_socket_fd_, (sockaddr *)&server_address_,
                  sizeof(server_address_));
  xtc::check_error(err_code < 0, "bind failed\n");

  // listen on the server fd_
  err_code = listen(server_socket_fd_, 5);
  xtc::check_error(err_code < 0, "listen failed\n");
  std::cout << "Server listening on port " << port_ << "\n";

  // Make epoll fd
  epoll_fd_ = wrap_socket_in_epoll(server_socket_fd_);
}

int xtc::server::wrap_socket_in_epoll(int server_socket_fd) {
  int epoll_fd = epoll_create1(0);
  xtc::check_error(epoll_fd < 0, "Couldn't start epoll socket.\n");

  epoll_event ev;
  ev.events = EPOLLIN | EPOLLET;
  ev.data.fd = server_socket_fd;

  int err_code = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket_fd, &ev);
  xtc::check_error(err_code < 0, "Could not add event to epoll");

  return epoll_fd;
}