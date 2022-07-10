#pragma once

#include <cstring>
#include <stdexcept>
#include <vector>

namespace rh {

class DataReader {
public:
    virtual ~DataReader() = default;

    // Reads data to buffer up specified max_len.
    // Returns the number of bytes being read.
    virtual size_t read(char* buffer, size_t max_len) = 0;

    template <typename T>
    T read_value() {
        static_assert(std::is_pod_v<T>, "T should be POD type");
        char tmp[sizeof(T)];
        const auto bytes_read = read(tmp, sizeof(T));
        if (bytes_read != sizeof(T)) {
            throw std::runtime_error("corrupted file");
        }
        T result;
        std::memcpy(&result, tmp, sizeof(T));
        return result;
    }
};

} // namespace rh