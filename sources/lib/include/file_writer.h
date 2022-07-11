#pragma once

#include <fstream>

#include "data_writer.h"

namespace rh {

class FileWriter: public DataWriter {
public:
    // `path` - path of a file to read from.
    // `buffered` - if internal buffer should be used.
    // Sometimes we know that we are writing by big chunks so it's more efficient
    // to disable buffering to avoid double copy.
    FileWriter(const std::string& path, bool buffered);

    void write(std::string_view data) override;

private:
    std::ofstream _stream;
};

} // namespace rh