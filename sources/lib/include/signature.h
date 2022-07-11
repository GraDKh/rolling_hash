#pragma once

#include <optional>
#include <unordered_map>

#include "types.h"
#include "hash_utils.h"

namespace rh {

// Chunk info for the each chunk.
// TODO: It can be optimized to be of 64-bit length only.
// We know that no more than 1 chunk has different size from chunk_size
// So we could use 1 bit in offset field to indicate that
// and store incomplete chunk length in Signature object.
struct ChunkInfo {
    uint64_t offset;
    uint64_t length;

    // NOTE: needed by tests only
    bool operator==(const ChunkInfo& other) const {
        return offset == other.offset && length == other.length;
    }
};

// Signature object used both while generating signature and while
// calculating delta.
// There could be used another approach: when generating signature
// we could just write information about chunks on the fly. That would
// save us hash maps lookup however signature size would be bigger
// for files with repeating segments, e.g. having big zeroed regions.
class Signature {
public:
    // NOTE: in real life these maps could be pretty small, so
    // potentially boost::flat_map could be used here.
    using chunks_by_strong_hash_t = std::unordered_map<strong_hash_t, ChunkInfo>;

public:
    Signature(uint64_t chunk_size);
    Signature(Signature&&) = default;

    // Add chunk information
    void add_chunk(const weak_hash_t weak_hash, const strong_hash_t strong_hash, const ChunkInfo& chunk) {
        _chunks_info[weak_hash].try_emplace(strong_hash, chunk);
    }

    // Returns collection chunks which have weak hash equal to `weak_hash`.
    // If there are no such chunks - returns null.
    const chunks_by_strong_hash_t* get_by_weak_hash(const weak_hash_t weak_hash) const {
        if (auto it = _chunks_info.find(weak_hash); it != _chunks_info.end()) {
            return &it->second;
        } else {
            return nullptr;
        }
    }

    uint64_t get_chunk_size() const {
        return _chunk_size;
    }

    void write_to_file(const std::string& path) const;
    static Signature read_from_file(const std::string& path);

    // NOTE: needed by tests only
    bool operator==(const Signature& other) const;

private:
    std::unordered_map<weak_hash_t, chunks_by_strong_hash_t> _chunks_info;
    uint64_t _chunk_size;
};

} // namespace rh