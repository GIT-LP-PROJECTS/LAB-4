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

    sc_fxnum_bitref.h - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_FXNUM_BITREF_H
#define SC_FXNUM_BITREF_H


#include <stdlib.h>

#ifndef   SC_BVREP_H
#include "sc_bvrep.h"
#endif

#ifndef   SC_BIT_H
#include "sc_bit.h"
#endif

#if defined(__BCPLUSPLUS__)
#pragma hdrstop
#endif

// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_bitref
//
//  Proxy class for bit-selection in class sc_fxnum, behaves like sc_bit.
// ----------------------------------------------------------------------------

class sc_fxnum_bitref
{

    friend class sc_fxnum;
    friend class sc_fxnum_fast_bitref;

    bool get() const;
    void set( bool );


    // constructor

    sc_fxnum_bitref( sc_fxnum&, int );

public:

    // assignment operators

#define DECL_ASN_OP_T(op,tp)                                                  \
    sc_fxnum_bitref& operator op ( tp );

#define DECL_ASN_OP(op)                                                       \
    DECL_ASN_OP_T(op,const sc_fxnum_bitref&)                                  \
    DECL_ASN_OP_T(op,const sc_fxnum_fast_bitref&)                             \
    DECL_ASN_OP_T(op,const sc_bit&)                                           \
    DECL_ASN_OP_T(op,bool)

    DECL_ASN_OP(=)

    DECL_ASN_OP(&=)
    DECL_ASN_OP(|=)
    DECL_ASN_OP(^=)

#undef DECL_ASN_OP_T
#undef DECL_ASN_OP


    // implicit conversion

    operator bool() const;


    // print or dump content

    void print( ostream& = cout ) const;
    void dump( ostream& = cout ) const;

private:

    sc_fxnum& _num;
    int       _idx;

};


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast_bitref
//
//  Proxy class for bit-selection in class sc_fxnum_fast, behaves like sc_bit.
// ----------------------------------------------------------------------------

class sc_fxnum_fast_bitref
{

    friend class sc_fxnum_fast;
    friend class sc_fxnum_bitref;

    bool get() const;
    void set( bool );


    // constructor

    sc_fxnum_fast_bitref( sc_fxnum_fast&, int );

public:

    // assignment operators

#define DECL_ASN_OP_T(op,tp)                                                  \
    sc_fxnum_fast_bitref& operator op ( tp );

#define DECL_ASN_OP(op)                                                       \
    DECL_ASN_OP_T(op,const sc_fxnum_bitref&)                                  \
    DECL_ASN_OP_T(op,const sc_fxnum_fast_bitref&)                             \
    DECL_ASN_OP_T(op,const sc_bit&)                                           \
    DECL_ASN_OP_T(op,bool)

    DECL_ASN_OP(=)

    DECL_ASN_OP(&=)
    DECL_ASN_OP(|=)
    DECL_ASN_OP(^=)

#undef DECL_ASN_OP_T
#undef DECL_ASN_OP


    // implicit conversion

    operator bool() const;


    // print or dump content

    void print( ostream& = cout ) const;
    void dump( ostream& = cout ) const;

private:

    sc_fxnum_fast& _num;
    int            _idx;

};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_bitref
//
//  Proxy class for bit-selection in class sc_fxnum, behaves like sc_bit.
// ----------------------------------------------------------------------------

// constructor

inline
sc_fxnum_bitref::sc_fxnum_bitref( sc_fxnum& num_, int idx_ )
: _num( num_ ), _idx( idx_ )
{}


// assignment operators

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator = ( const sc_fxnum_bitref& a )
{
    if( &a != this )
	set( a.get() );
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator = ( const sc_fxnum_fast_bitref& a )
{
    set( a.get() );
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator = ( const sc_bit& a )
{
    set( static_cast<bool>( a ) );
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator = ( bool a )
{
    set( a );
    return *this;
}


inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator &= ( const sc_fxnum_bitref& b )
{
    set( get() && b.get() );
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator &= ( const sc_fxnum_fast_bitref& b )
{
    set( get() && b.get() );
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator &= ( const sc_bit& b )
{
    set( get() && static_cast<bool>( b ) );
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator &= ( bool b )
{
    set( get() && b );
    return *this;
}


inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator |= ( const sc_fxnum_bitref& b )
{
    set( get() || b.get() );
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator |= ( const sc_fxnum_fast_bitref& b )
{
    set( get() || b.get() );
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator |= ( const sc_bit& b )
{
    set( get() || static_cast<bool>( b ) );
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator |= ( bool b )
{
    set( get() || b );
    return *this;
}


inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator ^= ( const sc_fxnum_bitref& b )
{
    set( get() != b.get() );
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator ^= ( const sc_fxnum_fast_bitref& b )
{
    set( get() != b.get() );
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator ^= ( const sc_bit& b )
{
    set( get() != static_cast<bool>( b ) );
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator ^= ( bool b )
{
    set( get() != b );
    return *this;
}


// implicit conversion

inline
sc_fxnum_bitref::operator bool() const
{
    return get();
}


inline
ostream&
operator << ( ostream& os, const sc_fxnum_bitref& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast_bitref
//
//  Proxy class for bit-selection in class sc_fxnum_fast, behaves like sc_bit.
// ----------------------------------------------------------------------------

// constructor

inline
sc_fxnum_fast_bitref::sc_fxnum_fast_bitref( sc_fxnum_fast& num_, int idx_ )
: _num( num_ ), _idx( idx_ )
{}


// assignment operators

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator = ( const sc_fxnum_bitref& a )
{
    set( a.get() );
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator = ( const sc_fxnum_fast_bitref& a )
{
    if( &a != this )
	set( a.get() );
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator = ( const sc_bit& a )
{
    set( static_cast<bool>( a ) );
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator = ( bool a )
{
    set( a );
    return *this;
}


inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator &= ( const sc_fxnum_bitref& b )
{
    set( get() && b.get() );
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator &= ( const sc_fxnum_fast_bitref& b )
{
    set( get() && b.get() );
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator &= ( const sc_bit& b )
{
    set( get() && static_cast<bool>( b ) );
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator &= ( bool b )
{
    set( get() && b );
    return *this;
}


inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator |= ( const sc_fxnum_bitref& b )
{
    set( get() || b.get() );
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator |= ( const sc_fxnum_fast_bitref& b )
{
    set( get() || b.get() );
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator |= ( const sc_bit& b )
{
    set( get() || static_cast<bool>( b ) );
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator |= ( bool b )
{
    set( get() || b );
    return *this;
}


inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator ^= ( const sc_fxnum_bitref& b )
{
    set( get() != b.get() );
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator ^= ( const sc_fxnum_fast_bitref& b )
{
    set( get() != b.get() );
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator ^= ( const sc_bit& b )
{
    set( get() != static_cast<bool>( b ) );
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator ^= ( bool b )
{
    set( get() != b );
    return *this;
}


// implicit conversion

inline
sc_fxnum_fast_bitref::operator bool() const
{
    return get();
}


inline
ostream&
operator << ( ostream& os, const sc_fxnum_fast_bitref& a )
{
    a.print( os );
    return os;
}


#endif

// Taf!
