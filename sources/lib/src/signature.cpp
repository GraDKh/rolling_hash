#include "signature.h"

#include "file_reader.h"
#include "file_writer.h"

namespace rh {

void Signature::write_to_file(const std::string& path) const {
    FileWriter output(path, true);
    output.write_value(_chunks_info.size());
    for (const auto& [weak_hash, chunks_by_strong]: _chunks_info) {
        output.write_value(weak_hash);
        output.write_value(chunks_by_strong.size());
        for (const auto& [strong_hash, chunk]: chunks_by_strong) {
            output.write_value(strong_hash);
            output.write_value(chunk.offset);
            output.write_value(chunk.length);
        }
    }
}

Signature Signature::read_from_file(const std::string& path) {
    Signature result;
    FileReader input(path, true);
    const auto count = input.read_value<size_t>();
    result._chunks_info.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        const auto weak_hash = input.read_value<weak_hash_t>();
        const auto chunks_count = input.read_value<size_t>();
        chunks_by_strong_hash_t chunks;
        chunks.reserve(chunks_count);
        for (size_t chunk_index = 0 ; chunk_index < chunks_count; ++chunk_index) {
            const auto strong_hash = input.read_value<strong_hash_t>();
            const auto offset = input.read_value<uint64_t>();
            const auto length = input.read_value<uint64_t>();
            chunks.insert({strong_hash, {offset, length}});
        }
        result._chunks_info.insert({weak_hash, std::move(chunks)});
    }

    return result;
}

bool Signature::operator==(const Signature& other) const {
    return _chunks_info == other._chunks_info;
}

} // namespace rh