#include <gtest/gtest.h>

#include "signature.h"

namespace rh {
bool operator==(const ChunkInfo& left, const ChunkInfo& right) {
    return left.length == right.length && left.offset == right.offset;
}

namespace ut {

TEST(SignatureTests, add_single) {
    const weak_hash_t weak = 1;
    const strong_hash_t strong = {2};
    const ChunkInfo chunk = {3, 4};

    Signature sign;
    sign.add_chunk(weak, strong, chunk);

    const auto* search_res = sign.get_by_weak_hash(weak);
    ASSERT_NE(search_res, nullptr);
    EXPECT_EQ(*search_res, (Signature::chunks_by_strong_hash_t{{strong, chunk}}));

    const auto search_res_2 = sign.get_by_weak_hash({2});
    ASSERT_EQ(search_res_2, nullptr);
}

TEST(SignatureTests, add_two_chunks) {
    const weak_hash_t weak = 1;
    const strong_hash_t strong_1 = {2};
    const strong_hash_t strong_2 = {3};
    const ChunkInfo chunk_1 = {4, 5};
    const ChunkInfo chunk_2 = {6, 7};

    Signature sign;
    sign.add_chunk(weak, strong_1, chunk_1);
    sign.add_chunk(weak, strong_2, chunk_2);

    const auto* search_res = sign.get_by_weak_hash(weak);
    ASSERT_NE(search_res, nullptr);
    EXPECT_EQ(*search_res, (Signature::chunks_by_strong_hash_t{{strong_1, chunk_1}, {strong_2, chunk_2}}));

    const auto search_res_2 = sign.get_by_weak_hash({2});
    ASSERT_EQ(search_res_2, nullptr);
}

} // namespace ut
} // namespace rh