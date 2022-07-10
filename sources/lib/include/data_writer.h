#pragma once

#include <string_view>

namespace rh {

class DataWriter {
public:
    virtual ~DataWriter() = default;

    virtual void write(std::string_view data) = 0;
};

} // namespace rh