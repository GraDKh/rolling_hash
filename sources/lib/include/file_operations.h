#pragma once

#include <string>

namespace rh {

// Generate signature of a file at `file_path` and write it to `signature_path`.
void generate_signature(const std::string& file_path, const std::string& signature_path);

// Generate delta based on a signature read from `new_file_path` and a file at `new_file_path`
// and write it to `delta_path`.
void generate_delta(const std::string& new_file_path, const std::string& signature_path,
    const std::string& delta_path);

} // namespace rh