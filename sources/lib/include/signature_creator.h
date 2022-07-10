#pragma once

#include "signature.h"
#include "data_reader.h"

namespace rh {

template <typename WeakHasher, typename StrongHasher>
Signature generate_signature(DataReader& input, WeakHasher weak_hash, StrongHasher strong_hash, const uint64_t chunk_size) {
    uint64_t offset = 0;
    Signature result;
    for (auto data = input.read(chunk_size); !data.empty(); data = input.read(chunk_size)) {
        const auto weak_hash = weak_hash(data);
        const auto strong_hash = strong_hash(data);
        result.add_chunk(weak_hash, strong_hash, {offset, data.size()});
        offset += data.size();
    }

    return result;
}

} // namespace rh