#include <atoms/numeric/fixed.h>
#include <atoms/numeric/value.h>
#include <iostream>

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek


// This example demonstrates usage of fixed point numbers 

using namespace atoms;

int main() {
    std::cout << "=== Q16.16 ===\n";
    Fixed<16, 16> a(1.0f);
    Fixed<16, 16> b(-4.2f);
    std::cout << "a + b = " << (a + b).to_float() << "\n";
    
}