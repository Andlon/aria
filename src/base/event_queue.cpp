#include "event_queue.hpp"

namespace aria {

    namespace internal {

        void event_queue::post(std::unique_ptr<const event> &&event)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _queue.push(std::move(event));
        }

        std::unique_ptr<const event> event_queue::take()
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_queue.empty())
            {
                return std::unique_ptr<const event>();
            }
            else
            {
                auto event = std::move(_queue.front());
                _queue.pop();
                return std::move(event);
            }
        }


        bool event_queue::empty() const
        {
            std::lock_guard<std::mutex> lock(_mutex);
            return _queue.empty();
        }

        postable_event_queue::postable_event_queue(std::shared_ptr<event_queue> queue)
            :   _queue(queue)
        {

        }

        void postable_event_queue::post(std::unique_ptr<const event> &&event)
        {
            _queue->post(std::move(event));
        }

    }

}
