#include "mock_reader.h"

#include <cstring>

namespace rh {
namespace ut {

MockDataReader::MockDataReader(std::vector<char> data) 
    : _data(std::move(data)) {}

size_t MockDataReader::read(char* buffer, const size_t max_len) {
    if (_offset == _data.size()) {
        return 0;
    }

    const auto bytes_to_read = std::min(max_len, _data.size() - _offset);
    std::memcpy(buffer, _data.data() + _offset, bytes_to_read);
    _offset += bytes_to_read;
    return bytes_to_read;
}

} // namespace ut
} // namespace rh