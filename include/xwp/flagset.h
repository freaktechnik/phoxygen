/*
 * phoxygen -- PHP documentation tool. (C) 2015--2016 Baubadil GmbH.
 *
 * phoxygen is free software; you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation, in version 2 as it comes
 * in the "LICENSE" file of the phoxygen main distribution. This program is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the LICENSE file for more details.
 */

#ifndef XWP_BITSET_H
#define XWP_BITSET_H

#include <atomic>

//

/**
 *  A class template for typesafe bit enumerations.
 *  See http://stackoverflow.com/questions/1448396/how-to-use-enums-as-flags-in-c#.
 *
 *  Usage:
 *
 *   1. Declare an enum class with bit values, like enum class MyEnum { a = 1, b = 2, c = 3 }.
 *
 *   2. Define a variable with FlagSet<MyEnum>.
 *
 *   3. Use operators, test(), set(), reset() to your liking.
 */

template <typename EnumType, typename Underlying = uint>
class FlagSet
{
    typedef Underlying FlagSet::* RestrictedBool;

public:
    FlagSet() : m_flags(Underlying()) {}

    FlagSet(EnumType singleFlag):
        m_flags((Underlying)singleFlag)
    {}

    FlagSet(const FlagSet& original):
        m_flags(original.m_flags)
    {}

    FlagSet& operator |=(const FlagSet& f) {
        m_flags |= f.m_flags;
        return *this;
    }

    FlagSet& operator &=(const FlagSet& f) {
        m_flags &= f.m_flags;
        return *this;
    }

    friend FlagSet operator |(const FlagSet& f1, const FlagSet& f2) {
        return FlagSet(f1) |= f2;
    }

    friend FlagSet operator &(const FlagSet& f1, const FlagSet& f2) {
        return FlagSet(f1) &= f2;
    }

    void reset(const EnumType& f)
    {
        m_flags &= ~((Underlying)f);
    }

    bool test(const EnumType &f)
    {
        return !!(m_flags & (Underlying)f);
    }

    operator RestrictedBool() const {
        return m_flags ? &FlagSet::m_flags : 0;
    }

    Underlying value() const {
        return m_flags;
    }

protected:
    Underlying  m_flags;
};

/*
#define DEFINE_BITSET(T) \
inline T operator~ (T a) { return (T)~(int)(a); } \
inline T operator| (T a, T b) { return (T)((int)(a) | (int)(b)); } \
inline int operator| (int a, T b) { return ((a) | (int)(b)); } \
inline T operator& (T a, T b) { return (T)((int)(a) & (int)(b)); } \
inline int operator& (int a, T b) { return ((a) & (int)(b)); } \
inline T operator^ (T a, T b) { return (T)((int)(a) ^ (int)(b)); } \
inline T& operator|= (T& a, T b) { return (T&)((int&)(a) |= (int)(b)); } \
inline int operator|= (int a, T b) { return ((a) |= (int)(b)); } \
inline T& operator&= (T& a, T b) { return (T&)((int&)(a) &= (int)(b)); } \
inline int operator&= (int a, T b) { return ((a) &= (int)(b)); } \
inline T& operator^= (T& a, T b) { return (T&)((int&)(a) ^= (int)(b)); }
*/

#endif // XWP_BITSET_H
