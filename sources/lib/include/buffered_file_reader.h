#pragma once

#include <fstream>
#include <vector>

#include "data_reader.h"

namespace rh {

class BufferedFileReader : public DataReader {
public:
    BufferedFileReader(const std::string& path, size_t buffer_size = 4096);

    std::string_view read_next_data(size_t len) override;

private:
    std::fstream _stream;
    size_t _buffer_size;
    size_t _offset = 0;
    size_t _buffer_offset = 0;
    std::vector<char> _buffer;
};

} // namespace rh