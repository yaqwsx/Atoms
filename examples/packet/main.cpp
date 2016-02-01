#include <atoms/communication/packet.h>
#include <atoms/utility_dynamic.h>
#include <iostream>
#include <algorithm>

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek


// This example demonstrates usage of Packet class. This class allows user
// to easily create and parse arbitrary structured packets.

using namespace atoms;

int main() {
    // We define our own packet structure. If none of the predefined components
    // fits the needs, a new component can be defined. Only bounded data buffers
    // are currently supported (here you can create a packet with up to 32 bytes
    // of data)
    using MyPacket = Packet <
        StaticU8<0x80>, CommandU8, AddressU8, SizeU8, BoundedData < 32 >>;

    std::cout << "=== Creating a packet ===\n";
    MyPacket out;
    std::cout << "Empty:           " << hex_dump(out.get_raw(), out.get_size()) << "\n";
    out.set_command(0xF);
    std::cout << "Command to 0x0f: " << hex_dump(out.get_raw(), out.get_size()) << "\n";
    out.set_address(0x1);
    std::cout << "Address to 0x01: " << hex_dump(out.get_raw(), out.get_size()) << "\n";
    out.push<uint32_t>(42);
    std::cout << "Add uint32_t 42: " << hex_dump(out.get_raw(), out.get_size()) << "\n";
    out.push<float>(42);
    std::cout << "Add float 42:    " << hex_dump(out.get_raw(), out.get_size()) << "\n";

    std::cout << "=== Reading a packet ===\n";
    const    uint8_t* data = out.get_raw<uint8_t>();
    MyPacket in;

    size_t i = 0;
    while (!in.push_byte(data[i]))
        i++;

    std::cout << "Size:         " << in.get_size() << "\n";
    std::cout << "Command:      " << in.get_command() << "\n";
    std::cout << "Address:      " << in.get_address() << "\n";
    std::cout << "First field:  " << in.get<uint32_t>(0) << "\n";
    std::cout << "Second field: " << in.get<float>(4) << "\n";

    std::cout << "=== Packet with no data ===\n";
    uint8_t data2[] = { 0x80, 0x0f, 0x01, 0x00 };
    MyPacket in2;

    i = 0;
    while (!in2.push_byte(data2[i]))
        i++;

    std::cout << "Size:         " << in2.get_size() << "\n";
    std::cout << "Command:      " << in2.get_command() << "\n";
    std::cout << "Address:      " << in2.get_address() << "\n";

    using AppPacket = Packet<StaticU8<0x80>, SizeU8, CommandU8, BoundedData<48>>;
    AppPacket t;
    t.get<float>(0);

    return 0;
}