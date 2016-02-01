#include <atoms/numeric/rolling_average.h>
#include <iostream>

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek


// This example demonstrates usage of average computation helpers

using namespace atoms;

int main() {
    std::cout << "=== Rolling average ===\n";
    RollingAverage<double, 5> average;
    std::cout << "Value:   " << average.get_average() << "\n";
    average.push(5);
    std::cout << "Push 5:  " << average.get_average() << "\n";
    average.push(5);
    std::cout << "Push 5:  " << average.get_average() << "\n";
    average.push(5);
    std::cout << "Push 5:  " << average.get_average() << "\n";
    average.push(5);
    std::cout << "Push 5:  " << average.get_average() << "\n";
    average.push(5);
    std::cout << "Push 5:  " << average.get_average() << "\n";
    average.push(-5);
    std::cout << "Push -5: " << average.get_average() << "\n";
}