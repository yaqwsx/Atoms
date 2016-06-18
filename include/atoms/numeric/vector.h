#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include <cmath>
#include "equality.h"

namespace atoms {

template <class T, class Eq = NumEq<T>>
struct Vector2D {
    T x, y;

    Vector2D(T x = T(), T y = T()) : x(x), y(y) {}

    T length() {
        return sqrt(x * x + y * y);
    }

    bool operator==(const Vector2D& o) const {
        return Eq::equal(x, o.x) && Eq::equal(y, o.y);
    }

    bool operator!=(const Vector2D& o) const {
        return *this != o;
    }

    Vector2D operator-() const {
        return { -x, -y };
    }

    Vector2D operator+() const {
        return *this;
    }

    Vector2D& operator+=(const Vector2D& o) {
        x += o.x;
        y += o.y;
        return *this;
    }

    Vector2D operator+(const Vector2D& o) const {
        return { x + o.x, y + o.y };
    }

    Vector2D& operator-=(const Vector2D& o) {
        x -= o.x;
        y -= o.y;
        return *this;
    }

    Vector2D operator-(const Vector2D& o) const {
        return { x - o.x, y - o.y };
    }

    T operator*(const Vector2D& o) const {
        return x * o.x + y * o.y;
    }

    template <class O>
    Vector2D& operator*=(const O& o) {
        x *= o;
        y *= o;
        return *this;
    }

    template <class O>
    Vector2D operator*(const O& o) const {
        return { x * o, y * o };
    }

    template <class O>
    Vector2D& operator/=(const O& o) {
        x /= o;
        y /= o;
        return *this;
    }

    template <class O>
    Vector2D operator/(const O& o) const {
        return { x / o, y / o };
    }

    Vector2D normalized() {
        auto l = length();
        return { x / l, y / l };
    }
};

template <class O, class T>
Vector2D<T> operator*(const O& o, const Vector2D<T>& v) {
    return { o * v.x, o * v.y };
}

template <class O, class T>
Vector2D<T> operator/(const O& o, const Vector2D<T>& v) {
    return { o / v.x, o / v.y };
}

} // namespace atoms    