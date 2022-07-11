#pragma once

#include <cstring>
#include <fstream>
#include <limits>

#include "signature.h"

namespace rh {

// Delta builder implementation that writes delta information 
// to the file on the fly.
// The current format is pretty simple and straightforward:
// Each block is of two types:
// 1. Existing chunk:
//    uint64  [UINT64_MAX]
//    uint64  [offset in original]
//    uint64  [length]
// 2. New data
//    uint64  [length in bytes]
//    byte    [byte 1]
//    ...
//    byte    [byte N]
// This format may be optimized for compactness.
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
        // Current approach is to write new data to the file.
        // and when section is finished go back and write the length.
        // That could be less efficient than just adding a new section per every call.
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
