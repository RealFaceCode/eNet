#include "netwrapper.hpp"

#include <cstring>

namespace eNet
{
    int initNetwork()
    {
#ifdef _WIN32
        WSADATA wsaData;
        return (::WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0;
#else
        return true;
#endif
    }

    int getAddressInfo(const char *hostname, const char *service, const addrinfo *hints, addrinfo **res)
    {
#ifdef _WIN32
        return ::getaddrinfo(hostname, service, (const addrinfo*)hints, res);
#else
        return ::getaddrinfo(hostname, service, hints, res);
#endif
    }

    socket_t createSocket(int domain, int type, int protocol)
    {
        socket_t sock;
        sock = ::socket(domain, type, protocol);
        if (sock == SOCK_ERR) {
            return SOCK_ERR;
        }
        return sock;
    }

    socket_t createSocket(addrinfo* res)
    {
        return createSocket(res->ai_family, res->ai_socktype, res->ai_protocol);
    }

    int connectToServer(socket_t sockfd, const sockaddr *serv_addr, socklen_t addrlen)
    {
#ifdef _WIN32
        return ::connect(sockfd, serv_addr, (int)addrlen);
#else
        return ::connect(sockfd, serv_addr, addrlen);
#endif
    }

    void freeAddressInfo(addrinfo *res)
    {
        ::freeaddrinfo(res);
    }

    void closeSocket(socket_t sockfd)
    {
#ifdef _WIN32
        ::closesocket(sockfd);
#else
        ::close(sockfd);
#endif
    }

    addrinfo fillAddrInfo(int family, int socktype, int protocol, int flags, int addrlen)
    {
        addrinfo hints;
        std::memset(&hints, 0, sizeof(hints));
        hints.ai_family = family;
        hints.ai_socktype = socktype;
        hints.ai_protocol = protocol;
        hints.ai_flags = flags;
        hints.ai_addrlen = addrlen;
        return hints;
    }

    addrinfo * getAddrInfo(const char *hostname, const char *service, int family, int socktype, int protocol, int flags, int addrlen)
    {
        addrinfo hints = fillAddrInfo(family, socktype, protocol, flags, addrlen);
        addrinfo *result;

        getAddressInfo(hostname, service, &hints, &result);

        return result;
    }

    int connectToServer(socket_t sockfd, const addrinfo *res)
    {
        return connectToServer(sockfd, (const sockaddr*)res->ai_addr, (socklen_t)res->ai_addrlen);
    }

    int bind(socket_t sockfd, const addrinfo *res)
    {
        return ::bind(sockfd, res->ai_addr, res->ai_addrlen);
    }

    int listen(socket_t sockfd, int backlog)
    {
        return ::listen(sockfd, backlog);
    }

    socket_t accept(socket_t sockfd, char** addrStr)
    {
        sockaddr_storage their_addr;
        socklen_t sin_size = sizeof their_addr;
        socket_t sock = ::accept(sockfd, (sockaddr *)&their_addr, &sin_size);
        if (sock == SOCK_ERR) {
            return SOCK_ERR;
        }

        *addrStr = inet_ntoa(((sockaddr_in *)&their_addr)->sin_addr);
        return sock;
    }
}
