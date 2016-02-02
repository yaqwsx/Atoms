#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include <array>
#include <initializer_list>
#include <algorithm>

namespace atoms {

template <class T, size_t SIZE>
class RollingAverage {
public:
    RollingAverage() : sum(0), index(0)
    {
        std::fill(values.begin(), values.end(), T(0));
    };

    RollingAverage(const std::initializer_list<T>& l) : sum(0), index(0)
    {
        std::copy(l.begin(), l.end(), values.begin());
    }

    void push(const T& t) {
        sum += t - values[index];
        values[index] = t;
        index++;
        if (index == SIZE)
            index = 0;
    }

    T get_average() {
        return sum / T(SIZE);
    }

    T get_sum() {
        return sum;
    }
private:
    std::array<T, SIZE> values;
    T sum;
    size_t index;
};

}