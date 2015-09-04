#pragma once

namespace aria {

    namespace internal {

        enum class event_type
        {
            PAUSE,
            STOP,
            BUFFERING_STARTED,
            FRAME_RECEIVED
        };

        class event {
        public:
            event(event_type type) : _type(type) { }
            virtual ~event() { }

            event_type type() const { return _type; }

        private:
            event_type _type;
        };

        class pause_event final : public event {
        public:
            pause_event() : event(event_type::PAUSE) { }
        };

        class stop_event final : public event {
        public:
            stop_event() : event(event_type::STOP) { }
        };

        class buffering_started_event final : public event {
        public:
            buffering_started_event() : event(event_type::BUFFERING_STARTED) { }
        };

        class frame_received_event final : public event {
        public:
            frame_received_event() : event(event_type::FRAME_RECEIVED) { }
        };

    }

}
