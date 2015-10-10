#include <iostream>
#include <aria/speaker.hpp>
#include <memory>

namespace asio = boost::asio;

using boost::asio::ip::tcp;

class stdout_callbacks : public aria::speaker_callbacks
{
public:
    virtual void audio_received(const char *data, size_t length)
    {
        std::cout.write(data, length);
        std::cout.flush();
    }
};

int main()
{
   stdout_callbacks callbacks;
   aria::speaker speaker(callbacks);
}
