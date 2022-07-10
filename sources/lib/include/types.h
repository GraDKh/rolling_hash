#pragma once

#include <array>
#include <cstdint>

namespace rh {

using weak_hash_t = uint32_t;
using strong_hash_t = std::array<uint32_t, 5>;

} // namespace rh
