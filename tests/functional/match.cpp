#include <atoms/functional/match.h>
#include <catch.hpp>

struct X {};
struct Y {};

#include <string>

using namespace atoms;


TEST_CASE( "Basic usage" ) {
    SECTION( "X" ) {
        std::variant< X, Y > x{ X{} };
        std::string res = match( x )
            | []( const X& x ) { return "x"; }
            | []( Y y ) { return "y"; }
            | otherwise( "other" )
            ;
        REQUIRE( res == "x" );
    }
    SECTION( "Y" ) {
        std::variant< X, Y > x{ Y{} };
        std::string res = match( x )
            | []( const X& x ) { return "x"; }
            | []( Y y ) { return "y"; }
            | otherwise( "other" )
            ;
        REQUIRE( res == "y" );
    }
    SECTION( "Other" ) {
        std::variant< X, Y > x{ X{} };
        std::string res = match( x )
            | []( Y y ) { return "y"; }
            | otherwise( "other" )
            ;
        REQUIRE( res == "other" );
    }
    SECTION( "Only one triggers" ) {
        std::variant< X, Y > x{ X{} };
        int i = 0;
        match( x ).with(
            [&]( const X& x ) { i++; },
            [&]( Y y ) { i++; },
            [&]( Otherwise ) { i++; }
        );
        REQUIRE( i == 1 );
    }
}