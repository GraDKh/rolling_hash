#pragma once

#include <string>

namespace rh {

void generate_signature(const std::string& file_path, const std::string& signature_path);

void generate_diff(const std::string& new_file_path, const std::string& signature_path, 
    const std::string& delta_path);

} // namespace rh