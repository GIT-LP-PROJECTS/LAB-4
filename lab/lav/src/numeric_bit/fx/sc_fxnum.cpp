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

    sc_fxnum.cpp - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include "sc_fxnum.h"

#include "sc_bv.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum
//
//  Base class for the fixed-point types; arbitrary precision.
// ----------------------------------------------------------------------------

// constructors

#define DEFN_CTOR_T(tp)                                                       \
sc_fxnum::sc_fxnum( tp a,                                                     \
		    const sc_fxtype_params& type_params_,                     \
		    sc_enc enc_,                                              \
		    const sc_fxcast_switch& cast_sw,                          \
		    sc_fxnum_observer* observer_ )                            \
: _val( a ),                                                                  \
  _params( type_params_, enc_, cast_sw ),                                     \
  _q_flag( false ),                                                           \
  _o_flag( false ),                                                           \
  _observer( observer_ )                                                      \
{                                                                             \
    cast();                                                                   \
}

DEFN_CTOR_T(const sc_int_base&)
DEFN_CTOR_T(const sc_uint_base&)

#undef DEFN_CTOR_T


// binary operators

#define DEFN_BIN_OP_T_A(op,tp)                                                \
const sc_fxval                                                                \
operator op ( const sc_fxnum& a, tp b )                                       \
{                                                                             \
    return sc_fxval( a._val op b );                                           \
}                                                                             \
                                                                              \
const sc_fxval                                                                \
operator op ( tp a, const sc_fxnum& b )                                       \
{                                                                             \
    return sc_fxval( a op b._val );                                           \
}

#define DEFN_BIN_OP(op)                                                       \
DEFN_BIN_OP_T_A(op,const sc_int_base&)                                        \
DEFN_BIN_OP_T_A(op,const sc_uint_base&)

DEFN_BIN_OP(*)
DEFN_BIN_OP(+)
DEFN_BIN_OP(-)
// DEFN_BIN_OP(/) // BCB produces illegal symbol
DEFN_BIN_OP_T_A(/,const sc_int_base&)
DEFN_BIN_OP_T_A(/,const sc_uint_base&)

#undef DEFN_BIN_OP_T_A
#undef DEFN_BIN_OP


// binary functions

#define DEFN_BIN_FNC_T_A(fnc,tp)                                              \
void                                                                          \
fnc ( sc_fxval& c, const sc_fxnum& a, tp b )                                  \
{                                                                             \
    fnc( c, a._val, b );                                                      \
}                                                                             \
                                                                              \
void                                                                          \
fnc ( sc_fxval& c, tp a, const sc_fxnum& b )                                  \
{                                                                             \
    fnc( c, a, b._val );                                                      \
}                                                                             \
                                                                              \
void                                                                          \
fnc ( sc_fxnum& c, const sc_fxnum& a, tp b )                                  \
{                                                                             \
    fnc( c._val, a._val, b );                                                 \
    c.cast();                                                                 \
}                                                                             \
                                                                              \
void                                                                          \
fnc ( sc_fxnum& c, tp a, const sc_fxnum& b )                                  \
{                                                                             \
    fnc( c._val, a, b._val );                                                 \
    c.cast();                                                                 \
}

#define DEFN_BIN_FNC(fnc)                                                     \
DEFN_BIN_FNC_T_A(fnc,const sc_int_base&)                                      \
DEFN_BIN_FNC_T_A(fnc,const sc_uint_base&)

DEFN_BIN_FNC(mult)
DEFN_BIN_FNC(div)
DEFN_BIN_FNC(add)
DEFN_BIN_FNC(sub)

#undef DEFN_BIN_FNC_T_A
#undef DEFN_BIN_FNC


// relational (including equality) operators

#define DEFN_REL_OP_T_A(op,tp)                                                \
bool                                                                          \
operator op ( const sc_fxnum& a, tp b )                                       \
{                                                                             \
    return ( a._val op b );                                                   \
}                                                                             \
                                                                              \
bool                                                                          \
operator op ( tp a, const sc_fxnum& b )                                       \
{                                                                             \
    return ( a op b._val );                                                   \
}

#define DEFN_REL_OP(op)                                                       \
DEFN_REL_OP_T_A(op,const sc_int_base&)                                        \
DEFN_REL_OP_T_A(op,const sc_uint_base&)

DEFN_REL_OP(<)
DEFN_REL_OP(<=)
DEFN_REL_OP(>)
DEFN_REL_OP(>=)
DEFN_REL_OP(==)
DEFN_REL_OP(!=)

#undef DEFN_REL_OP_T_A
#undef DEFN_REL_OP


// assignment operators

