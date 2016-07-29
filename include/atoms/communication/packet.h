#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include <stdint.h>
#if defined(ATOMS_NO_STDLIB) || defined(ATOMS_NO_STDLIB11)
    #include "../stdlib/type_traits.h"
#else
    #include <type_traits>
#endif
#include "../type/has_member.h"
#include "../utility_static.h"


namespace atoms {

// Declare test for presence of methods
GENERATE_HAS_MEMBER(init)
GENERATE_HAS_MEMBER(increase_size)
GENERATE_HAS_MEMBER(get_data_size)
GENERATE_HAS_MEMBER(set_command)
GENERATE_HAS_MEMBER(get_command)
GENERATE_HAS_MEMBER(set_address)
GENERATE_HAS_MEMBER(get_address)
GENERATE_HAS_MEMBER(push_data)
GENERATE_HAS_MEMBER(get_data)

template <size_t SIZE, size_t DATA_SIZE, class... Objects>
class PacketPart {
public:
    PacketPart() : counter(0), loaded(0) {};
    bool push_byte(uint8_t, size_t, size_t) { return true; };
    bool prepare(size_t, size_t) { return false; }
protected:
    constexpr static size_t static_size  = SIZE;
    constexpr static size_t data_size    = DATA_SIZE;
    constexpr static size_t object_count = 0;
    constexpr static bool   has_size     = false;
    constexpr static bool   has_command  = false;
    constexpr static bool   has_address  = false;
    constexpr static bool   has_data     = false;

    uint8_t buffer[SIZE + DATA_SIZE];
    size_t counter;
    size_t loaded;
};

template <size_t OFFSET, size_t DATA_SIZE, class Object, class... Objects>
class PacketPart<OFFSET, DATA_SIZE, Object, Objects...>
    : public PacketPart<OFFSET + Object::static_size,
                        DATA_SIZE + Object::data_size, Objects...>
{
public:
    PacketPart() {
        Object::init(Base::buffer + OFFSET);
    }

    // Current object support command
    template <class T, class S = Object>    
    void set_command(const T& command, typename std::enable_if_t<has_member_set_command<S>::value>* = nullptr)
    {
        Object::set_command(Base::buffer + OFFSET, command);
    }

    // Current object doesn't support command
    template <class T, class S = Object> 
    void set_command(const T& command, typename std::enable_if_t<!has_member_set_command<S>::value>* = nullptr)
    {
        Parent::set_command(command);
    }

    // Current object support command
    template <class T, class S = Object>
    T get_command(typename std::enable_if_t<has_member_get_command<S>::value>* = nullptr) const {
        return Object::get_command(Base::buffer + OFFSET);
    }

    // Current object doesn't support command
    template <class T, class S = Object>
    T get_command(typename std::enable_if_t<!has_member_get_command<S>::value>* = nullptr) const {
        return Parent::template get_command<T>();
    }

    // Current object support address
    template <class T, class S = Object>
    void set_address(const T& address, typename std::enable_if_t<has_member_set_address<S>::value>* = nullptr) {
        Object::set_address(Base::buffer + OFFSET, address);
    }

    // Current object doesn't support address
    template <class T, class S = Object>
    void set_address(const T& address, typename std::enable_if_t<!has_member_set_address<S>::value>* = nullptr) {
        Parent::set_address(address);
    }

    // Current object support address
    template <class T, class S = Object>
    T get_address(typename std::enable_if_t<has_member_get_address<S>::value>* = nullptr) const {
        return Object::get_address(Base::buffer + OFFSET);
    }

    // Current object doesn't support address
    template <class T, class S = Object>
    T get_address(typename std::enable_if_t<!has_member_get_address<S>::value>* = nullptr) const {
        return Parent::template get_address<T>();
    }

    // Current object support size
    template <class T, class S = Object>
    void increase_size(const T& size, typename std::enable_if_t<has_member_increase_size<S>::value>* = nullptr) {
        Object::increase_size(Base::buffer + OFFSET, size);
    }

    // Current object doesn't support size
    template <class T, class S = Object>
    void increase_size(const T& size, typename std::enable_if_t<!has_member_increase_size<S>::value>* = nullptr) {
        Parent::increase_size(size);
    }

    // Current object support size
    template <class T, class S = Object>
    T get_data_size(typename std::enable_if_t<has_member_get_data_size<S>::value>* = nullptr) const {
        return Object::get_data_size(Base::buffer + OFFSET);
    }

    // Current object doesn't support size
    template <class T, class S = Object>
    T get_data_size(typename std::enable_if_t<!has_member_get_data_size<S>::value>* = nullptr) const {
        return Parent::template get_data_size<T>();
    }

    // Current object support data
    template <class T, class Packet, class S = Object>
    void push(const T& data, Packet& p, typename std::enable_if_t<has_member_push_data<S>::value>* = nullptr) {
        Object::template push_data<T, Packet>(Base::buffer + OFFSET, data, p);
    }

    // Current object doesn't support data
    template <class T, class Packet, class S = Object>
    void push(const T& data, Packet& p, typename std::enable_if_t<!has_member_push_data<S>::value>* = nullptr) {
        Parent::template push<T, Packet>(data, p);
    }

    // Current object support data
    template <class T, class S = Object>
    T get(size_t offset, typename std::enable_if_t<has_member_get_data<S>::value>* = nullptr) const {
        return Object::template get_data<T>(Base::buffer + OFFSET, offset);
    }

    // Current object doesn't support data
    template <class T, class S = Object>
    T get(size_t offset, typename std::enable_if_t<!has_member_get_data<S>::value>* = nullptr) const {
        return Parent::template get<T>(offset);
    }

    bool push_byte(uint8_t c, size_t count, size_t size) {
        if (count != 0) {
            return Parent::push_byte(c, count - 1, size);
        }
        return Object::push_byte(c, Base::buffer + OFFSET, Base::counter, size);
    }
    
    bool prepare(size_t count, size_t size) {
        if (count != 0)
            return Parent::prepare(count - 1, size);
        return Object::prepare(size);
    }

protected:
    using Parent = PacketPart<OFFSET + Object::static_size,
        DATA_SIZE + Object::data_size, Objects...>;
    using Base = PacketPart<Parent::static_size, Parent::data_size>;

    constexpr static size_t object_count = Parent::object_count + 1;

    constexpr static bool has_size    = Parent::has_size |
        (has_member_increase_size<Object>::value && has_member_get_data_size<Object>::value);
    constexpr static bool has_command = Parent::has_command |
        (has_member_set_command<Object>::value && has_member_get_command<Object>::value);
    constexpr static bool has_address = Parent::has_address |
        (has_member_set_address<Object>::value && has_member_get_address<Object>::value);
    constexpr static bool has_data    = Parent::has_data |
        (has_member_push_data<Object>::value && has_member_get_data<Object>::value);
};

template <class... Objects>
class Packet : public PacketPart<0, 0, Objects...> {
private:
    using Parent = PacketPart<0, 0, Objects...>;
    using Base = PacketPart<Parent::static_size, Parent::data_size>;
public:
    Packet() {}

