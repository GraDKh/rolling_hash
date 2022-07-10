#include "file_delta_builder.h"

namespace rh {

FileDeltaBuilder::FileDeltaBuilder(const std::string& path)
    : _stream(path.c_str()) {
    if (_stream.fail()) {
        throw std::runtime_error("failed to open file for writing: " + path);
    }
}

FileDeltaBuilder::~FileDeltaBuilder() {
    close();
}

} // namespace rh