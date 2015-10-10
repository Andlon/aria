#include <aria/player.hpp>
#include <string>

namespace asio = boost::asio;

using boost::asio::ip::tcp;

namespace aria {

    player::player(std::shared_ptr<player_callbacks> callbacks)
        : _callbacks(callbacks), _socket(_io_service)
    {

    }

    void player::connect_speaker(const std::string &host, uint16_t port)
    {
        tcp::resolver resolver(_io_service);
        tcp::resolver::query query(host, std::to_string(port));
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        boost::asio::connect(_socket, endpoint_iterator);
        _callbacks->connected();
    }

    void player::feed_audio(const char *data, size_t length)
    {
        boost::system::error_code error_code;
        _socket.write_some(asio::buffer(data, length), error_code);
    }

}
