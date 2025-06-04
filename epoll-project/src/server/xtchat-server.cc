#include "xtchat-server.h"

#include <fcntl.h>
#include <netinet/in.h>
#include <spdlog/spdlog.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

xtc::server::Server::Server(int port)
    : server_socket_fd_(xtc::net::create_socket()),
      server_address_(xtc::net::create_address(port)),
      port_(port) {
  opt_bind_listen();
  setup_epoll();
}

xtc::server::Server::~Server() {
  close(epoll_fd_);
  close(server_socket_fd_);
}

// the one function to set socket options, address options,
// bind, start listening on, and add to epoll
// (the server socket file descriptor)
void xtc::server::Server::opt_bind_listen() {
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
}

void xtc::server::Server::setup_epoll() {
  epoll_fd_ = epoll_create1(0);
  xtc::check_error(epoll_fd_ < 0, "Couldn't make epoll socket");
  add_to_epoll(server_socket_fd_, EPOLLIN | EPOLLET);
}

void xtc::server::Server::add_to_epoll(int sock, uint32_t events) {
  epoll_event ev;
  ev.events = events;
  ev.data.fd = sock;

  int err_code = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, sock, &ev);
  xtc::check_error(err_code < 0, "Could not add event to epoll");
}

void xtc::server::Server::remove_from_epoll(int sock) {
  epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, sock, nullptr);
}

void xtc::server::Server::set_non_blocking(int sock) {
  int flags = fcntl(sock, F_GETFL, 0);
  int err_code = fcntl(sock, F_SETFL, flags | O_NONBLOCK);
  xtc::check_error(err_code < 0, "Failed to set server socket to non blocking");
}

void xtc::server::Server::handle_connections(){
  
}