#define DEFN_ASN_OP_T_A(op,tp)                                                \
sc_fxnum&                                                                     \
sc_fxnum::operator op ( tp b )                                                \
{                                                                             \
    _val op b;                                                                \
    cast();                                                                   \
    return *this;                                                             \
}

#define DEFN_ASN_OP(op)                                                       \
DEFN_ASN_OP_T_A(op,const sc_int_base&)                                        \
DEFN_ASN_OP_T_A(op,const sc_uint_base&)

DEFN_ASN_OP(=)

DEFN_ASN_OP(*=)
DEFN_ASN_OP(/=)
DEFN_ASN_OP(+=)
DEFN_ASN_OP(-=)

#undef DEFN_ASN_OP_T_A
#undef DEFN_ASN_OP


// explicit conversion to character string

const sc_string
sc_fxnum::to_string() const
{
    return to_string( SC_DEC, SC_F );
}

const sc_string
sc_fxnum::to_string( sc_numrep numrep ) const
{
    return to_string( numrep, SC_F );
}

const sc_string
sc_fxnum::to_string( sc_fmt fmt ) const
{
    return to_string( SC_DEC, fmt );
}

const sc_string
sc_fxnum::to_string( sc_numrep numrep, sc_fmt fmt ) const
{
    return _val.to_string( numrep, fmt, _params );
}


const sc_string
sc_fxnum::to_dec() const
{
    return to_string( SC_DEC, SC_F );
}

const sc_string
sc_fxnum::to_bin() const
{
    return to_string( SC_BIN, SC_F );
}

const sc_string
sc_fxnum::to_oct() const
{
    return to_string( SC_OCT, SC_F );
}

const sc_string
sc_fxnum::to_hex() const
{
    return to_string( SC_HEX, SC_F );
}


// print or dump content

void
sc_fxnum::print( ostream& os ) const
{
    os << to_string( SC_DEC, SC_F );
}

