#pragma once

#include <cstring>
#include <string_view>

namespace rh {

class DataWriter {
public:
    virtual ~DataWriter() = default;

    virtual void write(std::string_view data) = 0;

    template <typename T>
    void write_value(const T& value) {
        static_assert(std::is_pod_v<T>, "T should be POD type");
        char tmp[sizeof(T)];
        std::memcpy(tmp, &value, sizeof(T));
        write({tmp, sizeof(T)});
    }
};

} // namespace rh