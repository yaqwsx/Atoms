#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include "../type/type_of_size.h"

#if defined(ATOMS_NO_STDLIB) || defined(ATOMS_NO_STDLIB11)
    #include "../stdlib/type_traits.h"
#else
    #include <type_traits>
#endif

namespace atoms {

template <bit_width_type I, bit_width_type F>
Fixed<I, F> multiply(const Fixed<I, F>& l, const Fixed<I, F>& r,
typename std::enable_if<type_of_size<I + F>::next_type::is_specialised>::type* = 0)
{
	using next_type = typename Fixed<I, F>::next_type;
	using base_type = typename Fixed<I, F>::base_type;

	typename Fixed<I, F>::next_type result(
	static_cast<next_type>(l.raw()) * static_cast<next_type>(r.raw()));
	result >>= F;

	return Fixed<I, F>::from_raw(static_cast<base_type>(result));
}

/*template <bit_width_type I, bit_width_type F>
Fixed<I, F> multiply(const Fixed<I, F>& l, const Fixed<I, F>& r*//*,
typename std::enable_if<!type_of_size<I + F>::next_type::is_specialised>::type* = 0*//*)
{
	using base_type = typename Fixed<I, F>::base_type;

	const base_type l_int = l.raw() >> F;
	const base_type r_int = r.raw() >> F;
	const base_type l_fra = l.raw() & Fixed<I, F>::fractional_mask;
	const base_type r_fra = r.raw() & Fixed<I, F>::fractional_mask;

	const base_type integer  = l_int * r_int;
	const base_type middle1  = l_int * r_fra;
	const base_type middle2  = l_fra * r_int;
	const base_type fraction = l_fra * r_fra;

	return Fixed<I, F>::from_raw((integer << F) + middle1 + middle2 + (fraction >> F));
}*/

template<bit_width_type I, bit_width_type F>
Fixed<I, F> divide(const Fixed<I, F>& n, const Fixed<I, F>& d,
typename std::enable_if<type_of_size<I + F>::next_type::is_specialised>::type* = 0)
{
	using base_type = typename Fixed<I, F>::base_type;
	using next_type = typename Fixed<I, F>::next_type;

	next_type numerator(n.raw());
	numerator <<= F;

	return Fixed<I, F>::from_raw(static_cast<base_type>(numerator / d.raw()));
}

template <bit_width_type I, bit_width_type F>
class Fixed {
	static_assert(type_of_size<I + F>::is_specialised, "invalid combination "
	"of sizes");
public:
	using base_type_info = type_of_size<I + F>;
	using base_type      = typename base_type_info::value_type;
	using next_type      = typename base_type_info::next_type::value_type;
	using signed_type    = typename base_type_info::signed_type;
	using unsigned_type  = typename base_type_info::unsigned_type;

	
	static const base_type fractional_mask = ~((~base_type(0)) << F);
	static const base_type integer_mask    = ~fractional_mask;

	static const base_type one = base_type(1) << F;
	
	Fixed() : data(0) {}
	Fixed(float n) : data(static_cast<base_type> (n * one)) {}
	Fixed(double n) : data(static_cast<base_type> (n * one)) {}
	template <class T>
	Fixed(T n) : data(base_type(n) << F) {}

	Fixed (const Fixed& o) : data(o.data) {}
	Fixed& operator=(const Fixed &o) {
		data = o.data;
		return *this;
	}

	static Fixed from_raw(base_type d) {
		return Fixed(d, Raw());
	}

	base_type raw() const {
		return data;
	}

	bool operator==(const Fixed& o) const {
		return data = o.data;
	}

	bool operator<(const Fixed& o) const {
		return data < o.data;
	}

	bool operator<=(const Fixed& o) const {
		return data <= o.data;
	}

	bool operator>(const Fixed& o) const {
		return data > o.data;
	}

	bool operator>=(const Fixed& o) const {
		return data >= o.data;
	}

	bool operator!() const {
		return !data;
	}

	Fixed operator~() const {
		return from_raw(~data);
	}

	Fixed operator-() const {
		// ToDo: Is this correct?
		return from_raw(-data);
	}

	Fixed operator+() const {
		return *this;
	}

	Fixed& operator++() {
		data += one;
		return *this;
	}

	Fixed operator++() const {
		return from_raw(data + one);
	}

	Fixed& operator--() {
		data -= one;
		return *this;
	}

	Fixed operator--() const {
		return from_raw(data - one);
	}

	Fixed operator+(const Fixed& o) const {
		return from_raw(data + o.data);
	}

	Fixed& operator+=(const Fixed& o) {
		data += o.data;
		return *this;
	}

	Fixed operator-(const Fixed& o) const {
		return from_raw(data - o.data);
	}

	Fixed& operator-=(const Fixed& o) const {
		data -= o.data;
		return *this;
	}

	Fixed operator*(const Fixed& o) const {
		return multiply(*this, o);
	}

	Fixed& operator*=(const Fixed& o) {
		//this = std::move(multiply(*this, o));
		*this = multiply(*this, o);
		return *this;
	}

	Fixed operator/(const Fixed& o) const {
		return divide(*this, o);
	}

	Fixed& operator/=(const Fixed& o) {
		//this = std::move(divice(*this, o));
		*this = divide(*this, o);
		return *this;
	}

	Fixed operator>>(bit_width_type i) const {
		return from_raw(data >> i);
	}

	Fixed& operator>>=(bit_width_type i) {
		data >>= i;
		return *this;
	}

	Fixed operator<<(bit_width_type i) const {
		return from_raw(data << i);
	}

	Fixed& operator<<=(bit_width_type i) {
		data <<= i;
		return *this;
	}

	signed_type to_signed() const {
		base_type sgn = 1 + ~(data >> (sizeof(base_type) * 8 - 1));
		base_type result = ((((data + sgn) ^ sgn) >> F) + sgn) ^ sgn;
		return static_cast<signed_type>(result);
	}

	unsigned_type to_unsigned() const {
		return data >> F;
	}

	float to_float() const {
		return static_cast<float>(data) / one;
	}

	double to_double() const {
		return static_cast<double>(data) / one;
	}

private:
	base_type data;

	struct Raw {};
	Fixed(base_type d, const Raw&): data(d) {}
};

// signed_shiftl and signed_shiftr keeps the "divide by power of 2" semantics
template <bit_width_type I, bit_width_type F>
Fixed<I, F> signed_shifrl(const Fixed<I, F>& o, bit_width_type i) {
	using base_type = typename Fixed<I, F>::base_type;

	base_type sgn = 1 + ~(o.raw() >> (sizeof(base_type) * 8 - 1));
	return Fixed<I, F>::from_raw(((((o.raw() + sgn) ^ sgn) << i) + sgn) ^ sgn);
}

template <bit_width_type I, bit_width_type F>
Fixed<I, F> signed_shiftr(const Fixed<I, F>& o, bit_width_type i) {
	using base_type = typename Fixed<I, F>::base_type;
	base_type sgn = 1 + ~(o.raw() >> (sizeof(base_type) * 8 - 1));
	return Fixed<I, F>::from_raw(((((o.raw() + sgn) ^ sgn) >> i) + sgn) ^ sgn);
}

}