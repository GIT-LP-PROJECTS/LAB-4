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

    sc_ufix.cpp - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include "sc_ufix.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_ufix
//
//  "Unconstrained" unsigned fixed-point class; arbitrary precision.
// ----------------------------------------------------------------------------

// unary bitwise operators

const sc_ufix
sc_ufix::operator ~ () const
{
    int iwl_c = iwl();
    int wl_c = wl();
    sc_ufix c( wl_c, iwl_c );
    for( int i = iwl_c - wl_c; i < iwl_c; ++ i )
	c.set_bit( i, ! value().get_bit( i ) );
    return sc_ufix( c, wl_c, iwl_c );
}


// unary bitwise functions

void
b_not( sc_ufix& c, const sc_ufix& a )
{
    int iwl_c = c.iwl();
    for( int i = iwl_c - c.wl(); i < iwl_c; ++ i )
	c.set_bit( i, ! a.value().get_bit( i ) );
    c.cast();
}


// binary bitwise operators

#define DEFN_BIN_OP_T(op,op2,tp1,tp2)                                         \
const sc_ufix                                                                 \
operator op ( const tp1& a, const tp2& b )                                    \
{                                                                             \
    int iwl_a = a.iwl();                                                      \
    int iwl_b = b.iwl();                                                      \
    int iwl_c = MAXT( iwl_a, iwl_b );                                         \
    int fwl_c = MAXT( a.wl() - iwl_a, b.wl() - iwl_b );                       \
    sc_ufix c( iwl_c + fwl_c, iwl_c );                                        \
    for( int i = -fwl_c; i < iwl_c; ++ i )                                    \
	c.set_bit( i, a.value().get_bit( i ) op2 b.value().get_bit( i ) );    \
    return sc_ufix( c, iwl_c + fwl_c, iwl_c );                                \
}

DEFN_BIN_OP_T(&,&&,sc_ufix,sc_ufix)
DEFN_BIN_OP_T(&,&&,sc_ufix,sc_ufix_fast)
DEFN_BIN_OP_T(&,&&,sc_ufix_fast,sc_ufix)

DEFN_BIN_OP_T(|,||,sc_ufix,sc_ufix)
DEFN_BIN_OP_T(|,||,sc_ufix,sc_ufix_fast)
DEFN_BIN_OP_T(|,||,sc_ufix_fast,sc_ufix)

DEFN_BIN_OP_T(^,!=,sc_ufix,sc_ufix)
DEFN_BIN_OP_T(^,!=,sc_ufix,sc_ufix_fast)
DEFN_BIN_OP_T(^,!=,sc_ufix_fast,sc_ufix)

#undef DEFN_BIN_OP_T


// binary bitwise functions

#define DEFN_BIN_FNC_T(fnc,op2,tp1,tp2)                                       \
void                                                                          \
fnc ( sc_ufix& c, const tp1& a, const tp2& b )                                \
{                                                                             \
    int iwl_c = c.iwl();                                                      \
    for( int i = iwl_c - c.wl(); i < iwl_c; ++ i )                            \
	c.set_bit( i, a.value().get_bit( i ) op2 b.value().get_bit( i ) );    \
    c.cast();                                                                 \
}

DEFN_BIN_FNC_T(b_and,&&,sc_ufix,sc_ufix)
DEFN_BIN_FNC_T(b_and,&&,sc_ufix,sc_ufix_fast)
DEFN_BIN_FNC_T(b_and,&&,sc_ufix_fast,sc_ufix)

DEFN_BIN_FNC_T(b_or,||,sc_ufix,sc_ufix)
DEFN_BIN_FNC_T(b_or,||,sc_ufix,sc_ufix_fast)
DEFN_BIN_FNC_T(b_or,||,sc_ufix_fast,sc_ufix)

DEFN_BIN_FNC_T(b_xor,!=,sc_ufix,sc_ufix)
DEFN_BIN_FNC_T(b_xor,!=,sc_ufix,sc_ufix_fast)
DEFN_BIN_FNC_T(b_xor,!=,sc_ufix_fast,sc_ufix)

#undef DEFN_BIN_FNC_T


// assignment operators

#define DEFN_ASN_OP_T(op,op2,tp)                                              \
sc_ufix&                                                                      \
sc_ufix::operator op ( const tp& b )                                          \
{                                                                             \
    int iwl_c = iwl();                                                        \
    for( int i = iwl_c - wl(); i < iwl_c; ++ i )                              \
	set_bit( i, value().get_bit( i ) op2 b.value().get_bit( i ) );        \
    cast();                                                                   \
    return *this;                                                             \
}

