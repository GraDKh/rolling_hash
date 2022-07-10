#pragma once

#include <vector>

namespace rh {

class DataReader {
public:
    virtual ~DataReader() = default;

    // Reads data to buffer up specified max_len.
    // Returns the number of bytes being read.
    virtual size_t read(char* buffer, size_t max_len) = 0;
};

} // namespace rh