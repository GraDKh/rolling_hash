#include "file_reader.h"

namespace rh {

FileReader::FileReader(const std::string& path) {
    _stream.rdbuf()->pubsetbuf(0, 0);
    _stream.open(path.c_str());
    if (_stream.fail()) {
        throw std::runtime_error("Failed to open file " + path);
    }
}

std::string_view FileReader::read(const size_t len) {
    if (_stream.eof()) {
        return {};
    }

    _buffer.resize(len);
    const auto bytes_read = static_cast<size_t>(_stream.read(_buffer.data(), len).gcount());
    if (!_stream.eof() && _stream.fail()) {
        throw std::runtime_error("Failed to read from file");
    }
    _offset += bytes_read;
    _buffer.resize(bytes_read);

    return {_buffer.data(), bytes_read};
}

} // namespace rh