#include <catch.hpp>

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include <atoms/communication/avakar.h>
#include <vector>
#include <initializer_list>

using namespace atoms;

std::vector<uint8_t> a2v(const AvakarPacket& p) {
    return { p.raw(), p.raw() + p.raw_size() };
}

std::vector<uint8_t> c(const std::initializer_list<uint8_t>& l) {
    return { l.begin(), l.end() };
}

TEST_CASE("Avakar packet writing", "communication/avakar.h:writin") {
    AvakarPacket out;
    REQUIRE( a2v(out) == c({ 0x80, 0x00 }) );

    out.set_command(8);
    REQUIRE( out.get_command() == 8 );
    REQUIRE( a2v(out) == c({ 0x80, 0x80 }) );

    out.push<char>(42);
    REQUIRE( a2v(out) == c({ 0x80, 0x81, 0x2a }) );

    out.push<uint32_t>(42);
    REQUIRE( a2v(out) == c({ 0x80, 0x85, 0x2a, 0x2a, 0x00, 0x00, 0x00 }) );

    out.push<float>(42);
    REQUIRE( a2v(out) == c({ 0x80, 0x89, 0x2a, 0x2a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x42 }) );

    char test[] = { 'A', 'B' };
    out.push_n<char>(test, sizeof(test));
    REQUIRE( a2v(out) == c({ 0x80, 0x8b, 0x2a, 0x2a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x42, 0x41, 0x42 }) );
}

TEST_CASE("Avakar packet reading", "communication/avakar.h:reading") {
    std::vector<uint8_t> data = {
        0x42, 0x80, 0x8b, 0x2a, 0x2a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x42, 0x41, 0x42, 0x42, 0x42, 0x42
    };

    AvakarPacket in;
    int i = 0;
    while (!in.push_byte(data[i])) {
        i++;
        REQUIRE(i < data.size());
    }

    REQUIRE(in.get_command() == 8);
    REQUIRE(in.size() == 11);
    REQUIRE(in.get<char>(0) == 42);
    REQUIRE(in.get<uint32_t>(1) == 42);
    REQUIRE(in.get<float>(5) == 42);
    REQUIRE(in.get<char>(9) == 'A');
    REQUIRE(in.get<char>(10) == 'B');
}