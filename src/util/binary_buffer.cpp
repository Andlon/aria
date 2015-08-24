#include "binary_buffer.hpp"

#include <iterator>
#include <algorithm>
#include <sstream>
#include <boost/endian/conversion.hpp>

using boost::endian::native_to_big;
using boost::endian::big_to_native;

namespace {
    using aria::byte;

    template <typename P>
    void append_bytes_to_vector(std::vector<byte> & vec, P primitive)
    {
        auto * begin = reinterpret_cast<byte *>(&primitive);
        auto * end = begin + sizeof(primitive);
        std::copy(begin, end, std::back_inserter(vec));
    }

    template <typename P>
    P read_primitive_and_advance(const byte * buffer, size_t size, size_t & offset, const std::string & name)
    {
        size_t stride = sizeof(P);
        if (offset + stride <= size) {
            auto i = reinterpret_cast<const P *>(buffer + offset);
            offset += stride;
            return big_to_native(*i);
        } else {
            throw aria::internal::buffer_error("Insufficient bytes available to read " + name + ".");
        }
    }
}

aria::internal::buffer_error::buffer_error(const char *what)
    :   std::runtime_error(what)
{

}

aria::internal::buffer_error::buffer_error(const std::string &what)
    :   std::runtime_error(what)
{

}

void aria::internal::binary_buffer_writer::write_uint8(uint8_t i)
{
    _bytes.push_back(static_cast<byte>(i));
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
        _bytes.push_back(static_cast<byte>(c));
    }
}

void aria::internal::binary_buffer_writer::write_bytes(const std::vector<aria::byte> &bytes)
{
    write_uint32(bytes.size());
    std::copy(bytes.begin(), bytes.end(), std::back_inserter(_bytes));
}

std::vector<aria::byte> aria::internal::binary_buffer_writer::take_buffer()
{
    std::vector<byte> buffer;
    _bytes.swap(buffer);
    return buffer;
}

aria::internal::binary_buffer_reader::binary_buffer_reader(const std::vector<byte> * buffer)
    :   _buffer_start(buffer->data()), _buffer_size(buffer->size()), _offset(0)
{

}

uint8_t aria::internal::binary_buffer_reader::read_uint8()
{
    return read_primitive_and_advance<uint8_t>(_buffer_start, _buffer_size, _offset, "uint8");
}

uint16_t aria::internal::binary_buffer_reader::read_uint16()
{
    return read_primitive_and_advance<uint16_t>(_buffer_start, _buffer_size, _offset, "uint16");
}

uint32_t aria::internal::binary_buffer_reader::read_uint32()
{
    return read_primitive_and_advance<uint32_t>(_buffer_start, _buffer_size, _offset, "uint32");
}

uint64_t aria::internal::binary_buffer_reader::read_uint64()
{
    return read_primitive_and_advance<uint64_t>(_buffer_start, _buffer_size, _offset, "uint64");
}

std::string aria::internal::binary_buffer_reader::read_string()
{
    uint32_t size;
    try {
        size = read_uint32();
    } catch (buffer_error) {
        throw buffer_error("Insufficient bytes available to read string size.");
    }

    if (_offset + size <= _buffer_size) {
        auto data = reinterpret_cast<const char *>(_buffer_start + _offset);
        _offset += size;
        return std::string(data, size);;
    } else {
        assert(_offset <= _buffer_size);
        auto available = _buffer_size - _offset;
        std::stringstream ss;
        ss << "Expected " << size << " bytes of string data, but only " << available
           << " available bytes in buffer.";
        throw buffer_error(ss.str());
    }
}

std::vector<byte> aria::internal::binary_buffer_reader::read_bytes()
{
    uint32_t size;
    try {
        size = read_uint32();
    } catch (buffer_error) {
        throw buffer_error("Insufficient bytes available to read data size.");
    }

    if (_offset + size <= _buffer_size) {
        auto data = _buffer_start + _offset;
        _offset += size;
        return std::vector<byte>(data, data + size);
    } else {
        assert(_offset <= _buffer_size);
        auto available = _buffer_size - _offset;
        std::stringstream ss;
        ss << "Expected " << size << " bytes of data, but only " << available
           << " available bytes in buffer.";
        throw buffer_error(ss.str());
    }
}
