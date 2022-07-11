#include <gtest/gtest.h>

#include "signature.h"

#include "tmp_file.h"

namespace rh {

namespace ut {

namespace {

void check_roundtrip(const Signature& signature) {
    TmpFile tmp("signature");
    signature.write_to_file(tmp.get_path());
    const auto recovered_signature = Signature::read_from_file(tmp.get_path());
    EXPECT_EQ(signature, recovered_signature);
}

} // namespace

TEST(SignatureTests, add_single) {
    const weak_hash_t weak = 1;
    const strong_hash_t strong = {2};
    const ChunkInfo chunk = {3, 4};

    Signature sign{5};
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

    Signature sign{5};
    sign.add_chunk(weak, strong_1, chunk_1);
    sign.add_chunk(weak, strong_2, chunk_2);

    const auto* search_res = sign.get_by_weak_hash(weak);
    ASSERT_NE(search_res, nullptr);
    EXPECT_EQ(*search_res, (Signature::chunks_by_strong_hash_t{{strong_1, chunk_1}, {strong_2, chunk_2}}));

    const auto search_res_2 = sign.get_by_weak_hash({2});
    ASSERT_EQ(search_res_2, nullptr);
}

TEST(SignatureTests, empty_serialization_deserialization) {
    Signature sign{5};
    check_roundtrip(sign);
}

TEST(SignatureTests, complex_serialization_deserialization) {
    const weak_hash_t weak_1 = 1;
    const weak_hash_t weak_2 = 2;
    const strong_hash_t strong_1 = {3};
    const strong_hash_t strong_2 = {4};
    const strong_hash_t strong_3 = {5};
    const ChunkInfo chunk_1 = {6, 7};
    const ChunkInfo chunk_2 = {8, 9};
    const ChunkInfo chunk_3 = {10, 11};

    Signature sign{5};
    sign.add_chunk(weak_1, strong_1, chunk_1);
    sign.add_chunk(weak_2, strong_2, chunk_2);
    sign.add_chunk(weak_2, strong_3, chunk_3);
    check_roundtrip(sign);
}

} // namespace ut
} // namespace rh