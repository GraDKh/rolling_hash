#include <optional>
#include <vector>

#include <gtest/gtest.h>

#include "signature_creator.h"

#include "mock_reader.h"

namespace rh {
namespace ut {

namespace {

class WeakHashMock {
public:
    weak_hash_t operator()(std::string_view) const {
        return ++_index % 2;
    }

private:
    mutable uint32_t _index = 0;
};

class StrongHashMock {
public:
    strong_hash_t operator()(std::string_view) const {
        return {++_index};
    }

private:
    mutable uint32_t _index = 0;
};

Signature create_from_data(std::vector<char> data, const size_t chunk_size) {
    MockDataReader reader(std::move(data));
    return generate_signature(reader, WeakHashMock(), StrongHashMock(), chunk_size);
}

void check_signature(const Signature& signature,
    const std::vector<std::pair<weak_hash_t, std::optional<Signature::chunks_by_strong_hash_t>>>& checks) {
    for (const auto& check: checks) {
        const auto* chunks_by_weak_hash = signature.get_by_weak_hash(check.first);
    }
}

} // namespace

TEST(SignatureCreator, empty_data) {
    
}

} // namespace ut
} // namespace rh