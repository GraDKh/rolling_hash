#include "file_writer.h"

namespace rh {

FileWriter::FileWriter(const std::string& path, const bool buffered) {
    if (!buffered) {
        _stream.rdbuf()->pubsetbuf(0, 0);
    }
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