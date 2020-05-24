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

    sc_fxnum_subref.cpp - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include "sc_bv.h"
#include "sc_lv.h"

#include "sc_fxnum_subref.h"

#include "sc_fxnum.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_subref
//
//  Proxy class for part-selection in class sc_fxnum,
//  behaves like sc_bv_base.
// ----------------------------------------------------------------------------

bool
sc_fxnum_subref::get() const
{
    return _num._val.get_slice( _from, _to, _num._params, _bv );
}

bool
sc_fxnum_subref::set()
{
    return _num._val.set_slice( _from, _to, _num._params, _bv );
}


// constructor

sc_fxnum_subref::sc_fxnum_subref( sc_fxnum& num_, int from_, int to_ )
: _num( num_ ), _from( from_ ), _to( to_ ),
  _bv( *new sc_bv_base( MAXT( _from, _to ) - MINT( _from, _to ) + 1 ) )
{}


// assignment operators

sc_fxnum_subref&
sc_fxnum_subref::operator = ( const sc_fxnum_subref& a )
{
    if( &a != this )
    {
	_bv = static_cast<sc_bv_base>( a );
	set();
    }
    return *this;
}

sc_fxnum_subref&
sc_fxnum_subref::operator = ( const sc_fxnum_fast_subref& a )
{
    _bv = static_cast<sc_bv_base>( a );
    set();
    return *this;
}

#define DEFN_ASN_OP_T(tp)                                                     \
sc_fxnum_subref&                                                              \
sc_fxnum_subref::operator = ( tp a )                                          \
{                                                                             \
    _bv = a;                                                                  \
    set();                                                                    \
    return *this;                                                             \
}

DEFN_ASN_OP_T(const sc_bv_base&)
DEFN_ASN_OP_T(const sc_lv_base&)
DEFN_ASN_OP_T(const sc_bool_vector&)
DEFN_ASN_OP_T(const char*)
DEFN_ASN_OP_T(const bool*)
DEFN_ASN_OP_T(const sc_signed&)
DEFN_ASN_OP_T(const sc_unsigned&)
DEFN_ASN_OP_T(const sc_int_base&)
DEFN_ASN_OP_T(const sc_uint_base&)
DEFN_ASN_OP_T(int64)
DEFN_ASN_OP_T(uint64)
DEFN_ASN_OP_T(int)
DEFN_ASN_OP_T(unsigned int)
DEFN_ASN_OP_T(long)
DEFN_ASN_OP_T(unsigned long)
DEFN_ASN_OP_T(char)

#undef DEFN_ASN_OP_T


#define DEFN_ASN_OP_T_A(op,tp)                                                \
sc_fxnum_subref&                                                              \
sc_fxnum_subref::operator op ## = ( tp a )                                    \
{                                                                             \
    get();                                                                    \
    _bv = _bv op static_cast<sc_bv_base>( a );                                \
    set();                                                                    \
    return *this;                                                             \
}

#define DEFN_ASN_OP_T_B(op,tp)                                                \
sc_fxnum_subref&                                                              \
sc_fxnum_subref::operator op ## = ( tp a )                                    \
{                                                                             \
    get();                                                                    \
    _bv = _bv op a;                                                           \
    set();                                                                    \
    return *this;                                                             \
}

#define DEFN_ASN_OP_A(op)                                                     \
DEFN_ASN_OP_T_A(op,const sc_fxnum_subref&)                                    \
DEFN_ASN_OP_T_A(op,const sc_fxnum_fast_subref&)                               \
DEFN_ASN_OP_T_B(op,const sc_bv_base&)                                         \
DEFN_ASN_OP_T_B(op,const sc_lv_base&)

DEFN_ASN_OP_A(&)
DEFN_ASN_OP_A(|)
DEFN_ASN_OP_A(^)

#undef DEFN_ASN_OP_T_A
#undef DEFN_ASN_OP_T_B
#undef DEFN_ASN_OP_A


// relational operators

#define DEFN_REL_OP_T(op,tp)                                                  \
bool                                                                          \
operator op ( const sc_fxnum_subref& a, tp b )                                \
{                                                                             \
    return ( static_cast<sc_bv_base>( a ) op b );                             \
}                                                                             \
                                                                              \
bool                                                                          \
operator op ( tp a, const sc_fxnum_subref& b )                                \
{                                                                             \
    return ( static_cast<sc_bv_base>( b ) op a );                             \
}

#define DEFN_REL_OP(op)                                                       \
bool                                                                          \
operator op ( const sc_fxnum_subref& a, const sc_fxnum_subref& b )            \
{                                                                             \
    return ( static_cast<sc_bv_base>( a ) op static_cast<sc_bv_base>( b ) );  \
}                                                                             \
                                                                              \
bool                                                                          \
operator op ( const sc_fxnum_subref& a, const sc_fxnum_fast_subref& b )       \
{                                                                             \
    return ( static_cast<sc_bv_base>( a ) op static_cast<sc_bv_base>( b ) );  \
}                                                                             \
                                                                              \
