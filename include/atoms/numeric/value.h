#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#ifdef ATOMS_NO_STDLIB
    #include "../stdlib/algorithm.h"
#else
    #include <algorithm>
#endif

namespace atoms {

// Internal implementation of Value
template <class ValType, template <class> class... Modifiers>
class ValueMod {
protected:
    ValType initial(const ValType& v) {
        return v;
    }

    ValType set(const ValType& value) {
        return value;
    }

    template <class Time>
    ValType set(const ValType& value, const Time&) {
        return value;
    }
};

// Internal implementation of Value
template <class ValType, template <class> class T, template <class> class... Modifiers>
class ValueMod<ValType, T, Modifiers...> : ValueMod<ValType, Modifiers...> {
public:
    ValueMod(T<ValType> modifier, Modifiers<ValType>... modifiers)
        : ValueMod<ValType, Modifiers...>(modifiers...), modifier(modifier) {}
protected:
    ValType initial(const ValType& value) {
        auto new_val = modifier.process(value);
        return ValueMod<ValType, Modifiers...>::initial(new_val);
    }

    ValType set(const ValType& value) {
        auto new_value = modifier.process(value);
        return ValueMod<ValType, Modifiers...>::set(new_value);
    }

    template <class Time>
    ValType set(const ValType& value, const Time& t) {
        auto new_value = modifier.process(value, t);
        return ValueMod<ValType, Modifiers...>::set(new_value, t);
    }

private:
    T<ValType> modifier;
};

// Value - represents a value that can be read or stored. When store,
// given modifiers are applied.
template <class ValType, template <class> class... Modifiers>
class Value : ValueMod<ValType, Modifiers...> {
public:
    Value(ValType val, Modifiers<ValType>... modifiers)
        : ValueMod<ValType, Modifiers...>(modifiers...)
    {
        value = ValueMod<ValType, Modifiers...>::initial(val);
    }

    void set(const ValType& new_value) {
        value = ValueMod<ValType, Modifiers...>::set(new_value);
    }

    template <class Time>
    void set(const ValType& new_value, const Time& t) {
        value = ValueMod<ValType, Modifiers...>::set(new_value, t);
    }

    operator ValType() const {
        return value;
    }

    ValType get() const {
        return value;
    }

    Value& operator=(const Value&) = default;

    Value& operator=(const ValType& new_value) {
        set(new_value);
        return *this;
    }
    
    void reset(const ValType& val) {
        ValueMod<ValType, Modifiers...>::initial(val);
    }

private:
    ValType value;
};

// Accelerated modifier - the value cannot rise or fall faster than specified
template <class T>
struct Accelerated {
    T acceleration, decceleration;
    T value;

    T initial(T initial_value) {
        value = initial_value;
        return value;
    }

    T process(const T& new_value) {
        if (new_value > value)
            value = std::min(new_value, value + acceleration);
        else
            value = std::max(new_value, value - decceleration);
        return value;
    }

    template <class S>
    T process(const T& new_value, const S& step) {
        if (new_value > value)
            value = std::min(new_value, T(value + acceleration * step));
        else
            value = std::max(new_value, T(value - decceleration * step));
        return value;
    }
};

// Modifier for Value - keeps value withing defind range
template <class T>
struct Clamped {
    T bottom, top;

    T initial(T initial_value) {
        return process(initial_value);
    }

    T process(const T& new_value) {
        if (new_value > top)
            return top;
        if (new_value < bottom)
            return bottom;
        return new_value;
    }

    template <class S>
    T process(const T& new_value, const S&) {
        return process(new_value);
    }
};

}