//
// Created by andreas on 8/19/15.
//

#include "message.hpp"
#include <util/binary_buffer.hpp>
#include <boost/endian/conversion.hpp>
#include <boost/bimap.hpp>

namespace aria {

    namespace internal {

        namespace {
            static const auto HEADER_MAP = [] {
                typedef boost::bimap<message_type, uint8_t> header_map;
                std::vector<header_map::value_type> entries {
                    { message_type::PAUSE, 0 },
                    { message_type::STOP, 1 },
                    { message_type::MEDIA_DESCRIPTOR, 2 },
                    { message_type::FRAME, 3 },
                    { message_type::SCHEDULE, 4 }
                };

                return header_map(entries.begin(), entries.end());
            }();

            uint8_t message_header_from_type(message_type type)
            {
                return HEADER_MAP.left.at(type);
            }

            message_type message_type_from_header(uint8_t header)
            {
                assert(header < 5);
                return HEADER_MAP.right.at(header);
            }
        }

        message::message(message_type type)
        {
            _type = type;
        }

        message_type message::type() const
        {
            return _type;
        }

        pause_message::pause_message() : message(message_type::PAUSE) { }

        stop_message::stop_message() : message(message_type::STOP) { }

        frame_message::frame_message(frame_id id, std::vector<byte> && data)
            : message(message_type::FRAME), _id(id), _data(std::move(data))
        {

        }

        frame_id frame_message::id() const
        {
            return _id;
        }

        const std::vector<byte> & frame_message::data() const {
            return _data;
        }

        std::vector<byte> frame_message::take_data() {
            return std::move(_data);
        }

        schedule_message::schedule_message(frame_id id, timestamp scheduled_time)
            : message(message_type::SCHEDULE)
        {
            _id = id;
            _time = scheduled_time;
        }

        frame_id schedule_message::id() const
        {
            return _id;
        }

        timestamp schedule_message::scheduled_time() const
        {
            return _time;
        }

        media_message::media_message(media_id id, const std::string &metadata)
            : message(message_type::MEDIA_DESCRIPTOR)
        {
            _id = id;
            _metadata = metadata;
        }

        media_id media_message::id() const
        {
            return _id;
        }

        const std::string &media_message::metadata() const
        {
            return _metadata;
        }

        std::vector<byte> encode_message(const message &message)
        {
            binary_buffer_writer writer;

            // Write 1-byte unsigned header that signifies message type
            writer.write_uint8(message_header_from_type(message.type()));

            switch(message.type())
            {
            case message_type::MEDIA_DESCRIPTOR:
            {
                auto media = static_cast<const media_message &>(message);
                writer.write_uint64(media.id());
                writer.write_string(media.metadata());
                break;
            }
            case message_type::FRAME:
            {
                const auto & frame = static_cast<const frame_message &>(message);
                writer.write_uint64(frame.id());
                writer.write_bytes(frame.data());
                break;
            }
            case message_type::SCHEDULE:
            {
                auto schedule = static_cast<const schedule_message &>(message);
                writer.write_uint64(schedule.id());
                writer.write_uint64(schedule.scheduled_time());
                break;
            }
            default:
                // Some messages require nothing more than the header
                break;
            }

            return writer.take_buffer();
        }

        std::shared_ptr<message> decode_message(const std::vector<byte> &bytes)
        {
            binary_buffer_reader reader(&bytes);
            auto header = reader.read_uint8();
            auto type = message_type_from_header(header);

            switch (type) {
            case message_type::PAUSE:
                return std::make_shared<pause_message>();
            case message_type::STOP:
                return std::make_shared<stop_message>();
            case message_type::MEDIA_DESCRIPTOR:
            {
                auto id = reader.read_uint64();
                auto metadata = reader.read_string();
                return std::make_shared<media_message>(id, metadata);
            }
            case message_type::FRAME:
            {
                auto id = reader.read_uint64();
                auto data = reader.read_bytes();
                return std::make_shared<frame_message>(id, std::move(data));
            }
            case message_type::SCHEDULE:
            {
                auto id = reader.read_uint64();
                auto scheduled_time = reader.read_uint64();
                return std::make_shared<schedule_message>(id, scheduled_time);
            }
            }
        }

    } // namespace internal

} // namespace aria