    constexpr static size_t get_static_size() {
        return Parent::static_size;
    }

    const char* get_static_raw() const {
        return reinterpret_cast<const char*>(&Base::buffer[0]);
    }

    template <class S = Parent>
    typename std::enable_if<S::has_size, size_t>::
    type get_size() const {
        return get_static_size() + get_data_size(); // ToDo: Add support fo unbounded data buffers
    }

    template <class S = Parent>
    typename std::enable_if<!S::has_size, size_t>::
    type get_size() const {
        return get_static_size();
    }

    template <class T = char>
    const T* get_raw() const {
        return reinterpret_cast<const T*>(get_static_raw());
        // ToDo: Add support for unbounded data buffers
    }

    template <class T, class S = Parent>
    typename std::enable_if<S::has_command>::
    type set_command(const T& command) {
        Parent::set_command(command);
    }

    template <class T = size_t, class S = Parent>
    typename std::enable_if<S::has_command, T>::
    type get_command() const {
        return Parent::template get_command<T>();
    }

    template <class T, class S = Parent>
    typename std::enable_if<S::has_address>::
    type set_address(const T& address) {
        Parent::template set_address(address);
    }

    template <class T = size_t, class S = Parent>
    typename std::enable_if<S::has_address, T>::
    type get_address() const {
        return Parent::template get_address<T>();
    }

    template <class T, class S = Parent>
    typename std::enable_if<S::has_size>::
    type increase_size(const T& inc) {
        Parent::increase_size(inc);
    }

    template <class T = size_t, class S = Parent>
    typename std::enable_if<S::has_size, T>::
    type get_data_size() const {
        return Parent::template get_data_size<T>();
    }

    template <class T, class S = Parent>
    typename std::enable_if<S::has_data>::
    type push(const T& t) {
        Parent::template push<T>(t, *this);
    }

    template <class T, class S = Parent>
    typename std::enable_if<S::has_data, T>::
    type get(size_t offset) const {
        return Parent::template get<T>(offset);
    }

