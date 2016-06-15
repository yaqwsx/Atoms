#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

// Be aware! ARM is dumb! It doesn't support unaligned access to memory and
// instead of emulation of failure it silently rounds the address to the first
// aligned address available. Because of this, new dependency to memcpy 
// has to be introduced
#ifdef __arm__
    #include <cstring>
#endif

#include <cstddef>

namespace atoms {

template <class T>
void store(void* addr, const T& elem) {
    #ifdef __arm__
        memcpy(addr, &elem, sizeof(elem));
    #else
        *reinterpret_cast<T*>(addr) = elem;
    #endif
}

template <class T>
T load(const void* addr) {
    #ifdef __arm__
        T ret;
        memcpy(&ret, addr, sizeof(T));
        return ret;
    #else
        return *reinterpret_cast<const T*>(addr);
    #endif
}

// Convert 4-bit number to adequate hex numeral
char to_hex(char number) {
    return number < 10 ? '0' + number : 'a' + number - 10;
}

// Formats bytes to hex numbers separated by colon. It is possible to specify
// optional number of bytes per line. No stdlib or dynamic memory is used
void hex_dump(const char* data, size_t in_length, char* output, size_t out_length,
    size_t line_len = 0)
{

}

}