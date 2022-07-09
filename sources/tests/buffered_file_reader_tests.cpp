#include <algorithm>
#include <array>
#include <cstring>
#include <fstream>

#include "gtest/gtest.h"

#include "file_reader.h"


namespace rh {
namespace ut {

namespace {

class TmpFile {
public:
    TmpFile(size_t length) : _path("tmp.dat") {
        std::ofstream file(_path);

        for (size_t i = 0; i < length; i++) {
            const auto data = static_cast<char>(i);
            file.write(&data, 1);
        }
    }

    ~TmpFile() {
        std::remove(_path.c_str());
    }

    const std::string& get_path() const {
        return _path;
    }

    std::vector<char> read_all() const {
        std::ifstream file(_path, std::ios::binary);
        file.seekg(0, std::ios::end);
        const auto file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        // read the data:
        std::vector<char> result(file_size);
        file.read(result.data(), file_size);
        return result;
    }

private:
    std::string _path;
    size_t _length;
};

void test_read_from_file(TmpFile& file, const std::vector<size_t>& chunk_sizes) {
    FileReader reader{file.get_path()};
    const auto expected_data = file.read_all();
    std::vector<char> read_data;
    for (const auto& chunk_len: chunk_sizes) {
        const auto data = reader.read(chunk_len);
        const auto expected_length = (read_data.size() + chunk_len > expected_data.size()) ?
            (expected_data.size() - read_data.size()) :
            chunk_len;
        EXPECT_EQ(data.size(), expected_length);
        std::copy(std::begin(data), std::end(data), std::back_inserter(read_data));
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