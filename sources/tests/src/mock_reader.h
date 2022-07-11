#pragma once

#include <vector>

#include "data_reader.h"

namespace rh {
namespace ut {

class MockDataReader : public DataReader {
public:
    MockDataReader(std::vector<char> data);

    size_t read(char* buffer, size_t max_len) override;

private:
    std::vector<char> _data;
    size_t _offset = 0;
};

} // namespace ut
} // namespace rh