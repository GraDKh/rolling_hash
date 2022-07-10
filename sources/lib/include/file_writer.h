#pragma once

#include <fstream>

#include "data_writer.h"

namespace rh {

class FileWriter: DataWriter {
public:
    FileWriter(const std::string& path);

    void write(std::string_view data) override;

private:
    std::ofstream _stream;
};

} // namespace rh