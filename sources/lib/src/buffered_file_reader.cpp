#include "buffered_file_reader.h"

namespace rh {

BufferedFileReader::BufferedFileReader(const std::string& path, const size_t buffer_size)
    : _stream(path.c_str()), _buffer_size(buffer_size) {
    if (_stream.fail()) {
        throw std::runtime_error("failed to read file " + path);
    }
    _buffer.reserve(_buffer_size);
}

std::string_view BufferedFileReader::read_next_data(const size_t len) {
    if (_buffer_offset <= _offset && (_buffer_offset + _buffer.size()) >= (_offset + len)) {
        std::string_view result{_buffer.data() + _offset - _buffer_offset, len};
        _offset += len;

        return result;
    }

    _buffer.resize(std::min(len, _buffer_size));
    const auto bytes_read = _stream.read(_buffer.data(), len).gcount() - _offset;
    _buffer.resize(bytes_read);
    std::string_view result{_buffer.data() + _offset - _buffer_offset, std::min(len, bytes_read)};
    _offset += bytes_read;

    return result;
}

} // namespace rh