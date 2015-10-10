#include <string>
#include <cstdint>
#include <memory>
#include <boost/asio.hpp>

namespace aria {

    class speaker_callbacks
    {
    public:
        virtual void audio_received(const char * data, size_t length) = 0;
    };

    class speaker
    {
    public:
        speaker(speaker_callbacks & callbacks);

    private:
        void accept();

        boost::asio::io_service _io_service;
        speaker_callbacks & _callbacks;
    };

}
