#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include <string>
#include <iomanip>
#include <sstream>

namespace atoms {

// Formats bytes to hex numbers separated by colon. It is possible to specify
// optional number of bytes per line
std::string hex_dump(const char* data, size_t length, size_t line_len = 0) {
    std::ostringstream s;
    s << std::hex;
    for (size_t i = 0; i != length; i++) {
        unsigned char c = data[i];
        if (c < 0x10)
            s << '0';
        s << (int)c;
        if (line_len != 0 && i % line_len == 0)
            s << "\n";
        else if (i != length - 1)
            s << ":";
    }
    return s.str();
}

}