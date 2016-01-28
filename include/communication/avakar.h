#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include <stdint.h>

namespace atoms {

template <class CharType>
class AvakarPacketTempl {
public:
    AvakarPacketTempl() : buffer{0x80, 0}, state(State::EMPTY), idx(2) {}
    
    CharType raw_size() const {
        return 2 + size();
    }
    
    CharType size() const {
        return buffer[1] & 0xF;
    }
    
    const CharType* raw() {
        return reinterpret_cast<CharType*>(&buffer[0]);
    }
    
    CharType get_command() const {
        return buffer[1] >> 4;
    }
    
    void set_command(CharType command) {
        buffer[1] &= 0xF;
        buffer[1] |= command << 4;
    }
    
    template <class T>
    void push(T data) {
        T* ptr = reinterpret_cast<T*>(buffer + raw_size());
        *ptr = data;
        increase_size(sizeof(T));
    }
    
    bool push_byte(CharType c) {
        uint8_t cc = c;
        switch(state) {
        case State::EMPTY:
            if (cc == 0x80)
                state = State::HEADER;
            break;
        case State::HEADER:
            buffer[1] = cc;
            state = State::COMMAND;
            break;
        case State::COMMAND:
        case State::IN_PROGRESS:
            buffer[idx] = cc;
            idx++;
            if (idx == raw_size())
                state = State::DONE;
        }
        return complete();
    }
    
    bool complete() {
        return state == State::DONE;
    }
    
    template <class T>
    T get(uint8_t index) {
        T* ptr = reinterpret_cast<T*>(buffer + 2 + index);
        return *ptr;
    }
private:
    enum class State { EMPTY, HEADER, COMMAND, IN_PROGRESS, DONE };
    
    uint8_t buffer[2 + 16];
    State state;
    uint8_t idx;
    
    void increase_size(CharType size) {
        buffer[1] += size;
    }
};

using AvakarPacketS = AvakarPacketTempl<int8_t>;
using AvakarPacketU = AvakarPacketTempl<uint8_t>;
using AvakarPacket = AvakarPacketTempl<char>;
 
}