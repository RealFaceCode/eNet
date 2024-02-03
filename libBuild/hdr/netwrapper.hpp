#pragma once



#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    using socket_t = SOCKET;
#else
    #include <netdb.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    using socket_t = int;
#endif

constexpr int SOCK_ERR = -1;
constexpr int INVAL_SOCK = (~0);
constexpr unsigned int MAX_CONN = 0x7fffffff;

namespace eNet
{
    int initNetwork();
    int getAddressInfo(const char *hostname, const char *service, const addrinfo *hints, addrinfo **res);
    socket_t createSocket(int domain, int type, int protocol);
    socket_t createSocket(addrinfo* res);
    int connectToServer(socket_t sockfd, const sockaddr *serv_addr, socklen_t addrlen);
    void freeAddressInfo(addrinfo* res);
    void closeSocket(socket_t sockfd);

    addrinfo fillAddrInfo(int family, int socktype, int protocol, int flags = 0, int addrlen = 0);
    addrinfo * getAddrInfo(const char *hostname, const char *service, int family, int socktype, int protocol, int flags = 0, int addrlen = 0);
    int connectToServer(socket_t sockfd, const addrinfo *res);

    int bind(socket_t sockfd, const addrinfo *res);
    int listen(socket_t sockfd, int backlog = MAX_CONN);

    socket_t accept(socket_t sockfd, char** addrStr);
}