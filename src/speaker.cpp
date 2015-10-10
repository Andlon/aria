#include <aria/speaker.hpp>
#include <boost/asio.hpp>
#include <array>

namespace asio = boost::asio;

namespace aria {

    speaker::speaker(speaker_callbacks & callbacks)
        :   _callbacks(callbacks)
    {
        accept();
    }

    void speaker::accept()
    {
        asio::ip::tcp::acceptor acceptor(_io_service,
                                         asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 1582));

        std::array<char, 8192> buffer;

        // Accept only a single connection at a time
        while (true)
        {
            asio::ip::tcp::socket socket(_io_service);
            acceptor.accept(socket);
            boost::system::error_code error_code;

            while (error_code != boost::asio::error::eof) {
                size_t len = socket.read_some(asio::buffer(buffer), error_code);
                _callbacks.audio_received(buffer.data(), len);
            }
        }
    }

}
