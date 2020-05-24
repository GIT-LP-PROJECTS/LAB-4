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

    sc_fxnum_subref.h - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_FXNUM_SUBREF_H
#define SC_FXNUM_SUBREF_H


#ifndef   SC_DEFS_H
#include "numeric_bit/sc_nbdefs.h"
#endif

#if defined(__BCPLUSPLUS__)
#pragma hdrstop
#endif

namespace sc_bv_ns
{
    class sc_bv_base;
    class sc_lv_base;
};
using sc_bv_ns::sc_bv_base;
using sc_bv_ns::sc_lv_base;
class sc_bool_vector;
class sc_signed;
class sc_unsigned;
class sc_int_base;
class sc_uint_base;
class sc_string;


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_subref
//
//  Proxy class for part-selection in class sc_fxnum,
//  behaves like sc_bv_base.
// ----------------------------------------------------------------------------

class sc_fxnum_subref
{

    friend class sc_fxnum;
    friend class sc_fxnum_fast_subref;

    bool get() const;
    bool set();


    // constructor

    sc_fxnum_subref( sc_fxnum&, int, int );

public:

    // assignment operators

#define DECL_ASN_OP_T(tp)                                                     \
    sc_fxnum_subref& operator = ( tp );

    DECL_ASN_OP_T(const sc_fxnum_subref&)
    DECL_ASN_OP_T(const sc_fxnum_fast_subref&)
    DECL_ASN_OP_T(const sc_bv_base&)
    DECL_ASN_OP_T(const sc_lv_base&)
    DECL_ASN_OP_T(const sc_bool_vector&)
    DECL_ASN_OP_T(const char*)
    DECL_ASN_OP_T(const bool*)
    DECL_ASN_OP_T(const sc_signed&)
    DECL_ASN_OP_T(const sc_unsigned&)
    DECL_ASN_OP_T(const sc_int_base&)
    DECL_ASN_OP_T(const sc_uint_base&)
    DECL_ASN_OP_T(int64)
    DECL_ASN_OP_T(uint64)
    DECL_ASN_OP_T(int)
    DECL_ASN_OP_T(unsigned int)
    DECL_ASN_OP_T(long)
    DECL_ASN_OP_T(unsigned long)
    DECL_ASN_OP_T(char)

#undef DECL_ASN_OP_T

#define DECL_ASN_OP_T_A(op,tp)                                                \
    sc_fxnum_subref& operator op ## = ( tp );

#define DECL_ASN_OP_A(op)                                                     \
    DECL_ASN_OP_T_A(op,const sc_fxnum_subref&)                                \
    DECL_ASN_OP_T_A(op,const sc_fxnum_fast_subref&)                           \
    DECL_ASN_OP_T_A(op,const sc_bv_base&)                                     \
    DECL_ASN_OP_T_A(op,const sc_lv_base&)

    DECL_ASN_OP_A(&)
    DECL_ASN_OP_A(|)
    DECL_ASN_OP_A(^)

#undef DECL_ASN_OP_T_A
#undef DECL_ASN_OP_A


    // relational operators

#define DECL_REL_OP_T(op,tp)                                                  \
    friend bool operator op ( const sc_fxnum_subref&, tp );                   \
    friend bool operator op ( tp, const sc_fxnum_subref& );

#define DECL_REL_OP(op)                                                       \
    friend bool operator op ( const sc_fxnum_subref&,                         \
			      const sc_fxnum_subref& );                       \
    friend bool operator op ( const sc_fxnum_subref&,                         \
			      const sc_fxnum_fast_subref& );                  \
    DECL_REL_OP_T(op,const sc_bv_base&)                                       \
    DECL_REL_OP_T(op,const sc_lv_base&)                                       \
    DECL_REL_OP_T(op,const char*)                                             \
    DECL_REL_OP_T(op,const bool*)                                             \
    DECL_REL_OP_T(op,const sc_signed&)                                        \
    DECL_REL_OP_T(op,const sc_unsigned&)                                      \
    DECL_REL_OP_T(op,int)                                                     \
    DECL_REL_OP_T(op,unsigned int)                                            \
    DECL_REL_OP_T(op,long)                                                    \
    DECL_REL_OP_T(op,unsigned long)

    DECL_REL_OP(==)
    DECL_REL_OP(!=)

#undef DECL_REL_OP_T
#undef DECL_REL_OP


    // reduce functions

    bool and_reduce() const;
    bool nand_reduce() const;
    bool or_reduce() const;
    bool nor_reduce() const;
    bool xor_reduce() const;
    bool xnor_reduce() const;


    // query parameter

    int length() const;


    // explicit conversions

    int           to_signed() const;
    unsigned int  to_unsigned() const;
    long          to_long() const;
    unsigned long to_ulong() const;

    const sc_string to_string() const;


    // implicit conversion

    operator sc_bv_base() const;


    // print or dump content

    void print( ostream& = cout ) const;
    void dump( ostream& = cout ) const;

private:

    sc_fxnum& _num;
    int       _from;
    int       _to;

