#include <aria/frame.hpp>


aria::frame::frame(aria::frame_id id, std::vector<aria::byte> &&data)
        : _id(id), _data(data)
{

}

const std::vector<aria::byte> &aria::frame::data() const
{
        return _data;
}

aria::frame_id aria::frame::id() const
{
    return _id;
}

