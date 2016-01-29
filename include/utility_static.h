#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

namespace atoms {

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