#pragma once

#include <utility>
#include <variant>
#include <type_traits>
#include <optional>

#include "../type/function_trait.h"

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

namespace atoms {

struct Otherwise {};

namespace detail {

template< typename T, typename... Ts >
constexpr bool contains() {
    return std::disjunction_v< std::is_same< T, Ts >... >;
}

template < typename T >
struct Otherwise {
    T _value;
};

template < typename T >
struct Match;

template < typename T, typename Result >
struct MatchInProgress;

template < typename Self, typename Result, typename... Args >
struct MatchInProgressMix {
    Self& self() { return *static_cast< Self * >( this ); }
    const Self& self() const { return *static_cast< const Self * >( this ); }

    template< typename T >
    operator T () {
        return T( static_cast< Result >( *this ) );
    }

    operator Result () {
        if ( self()._result )
            return self()._result.value();
        throw std::runtime_error( "Exhaustive patterns" );
    }

    template < typename F >
    auto operator|( F f ) {
        using Variant = typename std::remove_reference_t< decltype( Self::_var ) >;
        using fTrait = functionTraits< F >;
        static_assert( fTrait::arity == 1, "Only unary functions are accepted in match" );
        static_assert( std::is_same< typename fTrait::returnType, Result>::value, "Pattern return types do not match" );
        using ArgType = typename fTrait:: template argument< 0 >::type;
        if constexpr ( std::is_same_v< ::atoms::Otherwise, ArgType > ) {
            if ( self()._result )
                return MatchInProgress< Variant, Result >{ self()._var, self()._result };
            return MatchInProgress< Variant, Result >{ self()._var, f( {} ) };
        }
        else {
            static_assert( contains< ArgType, Args... >(), "No match for this function" );
            if ( self()._result || !std::holds_alternative< ArgType >( self()._var ) )
                return MatchInProgress< Variant, Result >{ self()._var, self()._result };
            else
                return MatchInProgress< Variant, Result >{
                    self()._var, f( std::get< ArgType >( self()._var ) ) };
        }
    }

    template < typename T >
    auto operator|( Otherwise< T >&& o ) {
        using Variant = typename std::remove_reference_t< decltype( Self::_var ) >;
        if ( self()._result )
            return MatchInProgress< Variant, Result >{ self()._var, self()._result };
        return MatchInProgress< Variant, Result >{ self()._var, std::move( o._value ) };
    }
};

template < typename... Args, typename Result >
struct MatchInProgress< std::variant< Args... >, Result >
    : public MatchInProgressMix< MatchInProgress< std::variant< Args... >, Result >, Result, Args... >
{
    std::variant< Args... >& _var;
    std::optional< Result > _result;

    MatchInProgress( std::variant< Args... >& v, const std::optional< Result >& r )
        : _var( v ), _result( r )
    {}
};

template < typename... Args, typename Result >
struct MatchInProgress< const std::variant< Args... >, Result >
    : public MatchInProgressMix< MatchInProgress< const std::variant< Args... >, Result >, Result, Args... >
{
    const std::variant< Args... >& _var;
    std::optional< Result > _result;

    MatchInProgress( const std::variant< Args... >& v, const std::optional< Result >& r )
        : _var( v ), _result( r )
    {}
};

template < typename Self, typename... Args >
struct MatchMix {
    Self& self() { return *static_cast< Self * >( this ); }
    const Self& self() const { return *static_cast< const Self * >( this ); }

    template < typename F >
    auto operator|( F f ) {
        using Variant = typename std::remove_reference_t< decltype( Self::_var ) >;
        using fTrait = functionTraits< F >;
        static_assert( fTrait::arity == 1, "Only unary functions are accepted in match" );
        using ArgType = typename std::remove_cv<
                            typename std::remove_reference<
                                typename fTrait:: template argument< 0 >::type
                            >::type
                        >::type;
        static_assert( contains< ArgType, Args... >(), "No match for this function" );
        if ( !std::holds_alternative< ArgType >( self()._var ) )
            return MatchInProgress< Variant, typename fTrait::returnType >( self()._var, {} );
        else
            return MatchInProgress< Variant, typename fTrait::returnType >(
                self()._var, f( std::get< ArgType >( self()._var ) ) );
    }

    template < typename T >
    auto operator|( Otherwise< T >&& o ) {
        using Variant = typename std::remove_reference_t< decltype( Self::_var ) >;
        return MatchInProgress< Variant, T >{ self()._var, std::move( o._value ) };
    }
};

template < typename... Args >
struct Match< std::variant< Args... > >
    : public MatchMix< Match< std::variant< Args... > >, Args... >
{
    std::variant< Args... >& _var;
};

template < typename... Args >
struct Match< const std::variant< Args... > >
    : public MatchMix< Match< const std::variant< Args... > >, Args... >
{
    const std::variant< Args... >& _var;
};

} // namespace detail

template < typename... Args >
detail::Match< const std::variant< Args... > > match( const std::variant< Args... >& v ) {
    return { {}, v };
}

template < typename... Args >
detail::Match< std::variant< Args... > > match( std::variant< Args... >& v ) {
    return { {}, v };
}

template < typename T >
detail::Otherwise< T > otherwise( T&& value ) { return { value }; }

} // namespace atoms
