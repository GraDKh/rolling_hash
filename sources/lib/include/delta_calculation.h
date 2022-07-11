#pragma once

#include <memory>

#include "signature.h"
#include "data_reader.h"

namespace rh {

// Main method to provide delta calculation based on the new file and existing signature.
// `signature` - precalculated signature of the existing file.
// `new_file` - data reader for the new file.
// `rolling_hasher` - hasher to calculate weak rolling hash.
// `strong_hasher` - secondary hash to avoid collisions of the weak hash.
// `builder` - a class that build delta on the fly. It's passed here as an abstraction
// because:
// 1) In-memory builder is handy for testing.
// 2) When we are writing to the file we don't need to store the delta in memory.
// 3) In real life we can also need to send the diff via network and it would be
// more efficient to do it on the fly.
template <typename DeltaBuilder, typename RollingHasher, typename StrongHasher>
void build_delta(const Signature& signature, DataReader& new_file,
    RollingHasher rolling_hasher, StrongHasher strong_hasher, DeltaBuilder& builder) {
    // find the existing chunk in the signature
    const auto find_chunk =
        [&](std::string_view part1, std::string_view part2) -> const ChunkInfo* {
            const auto weak_hash = rolling_hasher.get_value();
            if (const auto* chunks_by_strong = signature.get_by_weak_hash(weak_hash)) {
                strong_hasher.init(part1);
                if (!part2.empty()) {
                    strong_hasher.add(part2);
                }
                const auto strong_hash = strong_hasher.get_value();
                if (const auto chunkIt = chunks_by_strong->find(strong_hash); chunkIt != chunks_by_strong->end()) {
                    return &chunkIt->second;
                }
            }

            return nullptr;
        };

    // Read the first block
    const auto chunk_size = signature.get_chunk_size();
    const auto buff1 = std::make_unique<char[]>(chunk_size);
    auto* current_buf = buff1.get();
    size_t current_block_size;
    std::string_view current_chunk;
    const auto read_current_chunk = [&]() {
        current_block_size = new_file.read(current_buf, chunk_size);
        current_chunk = {current_buf, current_block_size};
        rolling_hasher.init(current_chunk);
    };
    read_current_chunk();

    const auto buff2 = std::make_unique<char[]>(chunk_size);
    auto* next_buf = buff2.get();
    while (true) {
        // check if we have some data in the current block
        if (current_block_size == 0) {
            return;
        }

        // check if our current buffer corresponds to some chunk
        if (const auto* chunk = find_chunk(current_chunk, {})) {
            builder.add_existing_chunk(*chunk);
            read_current_chunk();
            continue;
        }

        // load the next chunk and start moving window
        const auto next_block_size = new_file.read(next_buf, chunk_size);
        if (next_block_size == 0) {
            builder.add_new_data(current_chunk);
            break;
        }

        bool chunk_found = false;
        const auto roll_count = std::min(next_block_size, chunk_size - 1);
        for (size_t i = 0; i < roll_count; ++i) {
            rolling_hasher.remove_old(current_buf[i]);
            rolling_hasher.add_new(next_buf[i]);
            if (const auto* chunk = find_chunk({current_buf + i + 1, current_block_size - i - 1}, {next_buf, i + 1})) {
                // add data to delta builder
                builder.add_new_data({current_buf, i + 1});
                builder.add_existing_chunk(*chunk);

                // load the remaining bytes to keep is of chunk_size max
                const auto remaining_bytes = next_block_size - i - 1;
                std::memcpy(current_buf, next_buf + i + 1, remaining_bytes);
                current_block_size = i + new_file.read(current_buf + remaining_bytes, chunk_size - remaining_bytes);
                current_chunk = {current_buf, current_block_size};
                rolling_hasher.init(current_chunk);
                chunk_found = true;
                break;
            }
        }
        if (chunk_found) {
            break;
        }

        builder.add_new_data(current_chunk);
        current_chunk = {next_buf, next_block_size};
        if (next_block_size < chunk_size) {
            // in case when next block is shorter than chunk_size
            // need to fully recalculate weak hash
            rolling_hasher.init(current_chunk);
        } else {
            // in other case we can do a roll
            rolling_hasher.remove_old(current_buf[chunk_size - 1]);
            rolling_hasher.add_new(next_buf[chunk_size - 1]);
        }
        std::swap(current_buf, next_buf);
        current_block_size = next_block_size;
    }
}

} // namespace rh
