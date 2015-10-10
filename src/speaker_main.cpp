#include <iostream>
#include <aria/speaker.hpp>
#include <memory>
#include <thread>
#include <chrono>

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

    void player_connected()
    {
        std::cerr << "Player connected." << std::endl;
    }

    void player_disconnected()
    {
        std::cerr << "Player disconnected." << std::endl;
    }
};

int main()
{
   stdout_callbacks callbacks;
   aria::speaker speaker(callbacks);
   speaker.start();

   while (true)
   {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        speaker.process();
   }
}