DEFN_REL_OP_T(op,const sc_bv_base&)                                           \
DEFN_REL_OP_T(op,const sc_lv_base&)                                           \
DEFN_REL_OP_T(op,const char*)                                                 \
DEFN_REL_OP_T(op,const bool*)                                                 \
DEFN_REL_OP_T(op,const sc_signed&)                                            \
DEFN_REL_OP_T(op,const sc_unsigned&)                                          \
DEFN_REL_OP_T(op,int)                                                         \
DEFN_REL_OP_T(op,unsigned int)                                                \
DEFN_REL_OP_T(op,long)                                                        \
DEFN_REL_OP_T(op,unsigned long)

DEFN_REL_OP(==)
DEFN_REL_OP(!=)

#undef DEFN_REL_OP_T
#undef DEFN_REL_OP


// reduce functions

#define DEFN_RED_FNC(fnc)                                                     \
bool                                                                          \
sc_fxnum_subref:: ## fnc ## () const                                          \
{                                                                             \
    get();                                                                    \
    return _bv. ## fnc ##();                                                  \
}

DEFN_RED_FNC(and_reduce)
DEFN_RED_FNC(nand_reduce)
DEFN_RED_FNC(or_reduce)
DEFN_RED_FNC(nor_reduce)
DEFN_RED_FNC(xor_reduce)
DEFN_RED_FNC(xnor_reduce)

#undef DEFN_RED_FNC


// query parameter

int
sc_fxnum_subref::length() const
{
    return _bv.length();
}


// explicit conversions

int
sc_fxnum_subref::to_signed() const
{
    get();
    return _bv.to_signed();
}

unsigned int
sc_fxnum_subref::to_unsigned() const
{
    get();
    return _bv.to_unsigned();
}

long
sc_fxnum_subref::to_long() const
{
    get();
    return _bv.to_long();
}

unsigned long
sc_fxnum_subref::to_ulong() const
{
    get();
    return _bv.to_ulong();
}


const sc_string
sc_fxnum_subref::to_string() const
{
    get();
    return _bv.to_string();
}


// implicit conversion

sc_fxnum_subref::operator sc_bv_base () const
{
    get();
    return _bv;
}


// print or dump content

void
sc_fxnum_subref::print( ostream& os ) const
{
    get();
    _bv.print( os );
}

