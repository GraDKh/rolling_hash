#include "file_reader.h"

namespace rh {

FileReader::FileReader(const std::string& path, const bool buffered) {
    if (!buffered) {
        _stream.rdbuf()->pubsetbuf(0, 0);
    }
    _stream.open(path.c_str());
    if (_stream.fail()) {
        throw std::runtime_error("Failed to open file " + path);
    }
}

size_t FileReader::read(char* buffer, size_t max_len) {
    if (_stream.eof()) {
        return 0;
    }

    const auto bytes_read = static_cast<size_t>(_stream.read(buffer, max_len).gcount());
    if (!_stream.eof() && _stream.fail()) {
        throw std::runtime_error("Failed to read from file");
    }

    return bytes_read;
}

} // namespace rh