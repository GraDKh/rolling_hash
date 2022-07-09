#pragma once

#include <string_view>

namespace rh {

class DataReader {
public:
    virtual ~DataReader() = default;

    virtual std::string_view read(size_t len) = 0;
};

} // namespace rh