#include "binary_buffer.hpp"

#include <iterator>
#include <algorithm>
#include <boost/endian/conversion.hpp>

using boost::endian::native_to_big;

namespace aria {
    namespace internal {
        namespace {
            template <typename P>
            void append_bytes_to_vector(std::vector<byte> & vec, P primitive) {
                byte * begin = (byte *) &primitive;
                byte * end = begin + sizeof(primitive);
                std::copy(begin, end, std::back_inserter(vec));
            }
        }
    }
}

void aria::internal::binary_buffer_writer::write_uint8(uint8_t i)
{
    uint8_t converted = native_to_big(i);
    _bytes.push_back(converted);
}

void aria::internal::binary_buffer_writer::write_uint16(uint16_t i)
{
    append_bytes_to_vector(_bytes, native_to_big(i));
}

void aria::internal::binary_buffer_writer::write_uint32(uint32_t i)
{
    append_bytes_to_vector(_bytes, native_to_big(i));
}

void aria::internal::binary_buffer_writer::write_uint64(uint64_t i)
{
    append_bytes_to_vector(_bytes, native_to_big(i));
}

void aria::internal::binary_buffer_writer::write_string(const std::string &str)
{
    write_uint32(str.size());

    for (auto c : str) {
        _bytes.push_back((byte) c);
    }
}

void aria::internal::binary_buffer_writer::write_bytes(const std::vector<aria::byte> &bytes)
{
    std::copy(bytes.begin(), bytes.end(), std::back_inserter(_bytes));
}

std::vector<aria::byte> aria::internal::binary_buffer_writer::take_buffer()
{
    std::vector<byte> buffer;
    _bytes.swap(buffer);
    return buffer;
}
