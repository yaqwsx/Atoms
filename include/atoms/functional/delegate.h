#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include <cstddef>
#include <utility>

// Leightweigt delegate - hides a function, a static member function, a member
// function or a functor behind the same type. In the worst case scenario,
// only two defer calls are performed. Delegate stores one pointer and
// one member function pointer.
// Beware - it is user's responsibility to maintain proper lifetime for binding
// objects.

namespace atoms {

template < typename ResT, typename... ArgT >
class delegate;

template < typename ResT, typename... ArgT >
class delegate< ResT( ArgT... ) > {

    template < typename Class, typename FunPtr >
    struct Storage {
        FunPtr function;
        Class *instance;
    };

    class DefaultClass;
    using DefaultFunction = void( DefaultClass::* )( void );
    using DefaultStorage = Storage< DefaultClass, DefaultFunction >;

    // Invoke free function
    template < std::nullptr_t, typename FunPtr >
    static ResT invoke( const char *storage, ArgT&&... args ) {
        using StorageType = const Storage< std::nullptr_t, FunPtr >;
        StorageType& inst = * reinterpret_cast< StorageType * >( storage );
        return ( inst.function )( std::forward< ArgT >( args )... );
    }

    // Invoke method
    template < typename Class, typename FunPtr >
    static ResT invoke( const char *storage, ArgT&&... args ) {
        using StorageType = const Storage< Class, FunPtr >;
        StorageType& inst = * reinterpret_cast< StorageType * >( storage );
        return ( ( inst.instance )->*( inst.function ) )( std::forward< ArgT >( args )... );
    }

    // Invoke functor
    template < typename Class, typename std::nullptr_t >
    static ResT invoke( const char *storage, ArgT&&... args ) {
        using StorageType = const Storage< Class, std::nullptr_t >;
        StorageType& inst = * reinterpret_cast< StorageType * >( storage );
        return ( *inst.instance )( std::forward< ArgT >( args )... );
    }

    char _storage[ sizeof( DefaultStorage ) ];
    ResT (*_invoker )( const char *, ArgT&&... );

public:
    delegate() noexcept
        : _storage{}, _invoker{ nullptr } {}

    // Ordinary function
    explicit delegate( ResT( *fun )( ArgT...) ) noexcept : delegate() {
        using FunPtr = decltype( fun );
        using StorageType = Storage< std::nullptr_t, FunPtr >;
        auto *storage = reinterpret_cast< StorageType * >( _storage );
        storage->function = fun;
        storage->instance = nullptr;
        _invoker = &delegate::invoke< nullptr, FunPtr >;
    }

    // Method
    template< typename Class, typename FunPtr >
    explicit delegate( Class& object, FunPtr method ) noexcept : delegate() {
        using StorageType = Storage< Class, FunPtr >;
        auto *storage = reinterpret_cast< StorageType * >( _storage );
        storage->function = method;
        storage->instance = &object;
        _invoker = &delegate::invoke< Class, FunPtr >;
    }

    // Functor
    template < typename Functor >
    explicit delegate( Functor& f ) noexcept : delegate() {
        using StorageType = Storage< Functor, std::nullptr_t >;
        auto *storage = reinterpret_cast< StorageType *>( _storage );
        storage->function = nullptr;
        storage->instance = &f;
        _invoker = &delegate::invoke< Functor, nullptr >;
    }

    ResT operator()( ArgT... args ) const noexcept {
        return ( *_invoker )( _storage, std::forward< ArgT >( args )... );
    }

    operator bool() const noexcept {
        return _invoker;
    }
};

} // namespace atoms
