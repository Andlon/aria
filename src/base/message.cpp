//
// Created by andreas on 8/19/15.
//

#include "message.hpp"
#include <boost/endian/conversion.hpp>

namespace aria {

    namespace internal {

        message::message(message_type type)
        {
            _type = type;
        }

        message_type message::type() const
        {
            return _type;
        }

        pause::pause() : message(message_type::PAUSE) { }

        stop::stop() : message(message_type::STOP) { }

        frame_data::frame_data(frame_id id, media_id media, std::vector<uint8_t> &&data)
            : message(message_type::FRAMEDATA)
        {
            _id = id;
            _media = media;
            _data = data;
        }


        frame_id frame_data::id() const
        {
            return _id;
        }

        media_id frame_data::media() const
        {
            return _media;
        }

        const std::vector<uint8_t> &frame_data::data() const
        {
            return _data;
        }

        frame_schedule::frame_schedule(frame_id id, timestamp scheduled_time)
            : message(message_type::FRAMESCHEDULE)
        {
            _id = id;
            _time = scheduled_time;
        }

        frame_id frame_schedule::id() const
        {
            return _id;
        }

        timestamp frame_schedule::scheduled_time() const
        {
            return _time;
        }

        media::media(media_id id, const std::string &metadata)
            : message(message_type::MEDIA)
        {
            _id = id;
            _metadata = metadata;
        }

        media_id media::id() const
        {
            return _id;
        }

        const std::string &media::metadata() const
        {
            return _metadata;
        }

        std::vector<byte> encode_message(const message &message)
        {

        }

        std::shared_ptr<message> decode_message(const std::vector<byte> &bytes)
        {

        }

    } // namespace internal

} // namespace aria
