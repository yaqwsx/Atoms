#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include <stdint.h>

namespace atoms {

// Sugar for register setup
template <class T>
T set_bits(uint8_t arg) {
	return T(1) << arg;
}

template <class T, class...Args>
T set_bits(uint8_t arg, Args...args) {
	return (T(1) << arg) | set_bits<T>(args...);
}

template <class...Args>
uint8_t set_bits8(Args...args) {
	return set_bits<uint8_t>(args...);
}

template <class...Args>
uint16_t set_bits16(Args...args) {
	return set_bits<uint16_t>(args...);
}

template <class...Args>
uint32_t set_bits32(Args...args) {
	return set_bits<uint32_t>(args...);
}

}