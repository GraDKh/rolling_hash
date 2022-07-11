#pragma once

#include <string_view>

#include "data_reader.h"
#include "memory.h"
#include "signature.h"

namespace rh {

// Creates signature objects based on the given input.
// `input` - data source to generate signature for.
// `weak_hasher` - object that provides a weak hash value for each chunk.
// `strong_hasher`- object that provides a strong hash value for each chunk.
// `chunk_size` - size of the chunk to split data from the input.
template <typename WeakHasher, typename StrongHasher>
Signature generate_signature(DataReader& input, WeakHasher weak_hasher,
    StrongHasher strong_hasher, const uint64_t chunk_size) {
    uint64_t offset = 0;
    Signature result{chunk_size};
    auto buffer = std::make_unique<char[]>(chunk_size);
    size_t bytes_read = 0;
    const auto read_next = [&]() { 
        bytes_read = input.read(buffer.get(), chunk_size);
    };
    for (read_next(); bytes_read != 0; read_next()) {
        const std::string_view data = {buffer.get(), bytes_read};
        const auto weak_hash = weak_hasher(data);
        const auto strong_hash = strong_hasher(data);
        result.add_chunk(weak_hash, strong_hash, {offset, bytes_read});
        offset += bytes_read;
    }

    return result;
}

} // namespace rh