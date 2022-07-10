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
        if (check.second) {
            ASSERT_NE(chunks_by_weak_hash, nullptr);
            EXPECT_EQ(*chunks_by_weak_hash, *check.second);
        } else {
            EXPECT_EQ(chunks_by_weak_hash, nullptr);
        }
    }
}

} // namespace

TEST(SignatureCreator, empty_data) {
    const auto signature = create_from_data({}, 0);
    check_signature(signature, {{weak_hash_t(0), std::nullopt}});
}

TEST(SignatureCreator, single_block) {
    const auto signature = create_from_data({'a', 'b'}, 2);
    check_signature(signature,
        {{weak_hash_t(0), std::nullopt},
         {weak_hash_t(1), {{{{1}, {0, 2}}}}}});
}

TEST(SignatureCreator, two_blocks_with_incomplete) {
    const auto signature = create_from_data({'a', 'b', 'c'}, 2);
    check_signature(signature,
        {{weak_hash_t(0), {{{{2}, {2, 1}}}}},
         {weak_hash_t(1), {{{{1}, {0, 2}}}}}});
}

TEST(SignatureCreator, weak_hash_collision) {
    const auto signature = create_from_data({'a', 'b', 'c', 'd', 'e'}, 2);
    check_signature(signature,
        {{weak_hash_t(0), {{{{2}, {2, 2}}}}},
         {weak_hash_t(1), {{{{1}, {0, 2}},
                            {{3}, {4, 1}}}}}});
}

} // namespace ut
} // namespace rh