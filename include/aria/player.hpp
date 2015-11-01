#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <boost/asio.hpp>
#include <aria/io_thread.hpp>
#include <vector>

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

    class player : public io_thread
    {
    public:
        player(std::shared_ptr<player_callbacks> callbacks);

        void connect_speaker(const std::string &host, uint16_t port);
        void feed_audio(std::vector<char> && data);

    protected:
        virtual void before_io_service() override;

    private:
        typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
        std::shared_ptr<player_callbacks> _callbacks;

        boost::asio::ip::tcp::resolver _resolver;
        std::vector<socket_ptr> _sockets;
    };

}
