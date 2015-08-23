//
// Created by andreas on 8/19/15.
//

#ifndef ARIA_MESSAGE_H
#define ARIA_MESSAGE_H

#include <cstdint>
#include <vector>
#include <string>
#include <ostream>

namespace aria {

    typedef uint64_t frame_id;
    typedef uint64_t media_id;
    typedef uint64_t timestamp;

    enum class message_type {
        PAUSE,
        STOP,
        MEDIA,
        FRAMEDATA,
        FRAMESCHEDULE,
    };

    class message {
    public:
        message_type type() const;

    protected:
        message(message_type type);

    private:
        message_type _type;
    };

    class pause final : message {
    public:
        pause();
    };

    class stop final : message {
    public:
        stop();
    };

    class frame_data final : message {
    public:
        frame_data(frame_id id, media_id media, std::vector<uint8_t> && data);

        frame_id id() const;
        media_id media() const;
        const std::vector<uint8_t> & data() const;

    private:
        frame_id _id;
        media_id _media;
        std::vector<uint8_t> _data;
    };

    class frame_schedule final : message {
    public:
        frame_schedule(frame_id id, timestamp scheduled_time);

        frame_id id() const;
        timestamp scheduled_time() const;

    private:
        frame_id _id;
        timestamp _time;
    };

    class media final : message {
    public:
        media(media_id id, const std::string & metadata);

        media_id id() const;
        const std::string & metadata() const;

    private:
        media_id _id;
        std::string _metadata;
    };
}


#endif //ARIA_MESSAGE_H
