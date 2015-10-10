#include <boost/asio.hpp>
#include <array>
#include <iostream>

namespace asio = boost::asio;

using boost::asio::ip::tcp;

int main()
{
    asio::io_service io_service;
    tcp::socket socket(io_service);
    tcp::endpoint endpoint( asio::ip::address::from_string("127.0.0.1"), 1582);
    socket.connect(endpoint);

    std::cout << "Connected successfully to endpoint." << std::endl;

    std::array<char, 8192> buffer;

    while(true)
    {
        std::cin.read(buffer.data(), 8192);
        auto readcount = std::cin.gcount();

        boost::system::error_code error_code;
        socket.write_some(asio::buffer(buffer, readcount), error_code);
    }

}
