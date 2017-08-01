#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include <ostream>

#include "fixed.h"

namespace atoms {

// Print fixed point number to stream naively (via conversion to float)
// For debugging purposes only
template < bit_width_type I, bit_width_type F >
std::ostream& operator<<( std::ostream& o, const atoms::Fixed< I, F >& x ) {
    o << x.to_float();
    return o;
}

} // namespace atoms