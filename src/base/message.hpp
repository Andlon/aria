//
// Created by andreas on 8/19/15.
//

#pragma once

#include <vector>
#include <string>
#include <memory>
#include "types.hpp"

namespace aria {

    namespace internal {

        enum class message_type {
            PAUSE,
            STOP,
            MEDIA_DESCRIPTOR,
            FRAME_DATA,
            FRAME_SCHEDULE,
        };

        class message {
        public:
            message_type type() const;

        protected:
            message(message_type type);

        private:
            message_type _type;
        };

        class pause final : public message {
        public:
            pause();
        };

        class stop final : public message {
        public:
            stop();
        };

        class frame_data final : public message {
        public:
            frame_data(frame_id id, media_id media, const std::vector<uint8_t> & data);

            frame_id id() const;
            media_id media() const;
            const std::vector<uint8_t> & data() const;

        private:
            frame_id _id;
            media_id _media;
            std::vector<uint8_t> _data;
        };

        class frame_schedule final : public message {
        public:
            frame_schedule(frame_id id, timestamp scheduled_time);

            frame_id id() const;
            timestamp scheduled_time() const;

        private:
            frame_id _id;
            timestamp _time;
        };

        class media_descriptor final : public message {
        public:
            media_descriptor(media_id id, const std::string & metadata);

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
