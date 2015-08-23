#include <gtest/gtest.h>

#include <util/binary_buffer.hpp>
#include <cassert>

using aria::byte;

typedef std::vector<byte> bytevector;

namespace {
    template <typename T>
    std::vector<T> slice(const std::vector<T> &vec, size_t a, size_t b) {
        assert(a <= b);
        assert(b <= vec.size());
        return std::vector<T>(vec.cbegin() + a, vec.cbegin() + b);
    }
}

class binary_buffer_writer_test : public ::testing::Test {
protected:
    virtual void SetUp() {

    }

    aria::internal::binary_buffer_writer writer;
};

TEST_F(binary_buffer_writer_test, empty_upon_construction) {
    EXPECT_TRUE(writer.take_buffer().empty());
}

TEST_F(binary_buffer_writer_test, take_buffer_empties_buffer) {
    writer.write_bytes({ 2 });
    auto result = writer.take_buffer();
    ASSERT_FALSE(result.empty());
    ASSERT_TRUE(writer.take_buffer().empty());
}

TEST_F(binary_buffer_writer_test, write_uint8) {
    writer.write_uint8(0);
    writer.write_uint8(1);

    auto result = writer.take_buffer();
    EXPECT_EQ(bytevector({ 0, 1 }), result);
}

TEST_F(binary_buffer_writer_test, write_uint16) {
    writer.write_uint16(0);
    writer.write_uint16(1);

    auto result = writer.take_buffer();
    EXPECT_EQ(bytevector({ 0, 0, 0, 1 }), result);
}

TEST_F(binary_buffer_writer_test, write_uint32) {
    writer.write_uint32(0);
    writer.write_uint32(1);

    auto result = writer.take_buffer();
    EXPECT_EQ(bytevector({ 0, 0, 0, 0, 0, 0, 0, 1 }), result);
}

TEST_F(binary_buffer_writer_test, write_uint64) {
    writer.write_uint64(0);
    writer.write_uint64(1);

    auto result = writer.take_buffer();
    EXPECT_EQ(bytevector({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }), result);
}

TEST_F(binary_buffer_writer_test, write_empty_string) {
    writer.write_string("");

    auto result = writer.take_buffer();
    EXPECT_EQ(bytevector(4, 0), result);
}

TEST_F(binary_buffer_writer_test, write_string) {
    writer.write_string("string");

    auto result = writer.take_buffer();
    EXPECT_EQ(10, result.size());
    EXPECT_EQ(bytevector({0, 0, 0, 6}), slice(result, 0, 4));
    EXPECT_EQ("string", std::string(result.data() + 4, result.data() + 10));
}

TEST_F(binary_buffer_writer_test, write_empty_bytes) {
    writer.write_bytes({ });

    auto result = writer.take_buffer();
    EXPECT_EQ(4, result.size());
    EXPECT_EQ(bytevector({0, 0, 0, 0}), slice(result, 0, 4));
}

TEST_F(binary_buffer_writer_test, write_bytes) {
    writer.write_bytes({ 1, 2, 0, 3 });

    auto result = writer.take_buffer();
    EXPECT_EQ(8, result.size());
    EXPECT_EQ(bytevector({0, 0, 0, 4}), slice(result, 0, 4));
    EXPECT_EQ(bytevector({1, 2, 0, 3}), slice(result, 4, 8));
}
