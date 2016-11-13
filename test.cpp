#include <gtest/gtest.h>

#include "tests/trivial_tests.h"
#include "tests/iterator_tests.h"
#include "tests/complexity_tests.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
