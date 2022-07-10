#pragma once

#include <optional>
#include <unordered_map>

#include "types.h"

namespace rh {

struct ChunkInfo {
    uint64_t offset;
    uint64_t length;
};

class Signature {
    using chunks_by_strong_hash_t = std::unordered_map<strong_hash_t, ChunkInfo>;

public:
    void add_chunk(const weak_hash_t weak_hash, const strong_hash_t strong_hash, const ChunkInfo& chunk) {
        _chunks_info[weak_hash].try_emplace(strong_hash, chunk);
    }

    const chunks_by_strong_hash_t* get_by_weak_hash(const weak_hash_t weak_hash) const {
        if (auto it = _chunks_info.find(weak_hash); it != _chunks_info.end()) {
            return &it->second;
        } else {
            return nullptr;
        }
    }

    void write_to_file(const std::string& path);
    static Signature read_from_file(const std::string& path);

private:
    std::unordered_map<weak_hash_t, chunks_by_strong_hash_t> _chunks_info;
};

} // namespace rh