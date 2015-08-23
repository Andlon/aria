#include <base/types.hpp>
#include <string>
#include <vector>

namespace aria {

    namespace internal {

        /**
         * @brief Simplifies writing C++ data types to the binary format used by
         * Aria's network protocol.
         *
         * Specifically, the class uses the following semantics:
         * - All numeric types are encoded in big-endian byte order
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
         * - All numeric types are encoded in big-endian byte order
         * - Strings must be UTF-8 encoded and are prefixed by a 32-bit unsigned integer specifying its length,
         *   followed by 1 byte per character.
         */
        class binary_buffer_reader final {
        public:

        private:

        };

    }

}
