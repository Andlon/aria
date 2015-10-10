#include <string>
#include <cstdint>
#include <memory>
#include <boost/asio.hpp>
#include <thread>
#include <functional>
#include <speaker_connection.hpp>
#include <util/callable_queue.hpp>

namespace aria {

    class speaker_callbacks
    {
    public:
        virtual void audio_received(const char * data, size_t length) = 0;
        virtual void player_connected() = 0;
        virtual void player_disconnected() = 0;
    };

    class speaker
    {
    public:
        speaker(speaker_callbacks & callbacks);

        void start();
        // void stop();

        void process();

    private:
        void accept();
        void run_on_main_thread(std::function<void()> && function);

        boost::asio::io_service _io_service;
        boost::asio::ip::tcp::acceptor _acceptor;
        std::thread _io_thread;
        speaker_callbacks & _callbacks;
        callable_queue _sync_queue;

        // Note: _connection must only be accessed from the io thread
        std::shared_ptr<speaker_connection> _connection;
    };

}
