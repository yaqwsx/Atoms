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

// Adopted from https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector
// Generates has_member_X class that can be used to check for existence
// of member function or attribute
#define GENERATE_HAS_MEMBER(member)                                               \
                                                                                  \
template < class T >                                                              \
class HasMember_##member                                                          \
{                                                                                 \
private:                                                                          \
    using Yes = char[2];                                                          \
    using  No = char[1];                                                          \
                                                                                  \
    struct Fallback { int member; };                                              \
    struct Derived : T, Fallback { };                                             \
                                                                                  \
    template < class U >                                                          \
    static No& test ( decltype(&U::member) );                                     \
    template < typename U >                                                       \
    static Yes& test ( ... );                                                      \
                                                                                  \
public:                                                                           \
    static constexpr bool RESULT = sizeof(test<Derived>(nullptr)) == sizeof(Yes); \
};                                                                                \
                                                                                  \
template < class T >                                                              \
struct has_member_##member                                                        \
: public std::integral_constant<bool, HasMember_##member<T>::RESULT>              \
{ };                                                                              \


}
