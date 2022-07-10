#pragma once

#include <fstream>

#include "data_reader.h"

namespace rh {

class FileReader : public DataReader {
public:
    FileReader(const std::string& path, bool buffered);

    size_t read(char* buffer, size_t max_len) override;

private:
    std::ifstream _stream;
};

} // namespace rh