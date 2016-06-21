#include <atoms/type/tagged.h>
#include <atoms/numeric/vector.h>
#include <iostream>
#include <string>

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek


// This example demonstrates usage of Tagged class. This class allows user
// to keep a tag with an arbitrary data structure

using namespace atoms;

template <class V>
std::string to_str(V v) {
    return "[" + std::to_string(v.x) + ", " + std::to_string(v.y) + "]";
}

template <class V>
std::string to_str_v(V v) {
    return "[" + std::to_string(v.x) + ", " + std::to_string(v.y) +
        ", " + std::to_string(v.tag) + "]";
}

int main() {
    Tagged<Vector2D<int>> v(5, 2), u(4, 2); // Vectors with no tag
    std::cout << "v = " << to_str(v) << ", u = " << to_str(u) << "\n";

    auto t = v;
    Tagged<Vector2D<int>> tt(v);
    std::cout << "assign = " << to_str(t) << ", copy = " << to_str(tt) << "\n";

    std::cout << "u + v =" << to_str(u + v) << "\n";

    using V2T = Tagged<Vector2D<int>, int, OperatorMerge<int>>;
    auto a = V2T::make_tagged(42, 1, 1);
    auto b = V2T::make_tagged(10, -1, -1);
    std::cout << "a = " << to_str_v(a) << ", b = " << to_str_v(b) << "\n";
    std::cout << "a + b = " << to_str_v(a + b) << "\n";

    a -= b;
    std::cout << "Modified a = " << to_str_v(a) << "\n";

    using V2T2 = Tagged<Vector2D<int>, int, KeepLeftMerge<int>>;
    auto c = V2T2::make_tagged(1, 1, 1);
    std::cout << "42 * c: " << to_str_v(42 * c) << "\n";

    return 0;
}