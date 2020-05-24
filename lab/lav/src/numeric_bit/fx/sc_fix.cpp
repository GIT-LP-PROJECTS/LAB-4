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

    sc_fix.cpp - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include "sc_fix.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_fix
//
//  "Unconstrained" signed fixed-point class; arbitrary precision.
// ----------------------------------------------------------------------------

// unary bitwise operators

const sc_fix
sc_fix::operator ~ () const
{
    int iwl_c = iwl();
    int wl_c = wl();
    sc_fix c( wl_c, iwl_c );
    for( int i = iwl_c - wl_c; i < iwl_c; ++ i )
	c.set_bit( i, ! value().get_bit( i ) );
    return sc_fix( c, wl_c, iwl_c );
}


// unary bitwise functions

void
b_not( sc_fix& c, const sc_fix& a )
{
    int iwl_c = c.iwl();
    for( int i = iwl_c - c.wl(); i < iwl_c; ++ i )
	c.set_bit( i, ! a.value().get_bit( i ) );
    c.cast();
}


// binary bitwise operators

#define DEFN_BIN_OP_T(op,op2,tp1,tp2)                                         \
const sc_fix                                                                  \
operator op ( const tp1& a, const tp2& b )                                    \
{                                                                             \
    int iwl_a = a.iwl();                                                      \
    int iwl_b = b.iwl();                                                      \
    int iwl_c = MAXT( iwl_a, iwl_b );                                         \
    int fwl_c = MAXT( a.wl() - iwl_a, b.wl() - iwl_b );                       \
    sc_fix c( iwl_c + fwl_c, iwl_c );                                         \
    for( int i = -fwl_c; i < iwl_c; ++ i )                                    \
	c.set_bit( i, a.value().get_bit( i ) op2 b.value().get_bit( i ) );    \
    return sc_fix( c, iwl_c + fwl_c, iwl_c );                                 \
}

DEFN_BIN_OP_T(&,&&,sc_fix,sc_fix)
DEFN_BIN_OP_T(&,&&,sc_fix,sc_fix_fast)
DEFN_BIN_OP_T(&,&&,sc_fix_fast,sc_fix)

DEFN_BIN_OP_T(|,||,sc_fix,sc_fix)
DEFN_BIN_OP_T(|,||,sc_fix,sc_fix_fast)
DEFN_BIN_OP_T(|,||,sc_fix_fast,sc_fix)

DEFN_BIN_OP_T(^,!=,sc_fix,sc_fix)
DEFN_BIN_OP_T(^,!=,sc_fix,sc_fix_fast)
DEFN_BIN_OP_T(^,!=,sc_fix_fast,sc_fix)

#undef DEFN_BIN_OP_T


// binary bitwise functions

#define DEFN_BIN_FNC_T(fnc,op2,tp1,tp2)                                       \
void                                                                          \
fnc ( sc_fix& c, const tp1& a, const tp2& b )                                 \
{                                                                             \
    int iwl_c = c.iwl();                                                      \
    for( int i = iwl_c - c.wl(); i < iwl_c; ++ i )                            \
	c.set_bit( i, a.value().get_bit( i ) op2 b.value().get_bit( i ) );    \
    c.cast();                                                                 \
}

DEFN_BIN_FNC_T(b_and,&&,sc_fix,sc_fix)
DEFN_BIN_FNC_T(b_and,&&,sc_fix,sc_fix_fast)
DEFN_BIN_FNC_T(b_and,&&,sc_fix_fast,sc_fix)

DEFN_BIN_FNC_T(b_or,||,sc_fix,sc_fix)
DEFN_BIN_FNC_T(b_or,||,sc_fix,sc_fix_fast)
DEFN_BIN_FNC_T(b_or,||,sc_fix_fast,sc_fix)

DEFN_BIN_FNC_T(b_xor,!=,sc_fix,sc_fix)
DEFN_BIN_FNC_T(b_xor,!=,sc_fix,sc_fix_fast)
DEFN_BIN_FNC_T(b_xor,!=,sc_fix_fast,sc_fix)

#undef DEFN_BIN_FNC_T


// assignment operators

#define DEFN_ASN_OP_T(op,op2,tp)                                              \
sc_fix&                                                                       \
sc_fix::operator op ( const tp& b )                                           \
{                                                                             \
    int iwl_c = iwl();                                                        \
    for( int i = iwl_c - wl(); i < iwl_c; ++ i )                              \
	set_bit( i, value().get_bit( i ) op2 b.value().get_bit( i ) );        \
    cast();                                                                   \
    return *this;                                                             \
}

