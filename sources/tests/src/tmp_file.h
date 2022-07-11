#pragma once

#include <cstring>
#include <fstream>
#include <string>

namespace rh {
namespace ut {

class TmpFile {
public:
    TmpFile(size_t length) : _path("tmp.dat") {
        std::ofstream file(_path);

        for (size_t i = 0; i < length; i++) {
            const auto data = static_cast<char>(i);
            file.write(&data, 1);
        }
    }

    TmpFile(const std::string& path) : _path(path) {}

    TmpFile(TmpFile&& other) : _path(std::move(other._path)) {
        other._path.clear();
    }

    ~TmpFile() {
        if (!_path.empty()) {
            std::remove(_path.c_str());
        }
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


} // namespace ut
} // namespace rh