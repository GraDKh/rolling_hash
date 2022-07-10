#include <vector>

#include <gtest/gtest.h>

#include "file_writer.h"

#include "tmp_file.h"

namespace rh {
namespace ut {

namespace {

TmpFile write_to_file(const std::vector<std::vector<char>>& data) {
    const std::string path("tmp.data");
    FileWriter writer(path);
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

void check_write_to_file(const std::vector<std::vector<char>>& data) {
    const auto file = write_to_file(data);
    const auto expected = merge(data);
    const auto file_data = file.read_all();
    EXPECT_EQ(file_data, expected);
}

} // namespace

TEST(FileWriterTests, empty_file) {
    check_write_to_file({});

    check_write_to_file({{}});
    check_write_to_file({{}, {}});
}

TEST(FileWriterTests, single_write) {
    check_write_to_file({{'a'}});
    check_write_to_file({{'a', 'b', 'c'}});
}

TEST(FileWriterTests, several_writes) {
    check_write_to_file({{'a'}, {'b'}});
    check_write_to_file({{'a', 'b', 'c'}, {'d'}});
    check_write_to_file({{'a', 'b', 'c'}, {'d'}, {'e', 'f'}});
}

TEST(FileWriterTests, several_writes_with_empty) {
    check_write_to_file({{}, {'a'}, {'b'}});
    check_write_to_file({{'a'}, {}, {'b'}});
    check_write_to_file({{'a'}, {'b'}, {}});
}

} // namespace ut
} // namespace rh