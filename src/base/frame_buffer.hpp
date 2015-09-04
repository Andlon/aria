#pragma once

#include <aria/frame.hpp>
#include <queue>

namespace aria {

    namespace internal {

        class frame_buffer {
        public:

            void push(aria::frame && frame);
            bool empty() const;
            aria::frame pop();

        private:
            std::queue<aria::frame> _fifo;
        };

    }

}
