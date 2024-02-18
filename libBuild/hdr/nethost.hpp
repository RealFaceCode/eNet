#pragma once

#include "address.hpp"
#include "nethostsetting.hpp"
#include "netwrapper.hpp"
#include "msg.hpp"

#include <string_view>
#include <cstdint>
#include <thread>
#include <memory>
#include <queue>
#include <optional>
#include <mutex>

namespace enet::structs
{
    struct NetHost
    {
    public:
        NetHost() = default;
        NetHost(const Address& address, size_t peerCount);
        NetHost(std::string_view host, uint16_t port, size_t peerCount);
        ~NetHost();

        void SetAddress(const Address& address);
        void SetAddress(std::string_view host, uint16_t port);
        void SetPeerCount(size_t peerCount);
        void SetChannelLimit(size_t channelLimit);
        void SetIncomingBandwidth(uint32_t incomingBandwidth);
        void SetOutgoingBandwidth(uint32_t outgoingBandwidth);

        void StartHost();
        void StopHost();

        void send(const Msg& msg);
        std::optional<Msg> recv();

        socket_t handle() const;
    private:
        void createHost();
        void destroyHost();

        Address m_address;
        NetHostSetting m_settings;
        std::shared_ptr<std::pair<std::jthread, std::jthread>> m_sr     = nullptr;
        std::shared_ptr<std::jthread> m_listener                        = nullptr;
        std::shared_ptr<std::queue<Msg>> m_SendMsgQueue                 = nullptr;
        std::shared_ptr<std::queue<Msg>> m_RecvMsgQueue                 = nullptr;
        std::shared_ptr<std::mutex> m_SendMsgQueueMutex                 = nullptr;
        std::shared_ptr<std::mutex> m_RecvMsgQueueMutex                 = nullptr;
        std::shared_ptr<std::vector<NetHost>> m_peers                   = nullptr;
        bool m_activeHost = false;
        socket_t m_socket = INVALID_SOCKET;
    };
}