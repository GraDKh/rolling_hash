#pragma once

#include "types.h"

namespace rh {

template <class T> 
void hash_combine(size_t& seed, const T& v) {
    // This implementation supports 64-bit hash size only
    static_assert(sizeof(size_t) == 8);

    std::hash<T> hasher;
    const size_t kMul = 0x9ddfea08eb382d69ULL;
    size_t a = (hasher(v) ^ seed) * kMul;
    a ^= (a >> 47);
    size_t b = (seed ^ a) * kMul;
    b ^= (b >> 47);
    seed = b * kMul;
}

} // namespace rh

namespace std {

template<>
struct hash<rh::strong_hash_t> {
    size_t operator()(const rh::strong_hash_t& val) const {
        size_t seed = 0;
        for (size_t i = 0; i < val.size(); ++i) {
            rh::hash_combine(seed, val[i]);
        }

        return seed;
    }
};

} // namespace std