DEFN_ASN_OP_T(&=,&&,sc_fix)
DEFN_ASN_OP_T(&=,&&,sc_fix_fast)
DEFN_ASN_OP_T(|=,||,sc_fix)
DEFN_ASN_OP_T(|=,||,sc_fix_fast)
DEFN_ASN_OP_T(^=,!=,sc_fix)
DEFN_ASN_OP_T(^=,!=,sc_fix_fast)

#undef DEFN_ASN_OP_T


// ----------------------------------------------------------------------------
//  CLASS : sc_fix_fast
//
//  "Unconstrained" signed fixed-point class; limited precision.
// ----------------------------------------------------------------------------

// unary bitwise operators

const sc_fix_fast
sc_fix_fast::operator ~ () const
{
    int iwl_c = iwl();
    int wl_c = wl();
    sc_fix_fast c( wl_c, iwl_c );
    for( int i = iwl_c - wl_c; i < iwl_c; ++ i )
	c.set_bit( i, ! value().get_bit( i ) );
    return sc_fix_fast( c, wl_c, iwl_c );
}


// unary bitwise functions

void
b_not( sc_fix_fast& c, const sc_fix_fast& a )
{
    int iwl_c = c.iwl();
    for( int i = iwl_c - c.wl(); i < iwl_c; ++ i )
	c.set_bit( i, ! a.value().get_bit( i ) );
    c.cast();
}


// binary bitwise operators

#define DEFN_BIN_OP_T(op,op2,tp1,tp2)                                         \
const sc_fix_fast                                                             \
operator op ( const tp1& a, const tp2& b )                                    \
{                                                                             \
    int iwl_a = a.iwl();                                                      \
    int iwl_b = b.iwl();                                                      \
    int iwl_c = MAXT( iwl_a, iwl_b );                                         \
    int fwl_c = MAXT( a.wl() - iwl_a, b.wl() - iwl_b );                       \
    sc_fix_fast c( iwl_c + fwl_c, iwl_c );                                    \
    for( int i = -fwl_c; i < iwl_c; ++ i )                                    \
	c.set_bit( i, a.value().get_bit( i ) op2 b.value().get_bit( i ) );    \
    return sc_fix_fast( c, iwl_c + fwl_c, iwl_c );                            \
}

DEFN_BIN_OP_T(&,&&,sc_fix_fast,sc_fix_fast)
DEFN_BIN_OP_T(|,||,sc_fix_fast,sc_fix_fast)
DEFN_BIN_OP_T(^,!=,sc_fix_fast,sc_fix_fast)

#undef DEFN_BIN_OP_T


// binary bitwise functions

#define DEFN_BIN_FNC_T(fnc,op2,tp1,tp2)                                       \
void                                                                          \
fnc ( sc_fix_fast& c, const tp1& a, const tp2& b )                            \
{                                                                             \
    int iwl_c = c.iwl();                                                      \
    for( int i = iwl_c - c.wl(); i < iwl_c; ++ i )                            \
	c.set_bit( i, a.value().get_bit( i ) op2 b.value().get_bit( i ) );    \
    c.cast();                                                                 \
}

DEFN_BIN_FNC_T(b_and,&&,sc_fix_fast,sc_fix_fast)
DEFN_BIN_FNC_T(b_or,||,sc_fix_fast,sc_fix_fast)
DEFN_BIN_FNC_T(b_xor,!=,sc_fix_fast,sc_fix_fast)

#undef DEFN_BIN_FNC_T


// assignment operators

#define DEFN_ASN_OP_T(op,op2,tp)                                              \
sc_fix_fast&                                                                  \
sc_fix_fast::operator op ( const tp& b )                                      \
{                                                                             \
    int iwl_c = iwl();                                                        \
    for( int i = iwl_c - wl(); i < iwl_c; ++ i )                              \
	set_bit( i, value().get_bit( i ) op2 b.value().get_bit( i ) );        \
    cast();                                                                   \
    return *this;                                                             \
}

DEFN_ASN_OP_T(&=,&&,sc_fix)
DEFN_ASN_OP_T(&=,&&,sc_fix_fast)
DEFN_ASN_OP_T(|=,||,sc_fix)
DEFN_ASN_OP_T(|=,||,sc_fix_fast)
DEFN_ASN_OP_T(^=,!=,sc_fix)
DEFN_ASN_OP_T(^=,!=,sc_fix_fast)

#undef DEFN_ASN_OP_T


// Taf!
