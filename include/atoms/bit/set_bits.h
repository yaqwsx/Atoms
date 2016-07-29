#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include <stdint.h>

namespace atoms {

// Sugar for register setup
template <class T>
constexpr T set_bits(uint8_t arg) {
	return T(1) << arg;
}

template <class T, class...Args>
constexpr T set_bits(uint8_t arg, Args...args) {
	return (T(1) << arg) | set_bits<T>(args...);
}

template <class...Args>
constexpr uint8_t set_bits8(Args...args) {
	return set_bits<uint8_t>(args...);
}

template <class...Args>
constexpr uint16_t set_bits16(Args...args) {
	return set_bits<uint16_t>(args...);
}

template <class...Args>
constexpr uint32_t set_bits32(Args...args) {
	return set_bits<uint32_t>(args...);
}

}