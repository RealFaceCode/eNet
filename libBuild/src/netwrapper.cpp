#include "netwrapper.hpp"

#include <cstring>
#include <stdio.h>
#include <array>

#include "elog.hpp"

namespace enet::internal
{

    int InitNetworking()
    {
#ifdef _WIN32
        WSADATA wsaData;
        return (::WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0;
#else
        return true;
#endif
    }

    void CleanupNetworking()
    {
#ifdef _WIN32
        ::WSACleanup();
#endif
    }

    int GetAddressInfo(const char *hostname, const char *service, const addrinfo *hints, addrinfo **res)
    {
        return ::getaddrinfo(hostname, service, hints, res);
    }

    socket_t CreateSocket(int domain, int type, int protocol)
    {
        socket_t sock;
        sock = ::socket(domain, type, protocol);
        if (sock == ::SOCK_ERR) {
            return ::SOCK_ERR;
        }
        return sock;
    }

    socket_t CreateSocket(const addrinfo* res)
    {
        return CreateSocket(res->ai_family, res->ai_socktype, res->ai_protocol);
    }

    int ConnectToServer(socket_t sockfd, const sockaddr *serv_addr, socklen_t addrlen)
    {
        return ::connect(sockfd, serv_addr, addrlen);
    }

    void FreeAddressInfo(addrinfo *res)
    {
        ::freeaddrinfo(res);
    }

    void CloseSocket(socket_t sockfd)
    {
#ifdef _WIN32
        ::closesocket(sockfd);
#else
        ::close(sockfd);
#endif
    }

    addrinfo FillAddrInfo(int family, int socktype, int protocol, int flags, int addrlen)
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

    addrinfo * GetAddressInfo(const char *hostname, const char *service, int family, int socktype, int protocol, int flags, int addrlen)
    {
        addrinfo hints = FillAddrInfo(family, socktype, protocol, flags, addrlen);
        addrinfo *result;

        GetAddressInfo(hostname, service, &hints, &result);

        return result;
    }

    int ConnectToServer(socket_t sockfd, const addrinfo *res)
    {
        return ConnectToServer(sockfd, (const sockaddr*)res->ai_addr, (socklen_t)res->ai_addrlen);
    }

    int Bind(socket_t sockfd, const addrinfo *res)
    {
        return ::bind(sockfd, res->ai_addr,static_cast<int>(res->ai_addrlen));
    }

    int Listen(socket_t sockfd, int backlog)
    {
        return ::listen(sockfd, backlog);
    }

    socket_t Accept(socket_t sockfd, std::string& addrStr, std::string& portStr)
    {
        sockaddr_storage their_addr;
        socklen_t sin_size = sizeof(sockaddr_storage);
        socket_t sock = ::accept(sockfd, (sockaddr *)&their_addr, &sin_size);
        if (sock == ::SOCK_ERR) {
            return ::SOCK_ERR;
        }
        
        addrStr = ::inet_ntoa(((sockaddr_in *)&their_addr)->sin_addr);
        portStr = elog::fmt::Format("{}", ((sockaddr_in *)&their_addr)->sin_port);
        return sock;
    }

    int Send(socket_t sockfd, const char *buf, int len, int flags)
    {
        return ::send(sockfd, buf, len, flags);
    }

    int Recv(socket_t sockfd, char *buf, int len, int flags)
    {
        return ::recv(sockfd, buf, len, flags);
    }

    int SendTo(socket_t sockfd, const char *buf, int len, int flags, const addrinfo* addr)
    {
        return ::sendto(sockfd, buf, len, flags, addr->ai_addr, (int)addr->ai_addrlen);
    }

    int RecvFrom(socket_t sockfd, char *buf, int len, int flags, addrinfo* addr )
    {
#ifdef _WIN32
        return ::recvfrom(sockfd, buf, len, flags, addr->ai_addr, (socklen_t*)addr->ai_addrlen);
#else
        return ::recvfrom(sockfd, buf, len, flags, addr->ai_addr, addr->ai_addrlen);
#endif
    }

    bool SetBlocking(socket_t sockfd, bool blocking)
    {
#ifdef _WIN32
        u_long mode = blocking ? 0 : 1;
        return ::ioctlsocket(sockfd, FIONBIO, &mode) != 0;
#else
        int flags = ::fcntl(sockfd, F_GETFL, 0);
        if (flags == -1) {
            return false;
        }
        flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
        return ::fcntl(sockfd, F_SETFL, flags) != -1;
#endif
    }

    std::pair<std::string, int> GetError()
    {
#ifdef _WIN32
        std::array<char, 256> buffer;
        int err = ::WSAGetLastError();
        ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, err, 0, &buffer[0], 256, nullptr);
        return {buffer.data(), err};
#else
        return {::strerror(errno), errno};
#endif
    }
} // namespace enet::internal