    sc_bv_base& _bv;

};


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast_subref
//
//  Proxy class for part-selection in class sc_fxnum_fast,
//  behaves like sc_bv_base.
// ----------------------------------------------------------------------------

class sc_fxnum_fast_subref
{

    friend class sc_fxnum_fast;
    friend class sc_fxnum_subref;

    bool get() const;
    bool set();


    // constructor

    sc_fxnum_fast_subref( sc_fxnum_fast&, int, int );

public:

    // assignment operators

#define DECL_ASN_OP_T(tp)                                                     \
    sc_fxnum_fast_subref& operator = ( tp );

    DECL_ASN_OP_T(const sc_fxnum_subref&)
    DECL_ASN_OP_T(const sc_fxnum_fast_subref&)
    DECL_ASN_OP_T(const sc_bv_base&)
    DECL_ASN_OP_T(const sc_lv_base&)
    DECL_ASN_OP_T(const sc_bool_vector&)
    DECL_ASN_OP_T(const char*)
    DECL_ASN_OP_T(const bool*)
    DECL_ASN_OP_T(const sc_signed&)
    DECL_ASN_OP_T(const sc_unsigned&)
    DECL_ASN_OP_T(const sc_int_base&)
    DECL_ASN_OP_T(const sc_uint_base&)
    DECL_ASN_OP_T(int64)
    DECL_ASN_OP_T(uint64)
    DECL_ASN_OP_T(int)
    DECL_ASN_OP_T(unsigned int)
    DECL_ASN_OP_T(long)
    DECL_ASN_OP_T(unsigned long)
    DECL_ASN_OP_T(char)

#undef DECL_ASN_OP_T

#define DECL_ASN_OP_T_A(op,tp)                                                \
    sc_fxnum_fast_subref& operator op ## = ( tp );

#define DECL_ASN_OP_A(op)                                                     \
    DECL_ASN_OP_T_A(op,const sc_fxnum_subref&)                                \
    DECL_ASN_OP_T_A(op,const sc_fxnum_fast_subref&)                           \
    DECL_ASN_OP_T_A(op,const sc_bv_base&)                                     \
    DECL_ASN_OP_T_A(op,const sc_lv_base&)

    DECL_ASN_OP_A(&)
    DECL_ASN_OP_A(|)
    DECL_ASN_OP_A(^)

#undef DECL_ASN_OP_T_A
#undef DECL_ASN_OP_A


    // relational operators

#define DECL_REL_OP_T(op,tp)                                                  \
    friend bool operator op ( const sc_fxnum_fast_subref&, tp );              \
    friend bool operator op ( tp, const sc_fxnum_fast_subref& );

#define DECL_REL_OP(op)                                                       \
    friend bool operator op ( const sc_fxnum_fast_subref&,                    \
			      const sc_fxnum_fast_subref& );                  \
    friend bool operator op ( const sc_fxnum_fast_subref&,                    \
			      const sc_fxnum_subref& );                       \
    DECL_REL_OP_T(op,const sc_bv_base&)                                       \
    DECL_REL_OP_T(op,const sc_lv_base&)                                       \
    DECL_REL_OP_T(op,const char*)                                             \
    DECL_REL_OP_T(op,const bool*)                                             \
    DECL_REL_OP_T(op,const sc_signed&)                                        \
    DECL_REL_OP_T(op,const sc_unsigned&)                                      \
    DECL_REL_OP_T(op,int)                                                     \
    DECL_REL_OP_T(op,unsigned int)                                            \
    DECL_REL_OP_T(op,long)                                                    \
    DECL_REL_OP_T(op,unsigned long)

    DECL_REL_OP(==)
    DECL_REL_OP(!=)

#undef DECL_REL_OP_T
#undef DECL_REL_OP


    // reduce functions

    bool and_reduce() const;
    bool nand_reduce() const;
    bool or_reduce() const;
    bool nor_reduce() const;
    bool xor_reduce() const;
    bool xnor_reduce() const;


    // query parameter

    int length() const;


    // explicit conversions

    int           to_signed() const;
    unsigned int  to_unsigned() const;
    long          to_long() const;
    unsigned long to_ulong() const;

    const sc_string to_string() const;


    // implicit conversion

    operator sc_bv_base() const;


    // print or dump content

    void print( ostream& = cout ) const;
    void dump( ostream& = cout ) const;

private:

    sc_fxnum_fast& _num;
    int            _from;
    int            _to;

    sc_bv_base& _bv;

};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_subref
//
//  Proxy class for part-selection in class sc_fxnum,
//  behaves like sc_bv_base.
// ----------------------------------------------------------------------------

inline
ostream&
operator << ( ostream& os, const sc_fxnum_subref& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast_subref
//
//  Proxy class for part-selection in class sc_fxnum_fast,
//  behaves like sc_bv_base.
// ----------------------------------------------------------------------------

inline
ostream&
operator << ( ostream& os, const sc_fxnum_fast_subref& a )
{
    a.print( os );
    return os;
}


#endif

// Taf!
