#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

// Atmel Studio and the others doesn't ship STL library, this is a small
// and humble replacement

namespace std {
	template <class T>
    T min(const T& a, const T& b) {
        return a < b ? a : b;
    }
    
    template <class T>
    T max(const T& a, const T& b) {
        return a > b ? a : b;
    }
}