#pragma once

namespace enet::structs
{
    struct NetHostSetting
    {
    public:
        size_t m_peerCount = 0;
        size_t m_channelLimit = 0;
        uint32_t m_incomingBandwidth = 0;
        uint32_t m_outgoingBandwidth = 0;
    };
}