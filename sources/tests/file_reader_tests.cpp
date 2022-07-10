#include <algorithm>

#include "gtest/gtest.h"

#include "file_reader.h"

#include "tmp_file.h"

namespace rh {
namespace ut {

namespace {

void test_read_from_file(TmpFile& file, const std::vector<size_t>& chunk_sizes) {
    FileReader reader{file.get_path()};
    const auto expected_data = file.read_all();
    std::vector<char> read_data;
    std::vector<char> chunk_data;
    for (const auto& chunk_len: chunk_sizes) {
        chunk_data.resize(chunk_len);
        const auto bytes_read = reader.read(chunk_data.data(), chunk_len);
        const auto expected_length = (read_data.size() + chunk_len > expected_data.size()) ?
            (expected_data.size() - read_data.size()) :
            chunk_len;
        EXPECT_EQ(bytes_read, expected_length);
        std::copy(chunk_data.data(), chunk_data.data() + bytes_read, std::back_inserter(read_data));
    }
    EXPECT_EQ(read_data, expected_data);
}

} // namespace

TEST(FileReaderTests, test_empty_file) {
    TmpFile file(0);

    test_read_from_file(file, {0});
    test_read_from_file(file, {1});
    test_read_from_file(file, {2, 1});
}

TEST(FileReaderTests, test_single_read) {
    TmpFile file(4);

    test_read_from_file(file, {4});
    test_read_from_file(file, {4, 1});
}

TEST(FileReaderTests, test_two_reads) {
    TmpFile file(4);

    test_read_from_file(file, {2, 2});
    test_read_from_file(file, {2, 2, 1});
}

TEST(FileReaderTests, test_zero_size_reads) {
    TmpFile file(4);

    test_read_from_file(file, {1, 0, 3});
    test_read_from_file(file, {0, 2, 0, 2, 1});
}

} // namespace ut
} // namespace rh