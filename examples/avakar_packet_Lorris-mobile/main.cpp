#include <atoms/communication/avakar.h>
#include <atoms/utility_dynamic.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jarek Páral

// This example demonstrates usage of AvakarPacket class with Android app Lorris mobile - tab Joystick - protocol Avakar's. 
// Lorris mobile: https://play.google.com/store/apps/details?id=com.tassadar.lorrismobile

// For standard environment
#define PRINT_WARNING(x) do {std::cerr << "[Warning] "<< x;} while(false)
// For Arduino environment
// #define PRINT_WARNING(x) do {Serial.print("[Warning] "); Serial.println(x);} while(false)

class Joystick {   
    int m_axe[4] = {0};
    int m_buttons = 0;
    atoms::AvakarPacket packet;

public:
    bool onByte(int byte) {
        if(byte == -1) {
            PRINT_WARNING("Byte is -1. Check if you are not reading empty stream/Serial/...");
        }
        if(packet.push_byte(byte)) {
            if((packet.get_command() == 1) && (packet.size() == 9)) {
                for(int i = 0; i < 4; ++i) {
                    m_axe[i] = packet.get<int16_t>(i*2);
                }
                m_buttons = packet.get<uint8_t>(8);
                packet.clear();
                return true;
            } else {
                PRINT_WARNING("Unknown packet");
            }
        }
        return false;
    }

    int axe(int index) {
        if(index < 0 || index > 3) {
            PRINT_WARNING("axe index - out of range");
            return 0;
        }
        return m_axe[index];
    }

    int buttons() {
        return m_buttons;
    }

    bool button(int index) {
        if(index < 0 || index > 7) {
            PRINT_WARNING("button index - out of range");
            return false;
        }
        return m_buttons & (1 << index);
    }
};

int main() {    
    // Avakar's packet: prefix (0x80), command (4 bit = 1), lenght (4 bit = 9), 4x axe (4x int16_t), 8x button (1x uint8_t)
    std::vector<int> data = {0x80, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0b11111111,    // first Avakar's packet 
                             0x80, 0x19, 0x40, 0x40, 0xFF, 0x7F, 0x40, 0x40, 0xFF, 0x7F, 0b01010101,    // second Avakar's packet
                             0x80, 0x19, 0xFF, 0x7F, 0xFF, 0x7F, 0xFF, 0x7F, 0xFF, 0x7F, 0b00000000};   // third Avakar's packet

    Joystick joy;

    for(int byte : data) {
        if(joy.onByte(byte)) {
            for(int index = 0; index < 4; ++index) {
                std::cout << "axe" << index << ": " << std::setw(5) << joy.axe(index) << "   ";
            }
            for(int index = 0; index < 8; ++index) {
                std::cout << "b" << index << ": " << joy.button(index) << "   ";
            }
            std::cout << "buts: " << joy.buttons();
            std::cout << std::endl;
        }
    }
    return 0;
}