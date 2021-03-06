#include <gtest/gtest.h>

#include <util/binary_buffer.hpp>
#include <cassert>

using aria::byte;
using aria::internal::binary_buffer_reader;

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

class binary_buffer_reader_test : public ::testing::Test {

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
    EXPECT_EQ(bytevector({0, 0, 0, 0}), result);
}

TEST_F(binary_buffer_writer_test, write_bytes) {
    writer.write_bytes({ 1, 2, 0, 3 });

    auto result = writer.take_buffer();
    EXPECT_EQ(8, result.size());
    EXPECT_EQ(bytevector({0, 0, 0, 4}), slice(result, 0, 4));
    EXPECT_EQ(bytevector({1, 2, 0, 3}), slice(result, 4, 8));
}

TEST_F(binary_buffer_reader_test, read_uint8) {
    auto input = bytevector{ 0, 1 };
    auto reader = binary_buffer_reader(&input);

    auto a = reader.read_uint8();
    auto b = reader.read_uint8();
    EXPECT_EQ(0, a);
    EXPECT_EQ(1, b);
}

TEST_F(binary_buffer_reader_test, read_uint16) {
    auto input = bytevector{ 0, 0, 0, 1 };
    auto reader = binary_buffer_reader(&input);

    auto a = reader.read_uint16();
    auto b = reader.read_uint16();
    EXPECT_EQ(0, a);
    EXPECT_EQ(1, b);
}

TEST_F(binary_buffer_reader_test, read_uint32) {
    auto input = bytevector{ 0, 0, 0, 0, 0, 0, 0, 1 };
    auto reader = binary_buffer_reader(&input);

    auto a = reader.read_uint32();
    auto b = reader.read_uint32();
    EXPECT_EQ(0, a);
    EXPECT_EQ(1, b);
}

TEST_F(binary_buffer_reader_test, read_uint64) {
    auto input = bytevector{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
    auto reader = binary_buffer_reader(&input);

    auto a = reader.read_uint64();
    auto b = reader.read_uint64();
    EXPECT_EQ(0, a);
    EXPECT_EQ(1, b);
}

TEST_F(binary_buffer_reader_test, read_empty_string) {
    auto input = bytevector{ 0, 0, 0, 0 };
    auto reader = binary_buffer_reader(&input);

    auto str = reader.read_string();
    EXPECT_EQ("", str);
}

TEST_F(binary_buffer_reader_test, read_string) {
    bytevector input = bytevector{ 0, 0, 0, 6, 's', 't', 'r', 'i', 'n', 'g' };
    auto reader = binary_buffer_reader(&input);

    auto str = reader.read_string();
    EXPECT_EQ("string", str);
}

TEST_F(binary_buffer_reader_test, read_empty_bytes) {
    auto input = bytevector{ 0, 0, 0, 0 };
    auto reader = binary_buffer_reader(&input);

    auto data = reader.read_bytes();
    EXPECT_EQ(bytevector(), data);
}

TEST_F(binary_buffer_reader_test, read_bytes) {
    bytevector input = bytevector{ 0, 0, 0, 4, 'd', 'a', 't', 'a' };
    auto reader = binary_buffer_reader(&input);

    auto data = reader.read_bytes();
    EXPECT_EQ(bytevector({'d', 'a', 't', 'a'}), data);
}
