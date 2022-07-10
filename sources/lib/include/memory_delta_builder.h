#pragma once

#include <string_view>
#include <variant>
#include <vector>

#include "signature.h"

namespace rh {

class MemoryDeltaBuilder {
public:
    using block_data = std::variant<ChunkInfo, std::vector<char>>;

public:
    void add_existing_chunk(const ChunkInfo& chunk);
    void add_new_data(std::string_view data);

    std::vector<block_data> take_delta();

private:
    void close_current_block();

private:
    std::vector<block_data> _blocks;
    std::vector<char> _current_new_data;
};

} // namespace rh