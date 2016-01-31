#include <atoms/numeric/value.h>
#include <iostream>

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek


// This example demonstrates usage of Value class. This class allows user
// to keep value with several restriction - like keeping the value in range
// or limiting the rate of change.

using namespace atoms;

int main() {
    // Create int value with initial value 0, that is always in range <-100, 100>
    std::cout << "=== Clamped value ===\n";
    Value<int, Clamped> a(0, { -100, 100 });
    std::cout << "Initial value: " << a << "\n";
    a = 50; // 50
    std::cout << "Set it to 50: " << a << "\n";
    a = a + 150; // 100
    std::cout << "Increase it by 100: " << a << "\n";
    a = a - 80; // 20
    std::cout << "Decrease it by 80: " << a << "\n";
    a = a - 200; // -100
    std::cout << "Decrease it by 200: " << a << "\n";

    // Create value with limited rate of change: 50 units/per time unit to
    // accelerate and 20 units/per time unit to decelerate
    std::cout << "=== Accelerated value ===\n";
    Value<float, Accelerated> b(0, { 50, 20 });
    std::cout << "Initial value: " << b << "\n";
    b = 40; // 40
    std::cout << "Set it to 40: " << b << "\n";
    b = 100; // 90
    std::cout << "Set it to 100: " << b << "\n";
    b = 0; // 70
    std::cout << "Set it to 0: " << b << "\n";
    // Now make a half step
    b.set(0, 0.5); // 60
    std::cout << "Setting it to 0 with half step: " << b << "\n";
    b.set(0, 0.1); // 58
    std::cout << "Setting it to 0 with one tenth of step: " << b << "\n";

    // You can also apply any number of modifiers. They are applied in the same
    // order they appear
    std::cout << "=== Combined value ===\n";
    Value<int, Clamped, Accelerated> c(10, { -10, 30 }, { 10, 20 });
    std::cout << "Initial value: " << c << "\n";
    c = 40; // 20
    std::cout << "Setting it to 40: " << c << "\n";
    c = 40; // 30
    std::cout << "Setting it to 40: " << c << "\n";
    c = 40; // 30
    std::cout << "Setting it to 40: " << c << "\n";

    return 0;
}