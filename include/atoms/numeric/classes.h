#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include <cmath>
#include <array>
#include <ostream>
#include "equality.h"

namespace atoms {

template < typename Base >
struct Eq {
    const Base& self() const {
        return *static_cast< const Base * >( this );
    }

    bool operator!=( const Base& o ) const {
        return !( self() == o );
    }
};

template < typename Base >
struct Num {
    const Base& self() const {
        return *static_cast< const Base * >( this );
    }

    Base operator+( const Base& o ) const {
        Base x = self();
        x += o;
        return x;
    }

    Base operator-( const Base& o ) const {
        Base x = self();
        x -= o;
        return x;
    }

    Base operator*( const Base& o ) const {
        Base x = self();
        x *= o;
        return x;
    }

    Base operator/( const Base& o ) const {
        Base x = self();
        x /= o;
        return x;
    }
};

} // namespace atoms