#pragma once

#include <aria/frame.hpp>
#include <base/event_queue.hpp>
#include <base/message.hpp>
#include <base/frame_buffer.hpp>
#include <memory>

namespace aria {

    namespace internal {

        class speaker_engine final {
        public:
            explicit speaker_engine(std::shared_ptr<postable_event_queue> queue,
                                    std::shared_ptr<frame_buffer> buffer);
            ~speaker_engine();

            void receive_message(std::unique_ptr<message> && message);

        private:
            // PIMPL to avoid propagating boost MSM past compilation unit
            struct fsm_wrapper;
            fsm_wrapper * _wrapper;
        };

    }

}
