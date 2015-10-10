#include "speaker_connection.hpp"

namespace asio = boost::asio;
using boost::system::error_code;

namespace aria {

    std::shared_ptr<speaker_connection> speaker_connection::create(boost::asio::ip::tcp::socket &&socket)
    {
        return std::shared_ptr<speaker_connection>(new speaker_connection(std::move(socket)));
    }

    void speaker_connection::async_read(std::function<void (std::vector<char> &&)> audio_received_func,
                                        std::function<void (const error_code&)> disconnected_func)
    {
        auto self = shared_from_this();
        _socket.async_read_some(asio::buffer(_buffer),
                                [=] (const error_code& error, size_t) {
            if (!error) {
                audio_received_func(std::move(_buffer));
                async_read(audio_received_func, disconnected_func);
            } else {
                disconnected_func(error);
            }
        });
    }

    speaker_connection::speaker_connection(boost::asio::ip::tcp::socket &&socket)
        :   _socket(std::move(socket)), _buffer(8192)
    {

    }

}
