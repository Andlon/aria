#include <gtest/gtest.h>

#include <speaker/speaker_engine.hpp>
#include <util/unique_ptr_tools.hpp>

using namespace aria;
using namespace aria::internal;

class speaker_engine_test : public ::testing::Test {
protected:
    speaker_engine_test()
            :   queue(new event_queue),
                buffer(new frame_buffer)
    {
        auto postable = std::make_shared<postable_event_queue>(queue);
        engine.reset(new speaker_engine(postable, buffer));
    }

    virtual void SetUp() {

    }

    std::shared_ptr<event_queue> queue;
    std::shared_ptr<frame_buffer> buffer;
    std::unique_ptr<speaker_engine> engine;

    void frame_when_stopped_triggers_events_and_buffer() {
        const auto data = std::vector<byte>({ 'a', 'u', 'd', 'i', 'o' });
        auto msg = std::unique_ptr<message>(new frame_message(4, std::vector<byte>(data)));
        engine->receive_message(std::move(msg));

        ASSERT_FALSE(buffer->empty());
        auto frame = buffer->pop();
        EXPECT_EQ(4, frame.id());
        EXPECT_EQ(data, frame.data());
        EXPECT_TRUE(buffer->empty());
        // TODO: This will crash if the test fails, which is unfortunate. Fix
        EXPECT_EQ(event_type::BUFFERING_STARTED, queue->take()->type());
        EXPECT_EQ(event_type::FRAME_RECEIVED, queue->take()->type());
        EXPECT_TRUE(queue->take().get() == nullptr);
    }
};

TEST_F(speaker_engine_test, assert_initial_conditions) {
    ASSERT_TRUE(queue->take().get() == nullptr);
    ASSERT_TRUE(buffer->empty());
}

TEST_F(speaker_engine_test, frame_when_stopped_triggers_events_and_buffer) {
    frame_when_stopped_triggers_events_and_buffer();
}

TEST_F(speaker_engine_test, frame_when_buffering_triggers_events_and_buffer) {
    // Put the speaker into a known, working buffering state (from previous test)
    frame_when_stopped_triggers_events_and_buffer();

    const auto data = std::vector<byte>({ 'a', 'u', 'd', 'i', 'o' });
    auto msg = std::unique_ptr<message>(new frame_message(4, std::vector<byte>(data)));
    engine->receive_message(std::move(msg));

    ASSERT_FALSE(buffer->empty());
    auto frame = buffer->pop();
    EXPECT_EQ(4, frame.id());
    EXPECT_EQ(data, frame.data());
    EXPECT_TRUE(buffer->empty());
    // TODO: This will crash if the test fails, which is unfortunate. Fix
    EXPECT_EQ(event_type::FRAME_RECEIVED, queue->take()->type());
    EXPECT_TRUE(queue->take().get() == nullptr);
}

TEST_F(speaker_engine_test, stop_when_buffering_triggers_event) {
    // Put the speaker into a known, working buffering state (from previous test)
    frame_when_stopped_triggers_events_and_buffer();

    auto msg = std::unique_ptr<message>(new stop_message);
    engine->receive_message(std::move(msg));

    EXPECT_TRUE(buffer->empty());
    // TODO: This will crash if the test fails, which is unfortunate. Fix
    EXPECT_EQ(event_type::STOP, queue->take()->type());
    EXPECT_TRUE(queue->take().get() == nullptr);
}