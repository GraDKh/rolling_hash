#include <gtest/gtest.h>

#include "hasher.h"

namespace rh {

namespace ut {

TEST(HasherTests, adler32_roll_test) {
    Adler32Hasher hasher;
    hasher.init("ca");
    hasher.remove_old('c');
    hasher.add_new('b');
    EXPECT_EQ(hasher.get_value(), Adler32BlockHasher()("ab"));
}

TEST(HasherTests, sha1_append) {
    SHA1Hasher hasher;
    hasher.init("a");
    hasher.add("b");
    EXPECT_EQ(hasher.get_value(), SHA1BlockHasher()("ab"));
}

} // namespace ut
} // namespace rh