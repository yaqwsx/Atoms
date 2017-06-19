#include <atoms/communication/avakar.h>
#include <atoms/utility_dynamic.h>
#include <iostream>
#include <algorithm>

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek


// This example demonstrates usage of AvakarPacket class. This class allows user
// to easily create and parse Avakar's packets

using namespace atoms;

int main() {
    AvakarPacket out; // Create empty packet
    std::cout << "=== Creating a packet ===\n";
    std::cout << "Empty packet:      " << hex_dump(out.raw(), out.raw_size()) << "\n";
    out.set_command(8);
    std::cout << "Set command to 8:  " << hex_dump(out.raw(), out.raw_size()) << "\n";
    out.push<char>(42);
    std::cout << "Push 42 as char:   " << hex_dump(out.raw(), out.raw_size()) << "\n";
    out.push<uint32_t>(42);
    std::cout << "Push 42 as uint32: " << hex_dump(out.raw(), out.raw_size()) << "\n";
    out.push<float>(42);
    std::cout << "Push 42 as float:  " << hex_dump(out.raw(), out.raw_size()) << "\n";
    char test[] = { 'A', 'B' };
    out.push_n<char>(test, sizeof(test));
    std::cout << "Push string \"AB\":  " << hex_dump(out.raw(), out.raw_size()) << "\n";

    std::cout << std::hex;
    for(int ch: out) {
       std::cout << ch ; 
    }
    std::cout << std::dec;
    
    // Add garbage to the output and parse it again
    std::cout << "\n=== Reading a packet ===\n";
    char input[18];
    std::fill_n(input, sizeof(input), 0x42);
    std::copy_n(out.raw(), out.raw_size(), input + 2);
    std::cout << "Input data: " << hex_dump(input, sizeof(input)) << "\n";

    AvakarPacket in; // Create empty packet
    int i = 0;
    while (!in.push_byte(input[i])) // Simulate incoming data
        i++;
    std::cout << i << " bytes read to parse a packet\n";
    std::cout << "Command:      " << (int)in.get_command() << "\n";
    std::cout << "Data size:    " << (int)in.size() << "\n";
    // To get value out of the packet, simply specify type and byte offset
    std::cout << "Char value:   " << (int)in.get<char>(0) << "\n";
    std::cout << "uint32 value: " << in.get<uint32_t>(1) << "\n";
    std::cout << "float value:  " << in.get<float>(5) << "\n";
    std::cout << "string:       " << in.get<char>(9) << in.get<char>(10) << "\n";
    
    // Note: To reuse packet in, call in.reset()
    return 0;
}