#include <array>
#include <iostream>

#include <aria/player.hpp>

int main()
{
    std::array<char, 8192> buffer;

    aria::player player(std::make_shared<aria::null_player_callbacks>());
    player.connect_speaker("localhost", 1582);

    while(true)
    {
        std::cin.read(buffer.data(), 8192);
        auto readcount = std::cin.gcount();
        player.feed_audio(buffer.data(), readcount);
    }

}
