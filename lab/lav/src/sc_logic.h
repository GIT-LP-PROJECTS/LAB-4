/******************************************************************************
    Copyright (c) 1996-2000 Synopsys, Inc.    ALL RIGHTS RESERVED

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC(TM) Open Community License Software Download and
  Use License Version 1.1 (the "License"); you may not use this file except
  in compliance with such restrictions and limitations. You may obtain
  instructions on how to receive a copy of the License at
  http://www.systemc.org/. Software distributed by Original Contributor
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

******************************************************************************/

/******************************************************************************

    sc_logic.h -- C++ implementation of logic type.  Behaves
                  pretty much the same way as HDLs except with 4 values.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_LOGIC_H
#define SC_LOGIC_H

#ifndef _MSC_VER
#include <iostream>
 #ifdef __BCPLUSPLUS__
  #pragma hdrstop
 #endif
using std::ostream;
using std::istream;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::hex;
#else
// MSVC6.0 has bugs in standard library
#include <iostream.h>
#endif


#include "sc_macros.h"
#include "sc_mempool.h"

class sc_logic {
    friend class sc_logic_resolve;

public:
    enum sc_logic_enum {
        XXXXX = 0,              // forcing unknown
        OOOOO,                  // forcing zero
        lllll,                  // forcing one
        ZZZZZ,                  // high impedance
        N_sc_logic_enum
    };

private:
    sc_logic_enum val;

    static const sc_logic_enum char_table[256];
    static const char to_char_table[N_sc_logic_enum];
    static const char to_4value_table[N_sc_logic_enum];
    static const bool to_bool_table[N_sc_logic_enum];
    static const sc_logic_enum and_table[N_sc_logic_enum][N_sc_logic_enum];
    static const sc_logic_enum or_table[N_sc_logic_enum][N_sc_logic_enum];
    static const sc_logic_enum xor_table[N_sc_logic_enum][N_sc_logic_enum];
    static const sc_logic_enum not_table[N_sc_logic_enum];

    sc_logic( sc_logic_enum r ) { val = r; }

    int to_index() const { return (int) val; }
    sc_logic& operator=( sc_logic_enum r ) { val = r; return *this; }

public:
    static const long to_long_table[N_sc_logic_enum]; // needed in sc_bv
    static void* operator new(size_t, void* p)         { return p; } // placement-new
    static void* operator new(size_t sz)               { return sc_mempool::allocate(sz); }
    static void  operator delete(void* p, size_t sz)   { sc_mempool::release(p, sz); }
    static void* operator new[](size_t sz)             { return sc_mempool::allocate(sz); }
    static void  operator delete[](void* p, size_t sz) { sc_mempool::release(p, sz); }

        // Default onstructor - assign the unknown value.
    sc_logic() { val = XXXXX; }

        // Copy constructor - copy the val member.
    sc_logic(const sc_logic& r) { val = r.val; }

        // Constructor from char - use lookup table for correspondence.
    sc_logic(char r) { val = char_table[(unsigned char) r]; }
    sc_logic(long r) { val = char_table[(unsigned char) (r&3)]; }
    sc_logic(int r) { val = char_table[(unsigned char) (r&3)]; }

        // Destructor
    ~sc_logic() { }

        // Bitwise operators on sc_logic
        // <group>
    sc_logic operator&( const sc_logic& r ) const
        {
            return sc_logic( and_table[val][r.val] );
        }
    sc_logic operator|( const sc_logic& r ) const
        {
            return sc_logic( or_table[val][r.val] );
        }
    sc_logic operator^( const sc_logic& r ) const
        { 
            return sc_logic( xor_table[val][r.val] );
        }
    sc_logic operator~() const
        {
            return sc_logic( not_table[val] );
        }
        // </group>

        // Assignment operators from sc_logic and from char
        // <group>
    sc_logic& operator=( const sc_logic& r )
        {
            val = r.val; return *this;
        }
    sc_logic& operator=( char r )
        { 
            val = char_table[(unsigned char) r]; return *this;
        }
    sc_logic& operator=( long r )
        {
            val = char_table[(unsigned char) r]; return *this;
        }
    sc_logic& operator=( int r )
        {
            val = char_table[(unsigned char) r]; return *this;
        }
        // </group>

        // op= operators
        // <group>
    sc_logic& operator&=( const sc_logic& r )
        {
            *this = *this & r; return *this;
        }
    sc_logic& operator|=( const sc_logic& r )
        {
            *this = *this | r; return *this;
        }
    sc_logic& operator^=( const sc_logic& r )
        {
            *this = *this ^ r; return *this;
        }
        // </group>

        // Negates the value of this sc_logic
    sc_logic& negate()
        {
            val = not_table[val]; return *this;
        }

        // Comparison operators with sc_logic and with char
        // <group>
    bool operator==( const sc_logic& r ) const
        {
            return (val == r.val);
        }
    bool operator==( char r ) const
        {
            return (val == char_table[(unsigned char) r]);
        }
    bool operator!=( const sc_logic& r ) const
        {
            return (val != r.val);
        }
    bool operator!=( char r ) const
        {
            return (val != char_table[(unsigned char) r]);
        }
        // </group>

        // Type cast into char
    char to_char() const { return to_char_table[val]; }

    long to_long() const { return to_long_table[val]; }

    bool is_01() const;
    void check_01() const;

    bool to_bool() const {
        check_01();
        return to_bool_table[val];
    }
    operator bool() const {
        check_01();
        return to_bool_table[val];
    }

    char to_4value() const { return to_4value_table[val]; }

    void print(ostream&) const;

};

/*---------------------------------------------------------------------------*/

#if 0
/*  THESE CAUSE PROBLEMS WITH IMPLICIT CONVERSION FROM sc_signal<unsigned>
    to unsigned, e.g. in the expression (foo == 5).  Spurious sc_logic
    object is created ...
    */
inline bool
operator==( char a, const sc_logic& b )
{
    return (b.operator==(a));
}

inline bool
operator!=( char a, const sc_logic& b )
{
    return (b.operator!=(a));
}
#endif


inline ostream&
operator<<( ostream& str, const sc_logic& z )
{
    return str << z.to_char();
}

extern const sc_logic sc_logic_X;
extern const sc_logic sc_logic_0;
extern const sc_logic sc_logic_1;
extern const sc_logic sc_logic_Z;

#endif
