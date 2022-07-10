#pragma once

#include <string_view>

#include "types.h"

namespace rh {

class Adler32Hasher {
    static constexpr weak_hash_t divisor = 65521;
public:
    void init(const std::string_view range) {
        _a = 1;
        _b = 0;
        for (const auto ch: range) {
            const auto val = static_cast<weak_hash_t>(ch);
            _a += val;
            _b += _a;
        }
        chunk_size += range.size();
    }

    void add_new(const char v) {
        const auto val = static_assert<weak_hash_t>(v);
        _a += val;
        _b += _a;
        ++chunk_size;
    }

    void remove_old(const char v) {
        const auto val = static_assert<weak_hash_t>(v);
        _a -= val;
        _b -= val * chunk_size;
        --chunk_size;
    }

    weak_hash_t get_value() {
        _a %= divisor;
        _b %= divisor;
        return (_b << 16) | _a;
    }

private:
    weak_hash_t _a = 1;
    weak_hash_t _b = 0;
    weak_hash_t chunk_size = 0;
};

class Adler32BlockHasher {
public:
    weak_hash_t operator()(const std::string_view range) const {
        Adler32Hasher hasher;
        hasher.init(range);

        return hasher.get_value();
    }
};

class SHA256Hasher {
public:
    strong_hash_t operator()(const std::string_view range) const {
        return {};
    }
};

} // namespace rh