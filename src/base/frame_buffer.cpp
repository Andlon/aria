#include "frame_buffer.hpp"

void aria::internal::frame_buffer::push(aria::frame &&frame) {
    _fifo.push(std::move(frame));
}

aria::frame aria::internal::frame_buffer::pop() {
    auto front = std::move(_fifo.front());
    _fifo.pop();
    return front;
}

bool aria::internal::frame_buffer::empty() const {
    return _fifo.empty();
}