    bool push_byte(uint8_t c) {
        if (Parent::push_byte(c, Base::loaded, get_data_size())) {
            Base::loaded++;
            Base::counter = 0;
            while (Parent::prepare(Base::loaded, get_data_size()))
                Base::loaded++; // Skip parts with no length
        }
        return complete();
    }

    bool complete() {
        return Base::loaded == Parent::object_count;
    }
};

// Static field for value
template <class T, T VALUE>
struct Static {
    constexpr static size_t static_size = sizeof(T);
    constexpr static size_t data_size   = 0;

    static void init(uint8_t* buffer) {
        store<T>(buffer, VALUE);
    }

    static bool push_byte(uint8_t c, uint8_t* buffer, size_t& counter, size_t) {
        if (c == buffer[counter])
            counter++;
        return counter == static_size;
    }
    
    static bool prepare(size_t) {
        return false;
    }
};

template <uint8_t VALUE>
using StaticU8 = Static<uint8_t, VALUE>;

template <uint16_t VALUE>
using StaticU16 = Static<uint16_t, VALUE>;

template <uint32_t VALUE>
using StaticU32 = Static<uint32_t, VALUE>;

// Field for command representation
template <class T>
struct Command {
    constexpr static size_t static_size = sizeof(T);
    constexpr static size_t data_size = 0;

    static void init(uint8_t* buffer) {
        store<T>(buffer, 0);
    }

    static void set_command(uint8_t* buffer, const T& command) {
        store<T>(buffer, command);
    }

    static size_t get_command(const uint8_t* buffer) {
        return load<T>(buffer);
    }

    static bool push_byte(uint8_t c, uint8_t* buffer, size_t& counter, size_t) {
        buffer[counter] = c;
        counter++;
        return counter == static_size;
    }
    
    static bool prepare(size_t) {
        return false;
    }
};

using CommandU8 = Command<uint8_t>;
using CommandU16 = Command<uint16_t>;
using CommandU32 = Command<uint32_t>;

// Field for address representation
template <class T>
struct Address {
    constexpr static size_t static_size = sizeof(T);
    constexpr static size_t data_size = 0;

    static void init(uint8_t* buffer) {
        store<T>(buffer, 0);
    }

    static void set_address(uint8_t* buffer, const T& address) {
        store<T>(buffer, address);
    }

    static size_t get_address(const uint8_t* buffer) {
        return load<T>(buffer);
    }

    static bool push_byte(uint8_t c, uint8_t* buffer, size_t& counter, size_t) {
        buffer[counter] = c;
        counter++;
        return counter == static_size;
    }
    
    static bool prepare(size_t) {
        return false;
    }
};

using AddressU8 = Address<uint8_t>;
using AddressU16 = Address<uint16_t>;
using AddressU32 = Address<uint32_t>;

// Field for size representation
template <class T>
struct Size {
    constexpr static size_t static_size = sizeof(T);
    constexpr static size_t data_size = 0;

    static void init(uint8_t* buffer) {
        T* ptr = reinterpret_cast<T*>(buffer);
        *ptr = 0;
    }

    static void increase_size(uint8_t* buffer, const T& inc) {
        T val = load<T>(buffer);
        val += inc;
        store<T>(buffer, val);
    }

    static size_t get_data_size(const uint8_t* buffer) {
        return load<T>(buffer);
    }

    static bool push_byte(uint8_t c, uint8_t* buffer, size_t& counter, size_t) {
        buffer[counter] = c;
        counter++;
        return counter == static_size;
    }
    
    static bool prepare(size_t) {
        return false;
    }
};

using SizeU8 = Size<uint8_t>;
using SizeU16 = Size<uint16_t>;
using SizeU32 = Size<uint32_t>;

// Dynamic field representing data with bounded size
template<size_t SIZE>
struct BoundedData {
    constexpr static size_t static_size = 0;
    constexpr static size_t data_size   = SIZE;

    static void init(uint8_t*) { }

    template <class T, class Packet>
    static void push_data(uint8_t* buffer, const T& data, Packet& p) {
        store<T>(buffer + p.get_data_size(), data);
        p.increase_size(sizeof(T));
    }

    template <class T>
    static T get_data(const uint8_t* buffer, size_t offset) {
        return load<T>(buffer + offset);
    }

    static bool push_byte(uint8_t c, uint8_t* buffer, size_t& counter, size_t size) {
        buffer[counter] = c;
        counter++;
        return counter == size;
    }
    
    static bool prepare(size_t size) {
        return size == 0;
    }
};

}