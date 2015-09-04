//
// Created by andreas on 8/19/15.
//

#pragma once

#include <vector>
#include <string>
#include <memory>
#include <aria/types.hpp>
#include <aria/frame.hpp>

namespace aria {

    namespace internal {

        enum class message_type {
            PAUSE,
            STOP,
            MEDIA_DESCRIPTOR,
            FRAME,
            SCHEDULE
        };

        class message {
        public:
            message_type type() const;

        protected:
            message(message_type type);

        private:
            message_type _type;
        };

        class pause_message final : public message {
        public:
            pause_message();
        };

        class stop_message final : public message {
        public:
            stop_message();
        };

        class frame_message final : public message {
        public:
            frame_message(frame_id id, std::vector<byte> && data);

            frame_id id() const;
            const std::vector<byte> & data() const;

            std::vector<byte> take_data();

        private:
            frame_id _id;
            std::vector<byte> _data;
        };

        class schedule_message final : public message {
        public:
            schedule_message(frame_id id, timestamp scheduled_time);

            frame_id id() const;
            timestamp scheduled_time() const;

        private:
            frame_id _id;
            timestamp _time;
        };

        class media_message final : public message {
        public:
            media_message(media_id id, const std::string & metadata);

            media_id id() const;
            const std::string & metadata() const;

        private:
            media_id _id;
            std::string _metadata;
        };

        std::vector<byte> encode_message(const message & message);

        std::shared_ptr<message> decode_message(const std::vector<byte> & bytes);

    } // namespace internal

} // namespace aria
