#include <aria/io_thread.hpp>
#include <iostream>

namespace aria {

    void io_thread::start()
    {
        if (!_io_thread.joinable())
        {
            _io_thread = std::thread([this] () {
                before_io_service();
                _io_service.run();
                std::cerr << "IO Service aborted." << std::endl;
            });
        }
    }

    void io_thread::sync()
    {
        _sync_queue.run_all();
    }

    boost::asio::io_service &io_thread::io_service()
    {
        return _io_service;
    }

    void io_thread::run_on_main_thread(std::function<void ()> &&function)
    {
        _sync_queue.post(std::move(function));
    }

}
