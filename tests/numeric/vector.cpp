#include <catch.hpp>

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include <atoms/numeric/vector.h>

using namespace atoms;

using V2 = Vector< float, 2 >;
using V3 = Vector< float, 3 >;

TEST_CASE( "2D basic", "numeric/vector:2D") {
    SECTION( "Default constructor" ) {
        V2 a;
        for ( auto x : a )
            REQUIRE( x == 0.0f );
        REQUIRE( a.x == 0.0f );
        REQUIRE( a.y == 0.0f );
    }
    SECTION( "Value constructor" ) {
        V2 a{ { 1, 2 } };
        for ( int i = 0; i != 2; i++ )
            REQUIRE( a[ i ] == i + 1 );
        REQUIRE( a.x == 1.0f );
        REQUIRE( a.y == 2.0f );
    }
    SECTION( "Basic operations" ) {
        V2 a{{ 1, 2 }};
        V2 b{{ 3, 4 }};

        REQUIRE( a + b == b + a );
        auto r1 = a + b;
        REQUIRE( r1.x == 4 ); REQUIRE( r1.y == 6 );
        r1 += a;
        REQUIRE( r1.x == 5 ); REQUIRE( r1.y == 8 );
        auto r2 = a - b;
        REQUIRE( r2.x == -2 ); REQUIRE( r2.y == -2 );
        r2 -= a;
        REQUIRE( r2.x == -3 ); REQUIRE( r2.y == -4 );
        auto r3 = a * 10;
        REQUIRE( r3.x == 10 ); REQUIRE( r3.y == 20 );
        REQUIRE( a * 10 == 10 * a );
        r3 *= 10;
        REQUIRE( r3.x == 100 ); REQUIRE( r3.y == 200 );
        auto r4 = a / 10;
        REQUIRE( r4.x == Approx( 0.1 ) ); REQUIRE( r4.y == Approx( 0.2 ) );
        r4 /= 10;
        REQUIRE( r4.x ==  Approx( 0.01 ) ); REQUIRE( r4.y ==  Approx( 0.02 ) );
        auto r5 = -a;
        REQUIRE( r5.x == -1 ); REQUIRE( r5.y == -2 );
        auto r6 = a * b;
        REQUIRE( r6 == 11 );
        REQUIRE( a * b == b * a );

        V2 ref{{ 3, 4 }};
        REQUIRE( ref.length() == Approx( 5 ) );
        REQUIRE( a.norm().length() == Approx( 1 ) );
    }
}

TEST_CASE( "3D basic", "numberic/vector:3D" ) {
    SECTION( "Default constructor" ) {
        V3 a;
        for ( auto x : a )
            REQUIRE( x == 0.0f );
        REQUIRE( a.x == 0.0f );
        REQUIRE( a.y == 0.0f );
        REQUIRE( a.z == 0.0f );
    }
    SECTION( "Value constructor" ) {
        V3 a{ { 1, 2, 3 } };
        for ( int i = 0; i != 2; i++ )
            REQUIRE( a[ i ] == i + 1 );
        REQUIRE( a.x == 1.0f );
        REQUIRE( a.y == 2.0f );
        REQUIRE( a.z == 3.0f );
    }

    SECTION( "Cross product" ) {
        V3 a{{ 4, 6, 5 }};
        V3 b{{ 2, 5, 1 }};
        auto r = cross( a, b );
        REQUIRE( r.x == Approx( -19 ) );
        REQUIRE( r.y == Approx( 6 ) );
        REQUIRE( r.z == Approx( 8 ) );
        REQUIRE( -r == cross( b, a ) );
    }
}