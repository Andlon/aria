#include <aria/speaker.hpp>
#include <boost/asio.hpp>
#include <array>
#include <iostream>

#include <util/unique_ptr_tools.hpp>

namespace asio = boost::asio;

using asio::ip::tcp;
using boost::system::error_code;

namespace aria {

    speaker::speaker(speaker_callbacks & callbacks)
        :   _callbacks(callbacks), _acceptor(io_service(), tcp::endpoint(tcp::v4(), 1582))
    {

    }

    void speaker::before_io_service()
    {
        accept();
    }

    void speaker::accept()
    {
        if (!_connection)
        {
            auto socket = std::make_shared<tcp::socket>(io_service());
            _acceptor.async_accept(*socket,
                                   [=] (const error_code& error) {
                if (!error)
                {
                    run_on_main_thread([this] () {
                        _callbacks.player_connected();
                    });

                    auto on_audio_received = [this] (std::vector<char> && data) {
                        run_on_main_thread([this, data] () {
                            _callbacks.audio_received(data.data(), data.size());
                        });
                    };

                    auto on_disconnection = [this] (const error_code & error) {
                        run_on_main_thread([this] () {
                            _callbacks.player_disconnected();
                        });
                        _connection = nullptr;
                        accept();
                    };

                    _connection = speaker_connection::create(std::move(*socket));
                    _connection->async_read(on_audio_received, on_disconnection);
                }
                else
                {
                    accept();
                }
            });
        }
    }
}
