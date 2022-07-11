#pragma once

#include <fstream>

#include "data_reader.h"

namespace rh {

// Implementation od DataReader that uses a file as a data source
class FileReader : public DataReader {
public:
    // `path` - path of a file to read from.
    // `buffered` - if internal buffer should be used.
    // Sometimes we know that we are reading by big chunks so it's more efficient
    // to disable buffering to avoid double copy.
    FileReader(const std::string& path, bool buffered);

    size_t read(char* buffer, size_t max_len) override;

private:
    std::ifstream _stream;
};

} // namespace rh