#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <boost/asio.hpp>
#include <speaker_connection.hpp>
#include <aria/io_thread.hpp>

namespace aria {

    class speaker_callbacks
    {
    public:
        virtual void audio_received(const char * data, size_t length) = 0;
        virtual void player_connected() = 0;
        virtual void player_disconnected() = 0;
    };

    class speaker : public io_thread
    {
    public:
        speaker(speaker_callbacks & callbacks);

    protected:
        void before_io_service() override;

    private:
        void accept();

        boost::asio::ip::tcp::acceptor _acceptor;
        speaker_callbacks & _callbacks;

        // Note: _connection must only be accessed from the io thread
        std::shared_ptr<speaker_connection> _connection;
    };

}
