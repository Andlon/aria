#include <boost/asio.hpp>
#include <array>
#include <iostream>

namespace asio = boost::asio;

using boost::asio::ip::tcp;

int main()
{
    asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1582));

    std::array<char, 8192> buffer;

    // Accept only a single connection at a time
    while (true)
    {
        tcp::socket socket(io_service);
        acceptor.accept(socket);
        boost::system::error_code error_code;

        while (error_code != boost::asio::error::eof) {
            size_t len = socket.read_some(asio::buffer(buffer), error_code);
            std::cout.write(buffer.data(), len);
        }
    }


}
