#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include <stdint.h>
#if defined(ATOMS_NO_STDLIB) || defined(ATOMS_NO_STDLIB11)
    #include "../stdlib/type_traits.h"
#else
    #include <type_traits>
#endif

namespace atoms {

// inspired by https://functionalcpp.wordpress.com/2013/08/05/function-traits/
template < typename F >
struct functionTraits;

template < typename R, typename... Args >
struct functionTraits< R(*)( Args... ) > : public functionTraits< R( Args... ) >
{};

template < typename R, typename... Args >
struct functionTraits< R( Args... ) > {
    using returnType = R;
    static constexpr std::size_t arity = sizeof...( Args );

    template < std::size_t N >
    struct argument {
        static_assert(N < arity, "Index out of range" );
        using type = typename std::tuple_element< N, std::tuple< Args...> >::type;
    };
};

// member function pointer
template< typename C, typename R, typename... Args >
struct functionTraits< R( C::* )( Args... ) > : public functionTraits< R( C&, Args... ) >
{};

// const member function pointer
template< typename C, typename R, typename... Args >
struct functionTraits< R( C::* )( Args... ) const > : public functionTraits< R( C&, Args... ) >
{};

// member object pointer
template< typename C, typename R >
struct functionTraits< R( C::* )> : public functionTraits< R( C& ) >
{};

template< typename F >
struct functionTraits {
    private:
        using call_type = functionTraits< decltype( &F::operator() ) >;
    public:
        using returnType = typename call_type::returnType;

        static constexpr std::size_t arity = call_type::arity - 1;

        template < std::size_t N >
        struct argument {
            static_assert( N < arity, "Index out of range" );
            using type = typename call_type::template argument< N + 1 >::type;
        };
};

} // namespace atoms