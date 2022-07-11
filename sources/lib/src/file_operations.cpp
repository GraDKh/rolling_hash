#include "file_operations.h"

#include "delta_calculation.h"
#include "file_delta_builder.h"
#include "file_reader.h"
#include "hasher.h"
#include "signature_creator.h"

namespace rh {

void generate_signature(const std::string& file_path, const std::string& signature_path) {
    static constexpr size_t chunk_size = 4096;
    FileReader reader(file_path, false);
    const auto signature = generate_signature(reader, Adler32BlockHasher(), SHA1BlockHasher(), chunk_size);
    signature.write_to_file(signature_path);
}

void generate_diff(const std::string& new_file_path, const std::string& signature_path,
    const std::string& delta_path) {
    const auto signature = Signature::read_from_file(signature_path);
    FileReader new_file(new_file_path, false);
    FileDeltaBuilder builder(signature_path);
    build_delta(signature, new_file, Adler32Hasher(), SHA1Hasher(), builder);
}

} // namespace rh