#include <gtest/gtest.h>

#include <base/message.hpp>

using aria::byte;

typedef std::vector<byte> bytevector;

namespace aria {

    namespace internal {

        TEST(encode_message, encodes_pause) {
            pause msg;

            auto result = encode_message(msg);
            EXPECT_EQ(bytevector({ 0 }), result);
        }

        TEST(encode_message, encodes_stop) {
            stop msg;

            auto result = encode_message(msg);
            EXPECT_EQ(bytevector({ 1 }), result);
        }

        TEST(encode_message, encodes_media_descriptor) {
            media_descriptor msg(1, "data");

            auto result = encode_message(msg);
            bytevector expected =
            {
                2,                      // header
                0, 0, 0, 0, 0, 0, 0, 1, // id
                0, 0, 0, 4,             // string size
                'd', 'a', 't', 'a'      // string
            };
            EXPECT_EQ(expected, result);
        }

        TEST(encode_message, encodes_frame_data) {
            frame_data msg(1, 1, { 'd', 'a', 't', 'a' });

            auto result = encode_message(msg);
            bytevector expected =
            {
                3,                        // header
                0, 0, 0, 0, 0, 0, 0, 1,   // id
                0, 0, 0, 0, 0, 0, 0, 1,   // media
                0, 0, 0, 4,               // data size
                'd', 'a', 't', 'a'        // data
            };
            EXPECT_EQ(expected, result);
        }

        TEST(encode_message, encodes_frame_schedule) {
            frame_schedule msg(1, 2500);

            auto result = encode_message(msg);
            bytevector expected =
            {
                4,                        // header
                0, 0, 0, 0, 0, 0, 0, 1,   // id
                0, 0, 0, 0, 0, 0, 9, 196, // timestamp
            };
            EXPECT_EQ(expected, result);
        }

        TEST(decode_message, decodes_pause) {
            bytevector bytes { 0 };

            auto result = decode_message(bytes);
            ASSERT_EQ(message_type::PAUSE, result->type());
        }

        TEST(decode_message, decodes_stop) {
            bytevector bytes { 1 };

            auto result = decode_message(bytes);
            ASSERT_EQ(message_type::STOP, result->type());
        }

        TEST(decode_message, decodes_media_descriptor) {
            bytevector bytes =
            {
                2,                      // header
                0, 0, 0, 0, 0, 0, 0, 1, // id
                0, 0, 0, 4,             // string size
                'd', 'a', 't', 'a'      // string
            };

            auto result = decode_message(bytes);
            ASSERT_EQ(message_type::MEDIA_DESCRIPTOR, result->type());

            auto msg = std::static_pointer_cast<media_descriptor>(result);
            EXPECT_EQ(1, msg->id());
            EXPECT_EQ("data", msg->metadata());
        }

        TEST(decode_message, decodes_frame_data) {
            bytevector bytes =
            {
                3,                        // header
                0, 0, 0, 0, 0, 0, 0, 1,   // id
                0, 0, 0, 0, 0, 0, 0, 1,   // media
                0, 0, 0, 4,               // data size
                'd', 'a', 't', 'a'        // data
            };

            auto result = decode_message(bytes);
            ASSERT_EQ(message_type::FRAME_DATA, result->type());

            auto msg = std::static_pointer_cast<frame_data>(result);
            EXPECT_EQ(1, msg->id());
            EXPECT_EQ(1, msg->media());
            EXPECT_EQ(bytevector({ 'd', 'a', 't', 'a' }), msg->data());
        }

        TEST(decode_message, decodes_frame_schedule) {
            bytevector bytes =
            {
                4,                        // header
                0, 0, 0, 0, 0, 0, 0, 1,   // id
                0, 0, 0, 0, 0, 0, 9, 196, // timestamp
            };

            auto result = decode_message(bytes);
            ASSERT_EQ(message_type::FRAME_SCHEDULE, result->type());

            auto msg = std::static_pointer_cast<frame_schedule>(result);
            EXPECT_EQ(1, msg->id());
            EXPECT_EQ(2500, msg->scheduled_time());
        }
    }

}
