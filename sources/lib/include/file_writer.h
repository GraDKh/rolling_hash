#pragma once

#include <fstream>

#include "data_writer.h"

namespace rh {

class FileWriter: public DataWriter {
public:
    FileWriter(const std::string& path, bool buffered);

    void write(std::string_view data) override;

private:
    std::ofstream _stream;
};

} // namespace rh