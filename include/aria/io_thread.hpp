#pragma once

#include <functional>
#include <thread>
#include <atomic>
#include <boost/asio.hpp>
#include <util/callable_queue.hpp>

namespace aria {

    class io_thread
    {
    public:
        io_thread();
        virtual ~io_thread();

        void start();
        void stop();
        void sync();

    protected:
        virtual void before_io_service() = 0;

        boost::asio::io_service & io_service();
        void run_on_main_thread(std::function<void()> && function);

    private:
        std::thread _io_thread;
        std::atomic_bool _should_run;

        boost::asio::io_service _io_service;
        callable_queue _sync_queue;
    };
}
