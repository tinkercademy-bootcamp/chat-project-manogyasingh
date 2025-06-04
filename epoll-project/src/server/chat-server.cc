#include <spdlog/spdlog.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "../net/chat-sockets.h"
#include "../utils.h"
#include "chat-server.h"

tt::chat::server::Server::Server(int port)
    : server_socket_fd_(tt::chat::net::create_socket()),
      address_(tt::chat::net::create_address(port)) {
  using namespace tt::chat;
  set_socket_options(server_socket_fd_, 1);

  address_.sin_addr.s_addr = INADDR_ANY;

  auto err_code =
      bind(server_socket_fd_, (sockaddr *)&address_, sizeof(address_));
  tt::chat::check_error(err_code < 0, "bind failed\n");

  err_code = listen(server_socket_fd_, 3);
  check_error(err_code < 0, "listen failed\n");
  std::cout << "Server listening on port " << port << "\n";
  setup_epoll();
  handle_connections_epoll();
}

tt::chat::server::Server::~Server() {
  close(server_socket_fd_);
  close(epoll_fd_);
}

void tt::chat::server::Server::handle_connections_epoll() {
  epoll_event events[kMaxEvents];

  while (true) {
    int nfds = epoll_wait(epoll_fd_, events, kMaxEvents, -1);
    for (int i = 0; i < nfds; i++) {
      if (events[i].data.fd == server_socket_fd_) { // check for new connections
        // currently only trying to get one client to connect
        // for this, using the same logic as before
        socklen_t address_size = sizeof(address_);
        int accepted_socket =
            accept(server_socket_fd_, (sockaddr *)&address_, &address_size);
        tt::chat::check_error(accepted_socket < 0, "Accept error n ");
        handle_accept(accepted_socket);
      }
    }
  }
}

void tt::chat::server::Server::set_socket_options(int sock, int opt) {
  using namespace tt::chat;
  auto err_code = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                             &opt, sizeof(opt));
  check_error(err_code < 0, "setsockopt() error\n");
}

void tt::chat::server::Server::handle_accept(int sock) {
  using namespace tt::chat;

  char buffer[kBufferSize] = {0};
  ssize_t read_size = read(sock, buffer, kBufferSize);

  if (read_size > 0) {
    SPDLOG_INFO("Received: {}", buffer);
    send(sock, buffer, read_size, 0);
    SPDLOG_INFO("Echo message sent");
  } else if (read_size == 0) {
    SPDLOG_INFO("Client disconnected.");
  } else {
    SPDLOG_ERROR("Read error on client socket {}", server_socket_fd_);
  }
  close(sock);
}

void tt::chat::server::Server::set_non_blocking(int sock) {
  int flags = fcntl(sock, F_GETFL, 0);
  int err_code = fcntl(sock, F_SETFL, flags | O_NONBLOCK);
  tt::chat::check_error(err_code < 0,
                        "Failed to set server socket to non blocking");
}

void tt::chat::server::Server::setup_epoll() {
  epoll_fd_ = epoll_create1(0);
  tt::chat::check_error(epoll_fd_ < 0, "Couldn't make epoll socket");
  add_to_epoll(server_socket_fd_, EPOLLIN | EPOLLET);
}

void tt::chat::server::Server::add_to_epoll(int sock, uint32_t events) {
  epoll_event ev;
  ev.events = events;
  ev.data.fd = sock;

  int err_code = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, sock, &ev);
  tt::chat::check_error(err_code < 0, "Could not add event to epoll");
}

void tt::chat::server::Server::remove_from_epoll(int sock) {
  epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, sock, nullptr);
}