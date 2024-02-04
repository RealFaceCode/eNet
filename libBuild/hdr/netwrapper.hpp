#pragma once

#include <string>
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
constexpr unsigned int MAX_BUF = 512;
constexpr unsigned int WOULD_NOT_BLOCK = 10035;

namespace enet::internal
{
    int InitNetworking();
    void CleanupNetworking();

    int GetAddressInfo(const char *hostname, const char *service, const addrinfo *hints, addrinfo **res);
    socket_t CreateSocket(int domain, int type, int protocol);
    socket_t CreateSocket(const addrinfo* res);
    int ConnectToServer(socket_t sockfd, const sockaddr *serv_addr, socklen_t addrlen);
    void FreeAddressInfo(addrinfo* res);
    void CloseSocket(socket_t sockfd);

    addrinfo FillAddrInfo(int family, int socktype, int protocol, int flags = 0, int addrlen = 0);
    addrinfo* GetAddressInfo(const char *hostname, const char *service, int family, int socktype, int protocol, int flags = 0, int addrlen = 0);
    int ConnectToServer(socket_t sockfd, const addrinfo *res);

    int Bind(socket_t sockfd, const addrinfo *res);
    int Listen(socket_t sockfd, int backlog = ::MAX_CONN);
    socket_t Accept(socket_t sockfd, char** addrStr, char** portStr);

    int Send(socket_t sockfd, const char *buf, int len, int flags);
    int Recv(socket_t sockfd, char *buf, int len, int flags);
    int SendTo(socket_t sockfd, const char *buf, int len, int flags, const addrinfo* addr);
    int RecvFrom(socket_t sockfd, char *buf, int len, int flags, addrinfo* addr);

    bool SetBlocking(socket_t sockfd, bool blocking);

    std::pair<std::string, int> GetError();
}