#include "memory_delta_builder.h"

#include <cstring>

namespace rh {

void MemoryDeltaBuilder::add_existing_chunk(const ChunkInfo& chunk) {
    close_current_block();
    _blocks.emplace_back(chunk);
}

void MemoryDeltaBuilder::add_new_data(std::string_view data) {
    if (!data.empty()) {
        auto old_size = _current_new_data.size();
        _current_new_data.resize(old_size + data.size());
        std::memcpy(_current_new_data.data() + old_size, data.data(), data.size());
    }
}

std::vector<MemoryDeltaBuilder::block_data> MemoryDeltaBuilder::take_delta() {
    close_current_block();
    return std::move(_blocks);
}

void MemoryDeltaBuilder::close_current_block() {
    if (!_current_new_data.empty()) {
        _blocks.emplace_back(std::move(_current_new_data));
    }
}

} // namespace rh