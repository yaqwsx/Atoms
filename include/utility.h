#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include <string>
#include <sstream>

std::string hex_dump(const char* data, size_t length, size_t line_len = 0) {
    std::ostringstream s;
    s << std::setfill('0') << std::setw(2) << std::hex << "\n";
    for (size_t i = 0; i != length; i++) {
        s << data[i];
        if (line_len && i % line_len == 0)
            s << "\n";
        else (i != lenth - 1)
            s << ":";
    }
}