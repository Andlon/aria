#include <vector>
#include <mutex>

namespace aria {

    class callable_queue {
    public:

        template <typename Callable>
        void post(Callable && callable);

        void run_all();

    private:
        std::vector< std::function<void()> > _queue;
        mutable std::mutex _mutex;
    };

    template <typename Callable>
    inline void callable_queue::post(Callable && callable)
    {
        std::lock_guard<std::mutex> guard(_mutex);
        _queue.push_back(std::move(callable));
    }

    inline void callable_queue::run_all()
    {
        std::vector<std::function<void()> > local_queue;

        {
            std::lock_guard<std::mutex> guard(_mutex);
            local_queue.swap(_queue);
        }

        for (auto & func : local_queue)
            func();
    }

}
