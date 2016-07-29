#include <catch.hpp>

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include <atoms/bit/set_bits.h>
#include <array>

using namespace atoms;

TEST_CASE("Single bit setting works", "bit/set_bits:single") {
    for (int i = 0; i != 9; i++) {
        CAPTURE(i);
        REQUIRE(set_bits8(i) == uint8_t(1 << i));
    }

    for (int i = 0; i != 17; i++) {
        CAPTURE(i);
        REQUIRE(set_bits16(i) == uint16_t(1 << i));
    }

    for (int i = 0; i != 33; i++) {
        CAPTURE(i);
        REQUIRE(set_bits32(i) == uint32_t(1 << i));
    }
}

TEST_CASE("Multiple bit setting works", "bit/set_bits:multiple") {
    REQUIRE(
        set_bits8(1, 2, 7)
        ==
        uint8_t( (1 << 1) | (1 << 2) | (1 << 7) )
    );

    REQUIRE(
        set_bits8(1, 2, 7, 8, 9, 15)
        ==
        uint8_t( (1 << 1) | (1 << 2) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 15) )
    );
}

TEST_CASE("Test bit is compile time constant", "bit/set_bits:const") {
    std::array<int, set_bits8(2)> test_array;
}