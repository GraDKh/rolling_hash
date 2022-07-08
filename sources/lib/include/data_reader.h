#pragma once

#include <string_view>

namespace rh {

class DataReader {
public:
    virtual ~DataReader() = 0;

    virtual std::string_view read_next_data(size_t len) = 0;
};

} // namespace rh