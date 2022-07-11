#pragma once

#include <cstring>
#include <fstream>
#include <limits>

#include "signature.h"

namespace rh {

// Writes delta information to the file on the fly
class FileDeltaBuilder {
public:
    FileDeltaBuilder(const std::string& path);
    ~FileDeltaBuilder();

    void add_existing_chunk(const ChunkInfo& chunk) {
        close_section();
        write_value(std::numeric_limits<uint64_t>::max());
        write_value(chunk);
    }

    void add_new_data(std::string_view data) {
        if (_last_section_length == 0) {
            _last_section_length = _stream.tellp();
        }
        _stream.write(data.data(), data.size());
        _last_section_length += data.size();
    }

    void close();

private:
    void close_section() {
        if (_last_section_length != 0) {
            // go back and write length of the closed section
            _stream.seekp(_last_pos);
            write_value(_last_section_length);
            _last_section_length = 0;

            // seek back to the next section
            _stream.seekp(std::ofstream::end);
        }
    }

    template <typename T>
    void write_value(const T& val) {
        static_assert(std::is_pod_v<T>, "T should be POD type");
        static constexpr auto value_size = sizeof(T);

        char buffer[value_size];
        std::memcpy(buffer, &val, value_size);
        _stream.write(buffer, sizeof(value_size));
    }

private:
    std::ofstream _stream;
    std::ofstream::pos_type _last_pos = std::ios_base::end;
    uint64_t _last_section_length = 0;
};


} // namespace rh
