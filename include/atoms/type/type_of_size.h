#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include <stdint.h>

namespace atoms {

using bit_width_type = uint8_t;

template <bit_width_type I, bit_width_type F>
class Fixed;

template <bit_width_type T>
struct type_of_size {
	static const bool is_specialised = false;
	using value_type = void;
};

template<>
struct type_of_size<64> {
	static const bool is_specialised = true;
	static const uint8_t size = 64;
	using value_type    = int64_t;
	using signed_type   = int64_t;
	using unsigned_type = uint64_t;
	using next_type     = type_of_size<128>;
};

template<>
struct type_of_size<32> {
	static const bool is_specialised = true;
	static const uint8_t size = 32;
	using value_type    = int32_t;
	using signed_type   = int32_t;
	using unsigned_type = uint32_t;
	using next_type     = type_of_size<64>;
};

template<>
struct type_of_size<16> {
	static const bool is_specialised = true;
	static const uint8_t size = 16;
	using value_type    = int16_t;
	using signed_type   = int16_t;
	using unsigned_type = uint16_t;
	using next_type     = type_of_size<32>;
};

template<>
struct type_of_size<8> {
	static const bool is_specialised = true;
	static const uint8_t size = 8;
	using value_type    = int8_t;
	using signed_type   = int8_t;
	using unsigned_type = uint8_t;
	using next_type     = type_of_size<16>;
};

}