DEFN_ASN_OP_T(&=,&&,sc_ufix)
DEFN_ASN_OP_T(&=,&&,sc_ufix_fast)
DEFN_ASN_OP_T(|=,||,sc_ufix)
DEFN_ASN_OP_T(|=,||,sc_ufix_fast)
DEFN_ASN_OP_T(^=,!=,sc_ufix)
DEFN_ASN_OP_T(^=,!=,sc_ufix_fast)

#undef DEFN_ASN_OP_T


// ----------------------------------------------------------------------------
//  CLASS : sc_ufix_fast
//
//  "Unconstrained" unsigned fixed-point class; limited precision.
// ----------------------------------------------------------------------------

// unary bitwise operators

const sc_ufix_fast
sc_ufix_fast::operator ~ () const
{
    int iwl_c = iwl();
    int wl_c = wl();
    sc_ufix_fast c( wl_c, iwl_c );
    for( int i = iwl_c - wl_c; i < iwl_c; ++ i )
	c.set_bit( i, ! value().get_bit( i ) );
    return sc_ufix_fast( c, wl_c, iwl_c );
}


// unary bitwise functions

void
b_not( sc_ufix_fast& c, const sc_ufix_fast& a )
{
    int iwl_c = c.iwl();
    for( int i = iwl_c - c.wl(); i < iwl_c; ++ i )
	c.set_bit( i, ! a.value().get_bit( i ) );
    c.cast();
}


// binary bitwise operators

#define DEFN_BIN_OP_T(op,op2,tp1,tp2)                                         \
const sc_ufix_fast                                                            \
operator op ( const tp1& a, const tp2& b )                                    \
{                                                                             \
    int iwl_a = a.iwl();                                                      \
    int iwl_b = b.iwl();                                                      \
    int iwl_c = MAXT( iwl_a, iwl_b );                                         \
    int fwl_c = MAXT( a.wl() - iwl_a, b.wl() - iwl_b );                       \
    sc_ufix_fast c( iwl_c + fwl_c, iwl_c );                                   \
    for( int i = -fwl_c; i < iwl_c; ++ i )                                    \
	c.set_bit( i, a.value().get_bit( i ) op2 b.value().get_bit( i ) );    \
    return sc_ufix_fast( c, iwl_c + fwl_c, iwl_c );                           \
}

DEFN_BIN_OP_T(&,&&,sc_ufix_fast,sc_ufix_fast)
DEFN_BIN_OP_T(|,||,sc_ufix_fast,sc_ufix_fast)
DEFN_BIN_OP_T(^,!=,sc_ufix_fast,sc_ufix_fast)

#undef DEFN_BIN_OP_T


// binary bitwise functions

#define DEFN_BIN_FNC_T(fnc,op2,tp1,tp2)                                       \
void                                                                          \
fnc ( sc_ufix_fast& c, const tp1& a, const tp2& b )                           \
{                                                                             \
    int iwl_c = c.iwl();                                                      \
    for( int i = iwl_c - c.wl(); i < iwl_c; ++ i )                            \
	c.set_bit( i, a.value().get_bit( i ) op2 b.value().get_bit( i ) );    \
    c.cast();                                                                 \
}

DEFN_BIN_FNC_T(b_and,&&,sc_ufix_fast,sc_ufix_fast)
DEFN_BIN_FNC_T(b_or,||,sc_ufix_fast,sc_ufix_fast)
DEFN_BIN_FNC_T(b_xor,!=,sc_ufix_fast,sc_ufix_fast)

#undef DEFN_BIN_FNC_T


// assignment operators

#define DEFN_ASN_OP_T(op,op2,tp)                                              \
sc_ufix_fast&                                                                 \
sc_ufix_fast::operator op ( const tp& b )                                     \
{                                                                             \
    int iwl_c = iwl();                                                        \
    for( int i = iwl_c - wl(); i < iwl_c; ++ i )                              \
	set_bit( i, value().get_bit( i ) op2 b.value().get_bit( i ) );        \
    cast();                                                                   \
    return *this;                                                             \
}

DEFN_ASN_OP_T(&=,&&,sc_ufix)
DEFN_ASN_OP_T(&=,&&,sc_ufix_fast)
DEFN_ASN_OP_T(|=,||,sc_ufix)
DEFN_ASN_OP_T(|=,||,sc_ufix_fast)
DEFN_ASN_OP_T(^=,!=,sc_ufix)
DEFN_ASN_OP_T(^=,!=,sc_ufix_fast)

#undef DEFN_ASN_OP_T


// Taf!
