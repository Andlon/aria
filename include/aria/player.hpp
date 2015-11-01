#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <boost/asio.hpp>

namespace aria {

    class player_callbacks {
    public:
        virtual void connected() = 0;
        virtual void connection_failed() = 0;
        virtual void disconnected() = 0;
    };

    class null_player_callbacks : public player_callbacks {
        virtual void connected() {}
        virtual void connection_failed() {}
        virtual void disconnected() {}
    };

    class player
    {
    public:
        player(std::shared_ptr<player_callbacks> callbacks);

        void connect_speaker(const std::string &host, uint16_t port);
        void feed_audio(const char * data, size_t length);

    private:
        std::shared_ptr<player_callbacks> _callbacks;

        boost::asio::io_service _io_service;
        boost::asio::ip::tcp::socket _socket;
    };

}
