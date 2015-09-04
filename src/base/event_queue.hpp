#pragma once

#include "event.hpp"
#include <memory>
#include <mutex>
#include <queue>

namespace aria {

    namespace internal {

        class postable_event_queue;

        class event_queue final {
        public:

            void post(std::unique_ptr<const event> && event);
            std::unique_ptr<const event> take();

            bool empty() const;

        private:
            mutable std::mutex _mutex;
            std::queue< std::unique_ptr<const event> > _queue;
        };

        class postable_event_queue {
        public:
            postable_event_queue(std::shared_ptr<event_queue> queue);

            void post(std::unique_ptr<const event> && event);

        private:
            std::shared_ptr<event_queue> _queue;
        };

    }

}
