#include <aria/player.hpp>
#include <string>
#include <cstddef>
#include <iostream>

namespace asio = boost::asio;

using boost::asio::ip::tcp;
using boost::system::error_code;

namespace aria {

    player::player(std::shared_ptr<player_callbacks> callbacks)
        : _callbacks(callbacks), _resolver(io_service())
    {

    }

    void player::connect_speaker(const std::string &host, uint16_t port)
    {
        typedef tcp::resolver::iterator iterator;
        tcp::resolver::query query(host, std::to_string(port));
        _resolver.async_resolve(query, [this] (const error_code & error,
                                iterator endpoint_iterator) {
            if (!error)
            {
                auto socket = std::make_shared<tcp::socket>(io_service());
                boost::asio::async_connect(*socket, endpoint_iterator,
                                           [this, socket] (const error_code & error,
                                           iterator endpoint)
                {
                    iterator end;
                    if (!error && endpoint != end)
                    {
                        run_on_main_thread([=] {
                            _sockets.push_back(socket);
                            _callbacks->connected();
                        });
                    }
                    else
                    {
                        run_on_main_thread([this] { _callbacks->connection_failed(); });
                    }
                });
            }
            else
            {
                // TODO: error handling when resolution fails
            }
        });
    }

    void player::feed_audio(std::vector<char> && data)
    {
        if (!data.empty())
        {
            // Create shared pointer to data to ensure that the data lives
            // until the async_write completion handler is run
            auto shared_data = std::make_shared<std::vector<char>>(std::move(data));
            for (auto socket : _sockets)
            {
                asio::async_write(*socket, asio::buffer(*shared_data),
                                  [shared_data] (const class error_code &error,
                                  std::size_t)
                {
                    if (error)
                    {
                        // TODO: Proper error handling
                        std::cerr << "Error occured when writing to socket." << std::endl;
                    }
                });
            }
        }
    }

    void player::before_io_service()
    {
        // Do nothing for now
    }

}
