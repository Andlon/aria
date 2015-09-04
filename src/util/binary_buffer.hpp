#include <aria/types.hpp>
#include <string>
#include <vector>
#include <stdexcept>

namespace aria {

    namespace internal {

        class buffer_error : public std::runtime_error {
        public:
            buffer_error(const char * what);
            buffer_error(const std::string & what);
        };

        /**
         * @brief Simplifies writing C++ data types to the binary format used by
         * Aria's network protocol.
         *
         * Specifically, the class uses the following semantics:
         * - All numeric types are encoded in big-endian byte order.
         * - Raw binary data is prefixed by a 32-bit unsigned integer specifying its length.
         * - Strings must be UTF-8 encoded and are prefixed by a 32-bit unsigned integer specifying its length,
         *   followed by 1 byte per character.
         */
        class binary_buffer_writer final {
        public:

            void write_uint8(uint8_t i);
            void write_uint16(uint16_t i);
            void write_uint32(uint32_t i);
            void write_uint64(uint64_t i);
            void write_string(const std::string & str);
            void write_bytes(const std::vector<byte> & bytes);

            std::vector<byte> take_buffer();

        private:
            std::vector<byte> _bytes;
        };

        /**
         * @brief Simplifies reading C++ data types from the binary format used by
         * Aria's network protocol.
         *
         * Specifically, the class uses the following semantics:
         * - All numeric types are encoded in big-endian byte order.
         * - Raw binary data is prefixed by a 32-bit unsigned integer specifying its length.
         * - Strings must be UTF-8 encoded and are prefixed by a 32-bit unsigned integer specifying its length,
         *   followed by 1 byte per character.
         */
        class binary_buffer_reader final {
        public:
            binary_buffer_reader(const std::vector<byte> * byte);

            uint8_t read_uint8();
            uint16_t read_uint16();
            uint32_t read_uint32();
            uint64_t read_uint64();
            std::string read_string();
            std::vector<byte> read_bytes();

        private:
            const byte * _buffer_start;
            const size_t _buffer_size;
            size_t _offset;
        };

    }

}
