#include <aria/io_thread.hpp>
#include <iostream>

namespace aria {

    io_thread::io_thread()
        :   _should_run(false)
    {

    }

    io_thread::~io_thread()
    {
        stop();
    }

    void io_thread::start()
    {
        if (!_should_run)
        {
            _should_run = true;
            _io_thread = std::thread([this] ()
            {
                before_io_service();
                do {
                    _io_service.run();
                    _io_service.reset();
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                } while (_should_run);

                std::cerr << "IO Service aborted." << std::endl;
            });
        }

    }

    void io_thread::stop()
    {
        _should_run = false;
        _io_service.stop();
        _io_thread.join();
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