void
sc_fxnum_subref::dump( ostream& os ) const
{
    os << "sc_fxnum_subref" << endl;
    os << "(" << endl;
    os << "num  = ";
    _num.dump( os );
    os << "from = " << _from << endl;
    os << "to   = " << _to << endl;
    os << ")" << endl;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast_subref
//
//  Proxy class for part-selection in class sc_fxnum_fast,
//  behaves like sc_bv_base.
// ----------------------------------------------------------------------------

bool
sc_fxnum_fast_subref::get() const
{
    return _num._val.get_slice( _from, _to, _num._params, _bv );
}

bool
sc_fxnum_fast_subref::set()
{
    return _num._val.set_slice( _from, _to, _num._params, _bv );
}


// constructor

sc_fxnum_fast_subref::sc_fxnum_fast_subref( sc_fxnum_fast& num_,
					    int from_, int to_ )
: _num( num_ ), _from( from_ ), _to( to_ ),
  _bv( *new sc_bv_base( MAXT( _from, _to ) - MINT( _from, _to ) + 1 ) )
{}


// assignment operators

sc_fxnum_fast_subref&
sc_fxnum_fast_subref::operator = ( const sc_fxnum_subref& a )
{
    _bv = static_cast<sc_bv_base>( a );
    set();
    return *this;
}

sc_fxnum_fast_subref&
sc_fxnum_fast_subref::operator = ( const sc_fxnum_fast_subref& a )
{
    if( &a != this )
    {
	_bv = static_cast<sc_bv_base>( a );
	set();
    }
    return *this;
}

#define DEFN_ASN_OP_T(tp)                                                     \
sc_fxnum_fast_subref&                                                         \
sc_fxnum_fast_subref::operator = ( tp a )                                     \
{                                                                             \
    _bv = a;                                                                  \
    set();                                                                    \
    return *this;                                                             \
}

DEFN_ASN_OP_T(const sc_bv_base&)
DEFN_ASN_OP_T(const sc_lv_base&)
DEFN_ASN_OP_T(const sc_bool_vector&)
DEFN_ASN_OP_T(const char*)
DEFN_ASN_OP_T(const bool*)
DEFN_ASN_OP_T(const sc_signed&)
DEFN_ASN_OP_T(const sc_unsigned&)
DEFN_ASN_OP_T(const sc_int_base&)
DEFN_ASN_OP_T(const sc_uint_base&)
DEFN_ASN_OP_T(int64)
DEFN_ASN_OP_T(uint64)
DEFN_ASN_OP_T(int)
DEFN_ASN_OP_T(unsigned int)
DEFN_ASN_OP_T(long)
DEFN_ASN_OP_T(unsigned long)
DEFN_ASN_OP_T(char)

#undef DEFN_ASN_OP_T


#define DEFN_ASN_OP_T_A(op,tp)                                                \
sc_fxnum_fast_subref&                                                         \
sc_fxnum_fast_subref::operator op ## = ( tp a )                               \
{                                                                             \
    get();                                                                    \
    _bv = _bv op static_cast<sc_bv_base>( a );                                \
    set();                                                                    \
    return *this;                                                             \
}

#define DEFN_ASN_OP_T_B(op,tp)                                                \
sc_fxnum_fast_subref&                                                         \
sc_fxnum_fast_subref::operator op ## = ( tp a )                               \
{                                                                             \
    get();                                                                    \
    _bv = _bv op a;                                                           \
    set();                                                                    \
    return *this;                                                             \
}

#define DEFN_ASN_OP_A(op)                                                     \
DEFN_ASN_OP_T_A(op,const sc_fxnum_subref&)                                    \
DEFN_ASN_OP_T_A(op,const sc_fxnum_fast_subref&)                               \
DEFN_ASN_OP_T_B(op,const sc_bv_base&)                                         \
DEFN_ASN_OP_T_B(op,const sc_lv_base&)

DEFN_ASN_OP_A(&)
DEFN_ASN_OP_A(|)
DEFN_ASN_OP_A(^)

#undef DEFN_ASN_OP_T_A
#undef DEFN_ASN_OP_T_B
#undef DEFN_ASN_OP_A


// relational operators

#define DEFN_REL_OP_T(op,tp)                                                  \
bool                                                                          \
operator op ( const sc_fxnum_fast_subref& a, tp b )                           \
{                                                                             \
    return ( static_cast<sc_bv_base>( a ) op b );                             \
}                                                                             \
                                                                              \
bool                                                                          \
operator op ( tp a, const sc_fxnum_fast_subref& b )                           \
{                                                                             \
    return ( static_cast<sc_bv_base>( b ) op a );                             \
}

#define DEFN_REL_OP(op)                                                       \
bool                                                                          \
operator op ( const sc_fxnum_fast_subref& a, const sc_fxnum_fast_subref& b )  \
{                                                                             \
    return ( static_cast<sc_bv_base>( a ) op static_cast<sc_bv_base>( b ) );  \
}                                                                             \
                                                                              \
bool                                                                          \
operator op ( const sc_fxnum_fast_subref& a, const sc_fxnum_subref& b )       \
{                                                                             \
    return ( static_cast<sc_bv_base>( a ) op static_cast<sc_bv_base>( b ) );  \
}                                                                             \
                                                                              \
DEFN_REL_OP_T(op,const sc_bv_base&)                                           \
DEFN_REL_OP_T(op,const sc_lv_base&)                                           \
DEFN_REL_OP_T(op,const char*)                                                 \
DEFN_REL_OP_T(op,const bool*)                                                 \
DEFN_REL_OP_T(op,const sc_signed&)                                            \
DEFN_REL_OP_T(op,const sc_unsigned&)                                          \
DEFN_REL_OP_T(op,int)                                                         \
DEFN_REL_OP_T(op,unsigned int)                                                \
DEFN_REL_OP_T(op,long)                                                        \
DEFN_REL_OP_T(op,unsigned long)

DEFN_REL_OP(==)
DEFN_REL_OP(!=)

#undef DEFN_REL_OP_T
#undef DEFN_REL_OP


// reduce functions

#define DEFN_RED_FNC(fnc)                                                     \
bool                                                                          \
sc_fxnum_fast_subref:: ## fnc ## () const                                     \
{                                                                             \
    get();                                                                    \
    return _bv. ## fnc ##();                                                  \
}

DEFN_RED_FNC(and_reduce)
DEFN_RED_FNC(nand_reduce)
DEFN_RED_FNC(or_reduce)
DEFN_RED_FNC(nor_reduce)
DEFN_RED_FNC(xor_reduce)
DEFN_RED_FNC(xnor_reduce)

#undef DEFN_RED_FNC


// query parameter

int
sc_fxnum_fast_subref::length() const
{
    return _bv.length();
}


// explicit conversions

int
sc_fxnum_fast_subref::to_signed() const
{
    get();
    return _bv.to_signed();
}

unsigned int
sc_fxnum_fast_subref::to_unsigned() const
{
    get();
    return _bv.to_unsigned();
}

long
sc_fxnum_fast_subref::to_long() const
{
    get();
    return _bv.to_long();
}

unsigned long
sc_fxnum_fast_subref::to_ulong() const
{
    get();
    return _bv.to_ulong();
}


const sc_string
sc_fxnum_fast_subref::to_string() const
{
    get();
    return _bv.to_string();
}


// implicit conversion

sc_fxnum_fast_subref::operator sc_bv_base () const
{
    get();
    return _bv;
}


// print or dump content

void
sc_fxnum_fast_subref::print( ostream& os ) const
{
    get();
    _bv.print( os );
}

void
sc_fxnum_fast_subref::dump( ostream& os ) const
{
    os << "sc_fxnum_fast_subref" << endl;
    os << "(" << endl;
    os << "num  = ";
    _num.dump( os );
    os << "from = " << _from << endl;
    os << "to   = " << _to << endl;
    os << ")" << endl;
}


// Taf!
