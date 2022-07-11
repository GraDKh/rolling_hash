#pragma once

#include <cstring>
#include <string_view>

namespace rh {

// Base class for writing data to the external data source.
class DataWriter {
public:
    virtual ~DataWriter() = default;

    // Writes data.
    // TODO: switch to std::span<byte> if C++20 is used.
    virtual void write(std::string_view data) = 0;

    // Helper method to write POD values.
    // That's why our signature and delta files aren't portable now.
    template <typename T>
    void write_value(const T& value) {
        static_assert(std::is_pod_v<T>, "T should be POD type");
        // Use memcpy to avoid type punning UB.
        // Compiler should be able to optimize it out.
        char tmp[sizeof(T)];
        std::memcpy(tmp, &value, sizeof(T));
        write({tmp, sizeof(T)});
    }
};

} // namespace rh