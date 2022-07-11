#include <vector>

#include <gtest/gtest.h>

#include "delta_calculation.h"
#include "memory_delta_builder.h"
#include "signature_creator.h"
#include "hasher.h"

#include "mock_reader.h"

namespace rh {
namespace ut {

namespace {

using delta_t = std::vector<MemoryDeltaBuilder::block_data>;
using byte_data = std::vector<char>;

delta_t calculate_delta(
    std::vector<char> old_data, std::vector<char> new_data, const size_t chunk_size) {
    MockDataReader old_data_reader(std::move(old_data));
    const auto signature = generate_signature(
        old_data_reader,
        Adler32BlockHasher(),
        SHA1BlockHasher(),
        chunk_size);
    MemoryDeltaBuilder delta_builder;
    MockDataReader new_data_reader(std::move(new_data));
    build_delta(signature, new_data_reader, Adler32Hasher(), SHA1Hasher(), delta_builder);
    return delta_builder.take_delta();
}

} // namespace

TEST(DeltaBuilderTests, two_empty_files) {
    const auto delta = calculate_delta({}, {}, 2);
    delta_t expected_delta{};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, old_empty_file_1_block) {
    const auto delta = calculate_delta({}, {'a'}, 2);
    delta_t expected_delta{byte_data{'a'}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, old_empty_file_1_full_block) {
    const auto delta = calculate_delta({}, {'a', 'b'}, 2);
    delta_t expected_delta{byte_data{'a', 'b'}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, old_empty_file_2_blocks) {
    const auto delta = calculate_delta({}, {'a', 'b', 'c'}, 2);
    delta_t expected_delta{byte_data{'a', 'b', 'c'}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, old_empty_file_2_full_blocks) {
    const auto delta = calculate_delta({}, {'a', 'b', 'c', 'd'}, 2);
    delta_t expected_delta{byte_data{'a', 'b', 'c', 'd'}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, same_data_1_block) {
    const auto delta = calculate_delta({'a'}, {'a'}, 2);
    delta_t expected_delta{ChunkInfo{0, 1}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, same_data_1_full_block) {
    const auto delta = calculate_delta({'a', 'b'}, {'a', 'b'}, 2);
    delta_t expected_delta{ChunkInfo{0, 2}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, same_data_2_blocks) {
    const auto delta = calculate_delta({'a', 'b', 'c'}, {'a', 'b', 'c'}, 2);
    delta_t expected_delta{ChunkInfo{0, 2}, ChunkInfo{2, 1}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, same_1_append_block) {
    const auto delta = calculate_delta({'a', 'b'}, {'a', 'b', 'c'}, 2);
    delta_t expected_delta{ChunkInfo{0, 2}, byte_data{'c'}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, prepend_block) {
    const auto delta = calculate_delta({'a', 'b'}, {'c', 'a', 'b'}, 2);
    delta_t expected_delta{byte_data{'c'}, ChunkInfo{0, 2}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, prepend_full_block) {
    const auto delta = calculate_delta({'a', 'b'}, {'c', 'd', 'a', 'b'}, 2);
    delta_t expected_delta{byte_data{'c', 'd'}, ChunkInfo{0, 2}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, prepend_2_blocks) {
    const auto delta = calculate_delta({'a', 'b'}, {'c', 'd', 'e', 'a', 'b'}, 2);
    delta_t expected_delta{byte_data{'c', 'd', 'e'}, ChunkInfo{0, 2}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, append_block) {
    const auto delta = calculate_delta({'a', 'b'}, {'a', 'b', 'c'}, 2);
    delta_t expected_delta{ChunkInfo{0, 2}, byte_data{'c'}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, append_full_block) {
    const auto delta = calculate_delta({'a', 'b'}, {'a', 'b', 'c', 'd'}, 2);
    delta_t expected_delta{ChunkInfo{0, 2}, byte_data{'c', 'd'}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, append_2_blocks) {
    const auto delta = calculate_delta({'a', 'b'}, {'a', 'b', 'c', 'd', 'e'}, 2);
    delta_t expected_delta{ChunkInfo{0, 2}, byte_data{'c', 'd', 'e'}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, suffix_1_block_missing) {
    const auto delta = calculate_delta({'a', 'b', 'c'}, {'a', 'b'}, 2);
    delta_t expected_delta{ChunkInfo{0, 2}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, suffix_full_block_missing) {
    const auto delta = calculate_delta({'a', 'b', 'c', 'd'}, {'a', 'b'}, 2);
    delta_t expected_delta{ChunkInfo{0, 2}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, prefix_1_block_missing) {
    const auto delta = calculate_delta({'a', 'b', 'c'}, {'c'}, 2);
    delta_t expected_delta{ChunkInfo{2, 1}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, prefix_2_blocks_missing) {
    const auto delta = calculate_delta({'a', 'b', 'c', 'd', 'e'}, {'e'}, 2);
    delta_t expected_delta{ChunkInfo{4, 1}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, byte_inserted) {
    const auto delta = calculate_delta({'a', 'b', 'c', 'd'}, {'a', 'b', 'e', 'c', 'd'}, 2);
    delta_t expected_delta{ChunkInfo{0, 2}, byte_data{'e'}, ChunkInfo{2, 2}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, two_bytes_inserted) {
    const auto delta = calculate_delta({'a', 'b', 'c', 'd'}, {'a', 'b', 'e', 'f', 'c', 'd'}, 2);
    delta_t expected_delta{ChunkInfo{0, 2}, byte_data{'e', 'f'}, ChunkInfo{2, 2}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, three_bytes_inserted) {
    const auto delta = calculate_delta({'a', 'b', 'c', 'd'}, {'a', 'b', 'e', 'f', 'g', 'c', 'd'}, 2);
    delta_t expected_delta{ChunkInfo{0, 2}, byte_data{'e', 'f', 'g'}, ChunkInfo{2, 2}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, chunks_swapped) {
    const auto delta = calculate_delta({'a', 'b', 'c', 'd'}, {'c', 'd', 'a', 'b'}, 2);
    delta_t expected_delta{ChunkInfo{2, 2}, ChunkInfo{0, 2}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, chunks_swapped_with_insertion) {
    const auto delta = calculate_delta({'a', 'b', 'c', 'd'}, {'c', 'd', 'e', 'a', 'b'}, 2);
    delta_t expected_delta{ChunkInfo{2, 2}, byte_data{'e'}, ChunkInfo{0, 2}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, first_chunk_replaced) {
    const auto delta = calculate_delta({'a', 'b', 'c'}, {'e', 'd', 'c'}, 2);
    delta_t expected_delta{byte_data{'e', 'd'}, ChunkInfo{2, 1}};
    EXPECT_EQ(delta, expected_delta);
}

TEST(DeltaBuilderTests, second_chunk_replaced) {
    const auto delta = calculate_delta({'a', 'b', 'c'}, {'a', 'b', 'd'}, 2);
    delta_t expected_delta{ChunkInfo{0, 2}, byte_data{'d'}};
    EXPECT_EQ(delta, expected_delta);
}

} // namespace ut
} // namespace rh