#include <array>
#include <iostream>
#include <thread>

#include <aria/player.hpp>

int main()
{
    const size_t CHUNK_SIZE = 8192;

    std::vector<char> buffer;
    buffer.resize(CHUNK_SIZE);

    aria::player player(std::make_shared<aria::null_player_callbacks>());
    player.connect_speaker("localhost", 1582);
    player.start();

    while(true)
    {
        std::cin.read(buffer.data(), CHUNK_SIZE);
        auto readcount = std::cin.gcount();
        player.feed_audio(std::vector<char>(buffer.cbegin(), buffer.cbegin() + readcount));
        player.sync();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

}
