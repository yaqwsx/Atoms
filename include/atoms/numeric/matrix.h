#pragma once

#include <cassert>
#include <array>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <iterator>

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

namespace atoms {

template < typename T, int W, int H >
struct Matrix {
    Matrix() = default;
    Matrix( T value ) {
        std::fill( _data.begin(), _data.end(), value );
    }

    Matrix( std::initializer_list< std::initializer_list< T > > list ) {
        assert( list.size() == H );
        auto it = _data.begin();
        for ( const auto& row : list ) {
            assert( row.size() == W );
            for ( const auto& item : row )
                *( it++ ) = item;
        }
    }

    struct _View {
        _View( Matrix& m, int offset ): _m( m ), _offset( offset ) {}
        T& operator[]( int idx ) { return _m._data[ _offset + idx ]; }
        Matrix& _m;
        int _offset;
    };

    struct _ConstView {
        _ConstView( const Matrix& m, int offset ): _m( m ), _offset( offset ) {}
        const T& operator[]( int idx ) const { return _m._data[ _offset + idx ]; }
        const Matrix& _m;
        int _offset;
    };

    _View operator[]( int idx ) {
        return { *this, idx * W };
    }

    _ConstView operator[]( int idx ) const {
        return { *this, idx * W };
    }

    Matrix& operator+=( const Matrix& o ) {
        auto y = o._data.begin();
        for ( auto x = _data.begin(); x != _data.end(); x++, y++ )
            *x += *y;
        return *this;
    }

    Matrix& operator-=( const Matrix& o ) {
        auto y = o._data.begin();
        for ( auto x = _data.begin(); x != _data.end(); x++, y++ )
            *x -= *y;
        return *this;
    }

    template < typename C >
    Matrix& operator*=( C c ) {
        for ( auto& x : _data )
            x *= c;
        return *this;
    }

    template < typename C >
    Matrix& operator/=( C c ) {
        for ( auto& x : _data )
            x /= c;
        return *this;
    }

    std::array< T, W * H > _data;
};

template < typename T, int W, int H >
Matrix< T, W, H > operator+( Matrix< T, W, H > lhs, const Matrix< T, W, H >& rhs ) {
    lhs += rhs;
    return lhs;
}

template < typename T, int W, int H >
Matrix< T, W, H > operator-( Matrix< T, W, H > lhs, const Matrix< T, W, H >& rhs ) {
    lhs -= rhs;
    return lhs;
}

template < typename T, int W, int H, typename C >
Matrix< T, W, H > operator*( Matrix< T, W, H > lhs, const C& rhs ) {
    lhs *= rhs;
    return lhs;
}

template < typename T, int W, int H, typename C >
Matrix< T, W, H > operator*( const C& lhs, Matrix< T, W, H > rhs ) {
    for ( auto& x : rhs )
        x = lhs * x;
    return rhs;
}

template < typename T, int W, int H, typename C >
Matrix< T, W, H > operator/( Matrix< T, W, H > lhs, const C& rhs ) {
    lhs /= rhs;
    return lhs;
}

template < typename T, int W, int H, typename C >
Matrix< T, W, H > operator/( const C& lhs, Matrix< T, W, H > rhs ) {
    for ( auto& x : rhs )
        x = lhs / x;
    return rhs;
}

template < typename T, int S, int H, int W >
Matrix< T, W, H > operator*( const Matrix< T, S, H >& lhs, const Matrix< T, W, S >& rhs ) {
    Matrix< T, W, H > ret;
    for ( int i = 0; i != H; i++ ) {
        for ( int j = 0; j!= W; j++ ) {
            T val = lhs[ i ][ 0 ] * rhs[ 0 ][ j ];
            for ( int k = 1; k != S; k++ )
                val += lhs[ i ][ k ] * rhs[ k ][ j ];
            ret[ i ][ j ] = val;
        }
    }
    return ret;
}

template < typename T, int W, int H >
std::ostream& operator<<( std::ostream& o, const Matrix< T, W, H >& m  ) {
    auto flags = o.flags();
    std::vector< std::string > items;
    items.reserve( W * H );
    size_t maxLen = 0;
    for ( const auto& item : m._data ) {
        std::ostringstream s;
        s.flags( flags );
        s << item;
        items.push_back( s.str() );
        maxLen = std::max( items.back().size(), maxLen );
    }

    auto itemIt = items.begin();
    for ( int i = 0; i != H; i++ ) {
        o << "{";
        for ( int j = 0; j != W; j++ ) {
            std::string& s = *( itemIt++ );
            o << " " << s;
            std::fill_n( std::ostream_iterator< char >( o ), maxLen - s.size(), ' ' );
            if ( j != W - 1 )
                o << ",";
        }
        o << " }\n";
    }
    return o;
}

} // namespace atoms