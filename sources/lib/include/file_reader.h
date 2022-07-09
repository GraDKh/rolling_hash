#pragma once

#include <fstream>
#include <vector>

#include "data_reader.h"

namespace rh {

class FileReader : public DataReader {
public:
    FileReader(const std::string& path);

    std::string_view read(size_t len) override;

private:
    std::fstream _stream;
    size_t _offset = 0;
    std::vector<char> _buffer;
};

} // namespace rh