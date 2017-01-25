#include <atoms/functional/delegate.h>
#include <iostream>
#include <vector>

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

using namespace atoms;

void foo( int x ) {
    std::cout << "function invoked with: " << x << "\n";
}

struct Foo {
    int y;

    Foo( int y ) : y(y) {}

    static void f( int x ) {
        std::cout << "static method invoked with: " << x << "\n";
    }

    void g( int x ) {
        std::cout << "method invoked with: " << x
            << " on object: " << y << "\n";
    }
};

int main() {
    auto l = [&]( int x ) -> void {
        std::cout << "lambda invoked with: " << x << "\n";
    };

    Foo obj( 42 );

    using Delegate = delegate< void( int ) >;
    std::vector< Delegate > v;
    v.push_back( Delegate( foo ) );
    v.push_back( Delegate( &Foo::f ) );
    v.push_back( Delegate( obj, &Foo::g) );
    v.push_back( Delegate( l ) );

    for( unsigned int i = 0; i != v.size(); i++ )
        v[i]( i );
}
