#include <vector>

#include <gtest/gtest.h>

#include "file_writer.h"

#include "tmp_file.h"

namespace rh {
namespace ut {

namespace {

TmpFile write_to_file(const std::vector<std::vector<char>>& data, const bool buffered) {
    const std::string path("tmp.data");
    FileWriter writer(path, buffered);
    for (const auto& chunk: data) {
        writer.write({chunk.data(), chunk.size()});
    }

    return {path};
}
template<typename T>
std::vector<T> merge(const std::vector<std::vector<T>>& vecs) {
    size_t size = 0;
    for (const auto& v: vecs) {
        size += v.size();
    }

    std::vector<T> result;
    result.reserve(size);
    for (const auto& v: vecs) {
        result.insert(result.end(), v.begin(), v.end());
    }

    return result;
}

void check_write_to_file(const std::vector<std::vector<char>>& data, const bool buffered) {
    const auto file = write_to_file(data, buffered);
    const auto expected = merge(data);
    const auto file_data = file.read_all();
    EXPECT_EQ(file_data, expected);
}

} // namespace

class FileWriterTests :public ::testing::TestWithParam<bool> {
};

TEST_P(FileWriterTests, empty_file) {
    check_write_to_file({}, GetParam());

    check_write_to_file({{}}, GetParam());
    check_write_to_file({{}, {}}, GetParam());
}

TEST_P(FileWriterTests, single_write) {
    check_write_to_file({{'a'}}, GetParam());
    check_write_to_file({{'a', 'b', 'c'}}, GetParam());
}

TEST_P(FileWriterTests, several_writes) {
    check_write_to_file({{'a'}, {'b'}}, GetParam());
    check_write_to_file({{'a', 'b', 'c'}, {'d'}}, GetParam());
    check_write_to_file({{'a', 'b', 'c'}, {'d'}, {'e', 'f'}}, GetParam());
}

TEST_P(FileWriterTests, several_writes_with_empty) {
    check_write_to_file({{}, {'a'}, {'b'}}, GetParam());
    check_write_to_file({{'a'}, {}, {'b'}}, GetParam());
    check_write_to_file({{'a'}, {'b'}, {}}, GetParam());
}

INSTANTIATE_TEST_CASE_P(
        FileWriterTests,
        FileWriterTests,
        ::testing::Values(true, false));

} // namespace ut
} // namespace rh