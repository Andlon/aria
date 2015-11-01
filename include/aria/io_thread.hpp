#pragma once

#include <functional>
#include <thread>
#include <boost/asio.hpp>
#include <util/callable_queue.hpp>

namespace aria {

    class io_thread
    {
    public:
        virtual ~io_thread() {}

        void start();
        void sync();

    protected:
        virtual void before_io_service() = 0;

        boost::asio::io_service & io_service();
        void run_on_main_thread(std::function<void()> && function);

    private:
        boost::asio::io_service _io_service;
        std::thread _io_thread;
        callable_queue _sync_queue;
    };
}
