#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include <cmath>
#include <array>
#include <ostream>
#include "equality.h"
#include "classes.h"

namespace atoms {

namespace detail {

template < typename T, int Size >
struct VecBase {
    std::array< T, Size > _elems;

    VecBase() = default;
    VecBase( const std::array< T, Size >& l )
        : _elems( l ) {}
};

template < typename T >
struct VecBase< T, 2 > {
    union {
        struct { T x, y; };
        std::array< T, 2 > _elems;
    };

    VecBase() = default;
    VecBase( const std::array< T, 2 >& l )
        : _elems( l ) {}
};

template < typename T >
struct VecBase< T, 3 > {
    union {
        struct { T x, y, z; };
        std::array< T, 3 > _elems;
    };

    VecBase() = default;
    VecBase( const std::array< T, 3 >& l )
        : _elems( l ) {}
};

} // namespace detail

template < typename T, int Size, typename ElemEq = NumEq< T >,
    typename Storage = detail::VecBase< T, Size > >
struct Vector : public Storage,
    public Eq< Vector< T, Size, ElemEq, Storage > >,
    public Num< Vector< T, Size, ElemEq, Storage > >
{
    Vector() {
        for ( auto& x : this->_elems )
            x = T();
    }

    Vector( std::array< T, Size > l )
        : Storage( l ) {}

    bool operator==( const Vector& o ) const {
        for ( int i = 0; i != Size; i++ )
            if ( !ElemEq::equal( this->_elems[ i ], o._elems[ i ] ) )
                return false;
        return true;
    }

    Vector& operator+=( const Vector& o ) {
        for ( int i = 0; i != Size; i++ )
            this->_elems[ i ] += o._elems[ i ];
        return *this;
    }

    Vector& operator-=( const Vector& o ) {
        for ( int i = 0; i != Size; i++ )
            this->_elems[ i ] -= o._elems[ i ];
        return *this;
    }

    template < typename O >
    std::enable_if_t< !std::is_same< Vector, O >::value, Vector& >
    operator*=( const O& o ) {
        for ( int i = 0; i != Size; i++ )
            this->_elems[ i ] *= o;
        return *this;
    }

    template < typename O >
    std::enable_if_t< !std::is_same< Vector, O >::value, Vector >
    operator*( const O& o ) const {
        Vector ret = *this;
        ret *= o;
        return ret;
    }

    template < typename O >
    std::enable_if_t< !std::is_same< Vector, O >::value, Vector& >
    operator/=( const O& o ) {
        for ( int i = 0; i != Size; i++ )
            this->_elems[ i ] /= o;
        return *this;
    }

    template < typename O >
    std::enable_if_t< !std::is_same< Vector, O >::value, Vector >
    operator/( const O& o ) const {
        Vector ret = *this;
        ret /= o;
        return ret;
    }

    T operator*( const Vector& o ) {
        T ret = 0;
        for ( int i = 0; i != Size; i++ )
            ret += this->_elems[ i ] * o._elems[ i ];
        return ret;
    }

    T length() const {
        T l{};
        for ( int i = 0; i != Size; i++ )
            l += this->_elems[ i ] * this->_elems[ i ];
        return sqrt( l );
    }

    void normalize() {
        *this /= length();
    }

    Vector norm() const {
        Vector x = *this;
        x.normalize();
        return x;
    }

    T& operator[]( int idx ) { return this->_elems[ idx ]; }
    const T& operator[]( int idx ) const { return this->_elems[ idx ]; }
    auto begin() { return this->_elems.begin(); }
    auto cbegin() const { return this->_elems.cbegin(); }
    auto end() { return this->_elems.end(); }
    auto cend() const { return this->_elems.cend(); }
};

template < typename T, int Size, typename Eq, typename Storage >
Vector< T, Size, Eq, Storage > operator-( Vector< T, Size, Eq, Storage > x ) {
    for ( int i = 0; i != Size; i++ )
        x._elems[ i ] = -x._elems[ i ];
    return x;
}

template < typename T, int Size, typename Eq, typename Storage, typename O >
auto operator*( const O& o, Vector< T, Size, Eq, Storage > a )
    -> Vector< T, Size, Eq, Storage >
{
    a *= o;
    return a;
}

template < typename T, int Size, typename Eq, typename Storage >
std::ostream& operator<<( std::ostream& o, const Vector< T, Size, Eq, Storage >& v ) {
    o << "[";
    const char *separator = "";
    for ( const auto& x : v._elems ) {
        o << separator << x;
        separator = ", ";
    }
    o << "]";
    return o;
}

template < typename T, typename Eq, typename Storage >
auto cross( const Vector< T, 3, Eq, Storage >& a, const Vector< T, 3, Eq, Storage >& b )
    -> Vector< T, 3, Eq, Storage >
{
    return {{ a.y * b.z - a.z * b.y,
              a.z * b.x - a.x * b.z,
              a.x * b.y - a.y * b.x }};
}

template < class T, class Eq = NumEq< T > >
using Vector2 = Vector< T, 2, Eq >;

template < class T, class Eq = NumEq< T > >
using Vector3 = Vector< T, 3, Eq >;


} // namespace atoms