#include "file_writer.h"

namespace rh {

FileWriter::FileWriter(const std::string& path) {
    _stream.open(path.c_str());
    if (_stream.fail()) {
        throw std::runtime_error("Failed to open file " + path);
    }
}

void FileWriter::write(std::string_view data) {
    _stream.write(data.data(), data.size());
    if (_stream.fail()) {
        throw std::runtime_error("Failed to write to the file");
    }
}

} // namespace rh