void
sc_fxnum::dump( ostream& os ) const
{
    os << "sc_fxnum" << endl;
    os << "(" << endl;
    os << "val      = ";
    _val.dump( os );
    os << "params   = ";
    _params.dump( os );
    os << "q_flag   = " << _q_flag << endl;
    os << "o_flag   = " << _o_flag << endl;
    // TO BE COMPLETED
    // os << "observer = ";
    // if( _observer != 0 )
    //     _observer->dump( os );
    // else
    //     os << "0" << endl;
    os << ")" << endl;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast
//
//  Base class for the fixed-point types; limited precision.
// ----------------------------------------------------------------------------

// constructors

#define DEFN_CTOR_T(tp)                                                       \
sc_fxnum_fast::sc_fxnum_fast( tp a,                                           \
		              const sc_fxtype_params& type_params_,           \
		              sc_enc enc_,                                    \
		              const sc_fxcast_switch& cast_sw,                \
		              sc_fxnum_fast_observer* observer_ )             \
: _val( a ),                                                                  \
  _params( type_params_, enc_, cast_sw ),                                     \
  _q_flag( false ),                                                           \
  _o_flag( false ),                                                           \
  _observer( observer_ )                                                      \
{                                                                             \
    cast();                                                                   \
}

DEFN_CTOR_T(const sc_int_base&)
DEFN_CTOR_T(const sc_uint_base&)

#undef DEFN_CTOR_T


// binary operators

#define DEFN_BIN_OP_T(op,tp)                                                  \
const sc_fxval_fast                                                           \
operator op ( const sc_fxnum_fast& a, tp b )                                  \
{                                                                             \
    return sc_fxval_fast( a._val op b );                                      \
}                                                                             \
                                                                              \
const sc_fxval_fast                                                           \
operator op ( tp a, const sc_fxnum_fast& b )                                  \
{                                                                             \
    return sc_fxval_fast( a op b._val );                                      \
}

#define DEFN_BIN_OP(op)                                                       \
DEFN_BIN_OP_T(op,const sc_int_base&)                                          \
DEFN_BIN_OP_T(op,const sc_uint_base&)

DEFN_BIN_OP(*)
DEFN_BIN_OP(+)
DEFN_BIN_OP(-)
// DEFN_BIN_OP(/) // BCB produces illegal symbol
DEFN_BIN_OP_T(/,const sc_int_base&)
DEFN_BIN_OP_T(/,const sc_uint_base&)

#undef DEFN_BIN_OP_T
#undef DEFN_BIN_OP


// binary functions

#define DEFN_BIN_FNC_T(fnc,tp)                                                \
void                                                                          \
fnc ( sc_fxval_fast& c, const sc_fxnum_fast& a, tp b )                        \
{                                                                             \
    fnc( c, a._val, b );                                                      \
}                                                                             \
                                                                              \
void                                                                          \
fnc ( sc_fxval_fast& c, tp a, const sc_fxnum_fast& b )                        \
{                                                                             \
    fnc( c, a, b._val );                                                      \
}                                                                             \
                                                                              \
void                                                                          \
fnc ( sc_fxnum_fast& c, const sc_fxnum_fast& a, tp b )                        \
{                                                                             \
    fnc( c._val, a._val, b );                                                 \
    c.cast();                                                                 \
}                                                                             \
                                                                              \
void                                                                          \
fnc ( sc_fxnum_fast& c, tp a, const sc_fxnum_fast& b )                        \
{                                                                             \
    fnc( c._val, a, b._val );                                                 \
    c.cast();                                                                 \
}

#define DEFN_BIN_FNC(fnc)                                                     \
DEFN_BIN_FNC_T(fnc,const sc_int_base&)                                        \
DEFN_BIN_FNC_T(fnc,const sc_uint_base&)

DEFN_BIN_FNC(mult)
DEFN_BIN_FNC(div)
DEFN_BIN_FNC(add)
DEFN_BIN_FNC(sub)

#undef DEFN_BIN_FNC_T
#undef DEFN_BIN_FNC


// relational (including equality) operators

#define DEFN_REL_OP_T(op,tp)                                                \
bool                                                                          \
operator op ( const sc_fxnum_fast& a, tp b )                                  \
{                                                                             \
    return ( a._val op b );                                                   \
}                                                                             \
                                                                              \
bool                                                                          \
operator op ( tp a, const sc_fxnum_fast& b )                                  \
{                                                                             \
    return ( a op b._val );                                                   \
}

#define DEFN_REL_OP(op)                                                       \
DEFN_REL_OP_T(op,const sc_int_base&)                                          \
DEFN_REL_OP_T(op,const sc_uint_base&)

DEFN_REL_OP(<)
DEFN_REL_OP(<=)
DEFN_REL_OP(>)
DEFN_REL_OP(>=)
DEFN_REL_OP(==)
DEFN_REL_OP(!=)

#undef DEFN_REL_OP_T
#undef DEFN_REL_OP


// assignment operators

#define DEFN_ASN_OP_T_A(op,tp)                                                \
sc_fxnum_fast&                                                                \
sc_fxnum_fast::operator op ( tp b )                                           \
{                                                                             \
    _val op b;                                                                \
    cast();                                                                   \
    return *this;                                                             \
}

#define DEFN_ASN_OP(op)                                                       \
DEFN_ASN_OP_T_A(op,const sc_int_base&)                                        \
DEFN_ASN_OP_T_A(op,const sc_uint_base&)

DEFN_ASN_OP(=)

DEFN_ASN_OP(*=)
DEFN_ASN_OP(/=)
DEFN_ASN_OP(+=)
DEFN_ASN_OP(-=)

#undef DEFN_ASN_OP_T_A
#undef DEFN_ASN_OP


// explicit conversion to character string

const sc_string
sc_fxnum_fast::to_string() const
{
    return to_string( SC_DEC, SC_F );
}

const sc_string
sc_fxnum_fast::to_string( sc_numrep numrep ) const
{
    return to_string( numrep, SC_F );
}

const sc_string
sc_fxnum_fast::to_string( sc_fmt fmt ) const
{
    return to_string( SC_DEC, fmt );
}

const sc_string
sc_fxnum_fast::to_string( sc_numrep numrep, sc_fmt fmt ) const
{
    return _val.to_string( numrep, fmt, _params );
}


const sc_string
sc_fxnum_fast::to_dec() const
{
    return to_string( SC_DEC, SC_F );
}

const sc_string
sc_fxnum_fast::to_bin() const
{
    return to_string( SC_BIN, SC_F );
}

const sc_string
sc_fxnum_fast::to_oct() const
{
    return to_string( SC_OCT, SC_F );
}

const sc_string
sc_fxnum_fast::to_hex() const
{
    return to_string( SC_HEX, SC_F );
}


// print or dump content

void
sc_fxnum_fast::print( ostream& os ) const
{
    os << to_string( SC_DEC, SC_F );
}

void
sc_fxnum_fast::dump( ostream& os ) const
{
    os << "sc_fxnum_fast" << endl;
    os << "(" << endl;
    os << "val      = ";
    _val.dump( os );
    os << "params   = ";
    _params.dump( os );
    os << "q_flag   = " << _q_flag << endl;
    os << "o_flag   = " << _o_flag << endl;
    // TO BE COMPLETED
    // os << "observer = ";
    // if( _observer != 0 )
    //     _observer->dump( os );
    // else
    //     os << "0" << endl;
    os << ")" << endl;
}


// Taf!
