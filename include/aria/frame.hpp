#pragma once

#include <aria/types.hpp>
#include <vector>
#include <string>

namespace aria {

    class frame final {
    public:
        frame(frame_id id, std::vector<byte> && data);

        // Assume 2 channels
        // unsigned int channels() const;

        // Assume 44100 Hz
        // unsigned int sample_rate() const;

        // Assume linear PCM
        // const std::string & codec() const;

        frame_id id() const;

        const std::vector<byte> & data() const;

    private:
        frame_id _id;
        std::vector<byte> _data;
    };

}
