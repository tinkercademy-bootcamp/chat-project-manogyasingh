#include "chat-server.h"

#include <spdlog/spdlog.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../net/chat-sockets.h"
#include "../utils.h"

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
      if (events[i].data.fd ==
          server_socket_fd_) {  // check for new connections
        handle_new_connection();
      } else {
        handle_existing_connection(events[i].data.fd);
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

void tt::chat::server::Server::handle_new_connection() {
  while (true) {
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd =
        accept(server_socket_fd_, (sockaddr *)&client_addr, &client_len);

    if (client_fd < 0) {
      SPDLOG_ERROR("Accept Failed.");
    }

    set_non_blocking(client_fd);
    add_to_epoll(client_fd, EPOLLIN | EPOLLET);
    clients_[client_fd] = {client_fd, "", ""};
    SPDLOG_INFO("New client connected: fd={}", client_fd);
  }
}

void tt::chat::server::Server::handle_existing_connection(int sock) {
  char buffer[kBufferSize];
  ssize_t count = read(sock, buffer, sizeof(buffer));
  if (count > 0) {
    // forward the message where it's supposed to be
    send(sock, buffer, count, MSG_NOSIGNAL);  // echo for now
    SPDLOG_INFO("Received: {}", buffer);
    send(sock, buffer, count, 0);
    SPDLOG_INFO("Echo message sent");
  } else if (count == 0) {
    disconnect_client(sock);
  }
  // while (true){
  //   ssize_t count = read(sock, buffer, sizeof(buffer));
  //   if (count < 0) {
  //     disconnect_client(sock); return;
  //   }
  //   clients_[sock].buffer.append(buffer,count);
  //   ClientDataOnServer& client = clients_[sock];
  //   size_t posn;
  //   while ((posn = client.buffer.find('\n'))!=std::string::npos){
  //     std::string message = client.buffer.substr(0,posn);
  //     client.buffer.erase(0,posn+1);

  //     std::string echo = message+"\n";
  //     send(sock,echo.c_str(),echo.length(),MSG_NOSIGNAL);
  //   }
  // }
}

void tt::chat::server::Server::disconnect_client(int sock) {
  SPDLOG_INFO("Client {} Disconnected", sock);
  remove_from_epoll(sock);
  close(sock);
  clients_.erase(sock);
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