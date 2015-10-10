#include <memory>
#include <vector>
#include <boost/asio.hpp>

namespace aria {

    class speaker_connection : public std::enable_shared_from_this<speaker_connection>
    {
    public:
        static std::shared_ptr<speaker_connection> create(boost::asio::ip::tcp::socket && socket);

        void async_read(std::function<void(std::vector<char> &&)> audio_received_func,
                   std::function<void(const boost::system::error_code&)> disconnected_func);

    private:
        explicit speaker_connection(boost::asio::ip::tcp::socket && socket);

        boost::asio::ip::tcp::socket _socket;
        std::vector<char> _buffer;
    };

}
