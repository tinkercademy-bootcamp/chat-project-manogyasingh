#ifndef COMMON_UTILS
#define COMMON_UTILS

template <typename T, typename S> void check_error(T test, S error_message);
int create_socket();

const int kBufferSize = 1024;
const int kDefaultPort = 8080;

#endif