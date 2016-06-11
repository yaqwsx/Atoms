#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include <cmath>

namespace atoms {

template <class T>
struct NumEq {
    static bool equal(const T& a, const T& b) {
        return a == b;
    }
};

template<>
struct NumEq<float> {
    static bool equal(const float& a, const float& b) {
        return fabs(a - b) < 0.00001; // Hack: use Google's AlmostEqual?
    }
};

template<>
struct NumEq<double> {
    static bool equal(const double& a, const double& b) {
        return fabs(a - b) < 0.0000001; // Hack: use Google's AlmostEqual?
    }
};

} // namespace atoms    