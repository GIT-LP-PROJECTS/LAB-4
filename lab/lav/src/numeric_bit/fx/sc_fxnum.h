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

    sc_fxnum.h - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_FXNUM_H
#define SC_FXNUM_H


#ifndef   SC_FXVAL_H
#include "sc_fxval.h"
#endif

#ifndef   SCFX_PARAMS_H
#include "scfx_params.h"
#endif

#ifndef   SC_FXNUM_BITREF_H
#include "sc_fxnum_bitref.h"
#endif

#ifndef   SC_FXNUM_SUBREF_H
#include "sc_fxnum_subref.h"
#endif

#if defined(__BCPLUSPLUS__)
#pragma hdrstop
#endif

class sc_fxnum_observer;
class sc_fxnum_fast_observer;


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum
//
//  Base class for the fixed-point types; arbitrary precision.
// ----------------------------------------------------------------------------

class sc_fxnum
{

    friend class sc_fxnum_bitref;
    friend class sc_fxnum_subref;

    friend class isdb_sc_fxnum_trace;
    friend class vcd_sc_fxnum_trace;
    friend class wif_sc_fxnum_trace;

protected:

    void cast();


    // constructors

    sc_fxnum( const sc_fxtype_params&,
	      sc_enc,
	      const sc_fxcast_switch&,
	      sc_fxnum_observer* );

#define DECL_CTOR_T(tp)                                                       \
    sc_fxnum( tp,                                                             \
	      const sc_fxtype_params&,                                        \
	      sc_enc,                                                         \
	      const sc_fxcast_switch&,                                        \
	      sc_fxnum_observer* );

    DECL_CTOR_T(short)
    DECL_CTOR_T(unsigned short)
    DECL_CTOR_T(int)
    DECL_CTOR_T(unsigned int)
    DECL_CTOR_T(long)
    DECL_CTOR_T(unsigned long)
    DECL_CTOR_T(float)
    DECL_CTOR_T(double)
    DECL_CTOR_T(const char*)
    DECL_CTOR_T(int64)
    DECL_CTOR_T(uint64)
    DECL_CTOR_T(const sc_int_base&)
    DECL_CTOR_T(const sc_uint_base&)
    DECL_CTOR_T(const sc_signed&)
    DECL_CTOR_T(const sc_unsigned&)
    DECL_CTOR_T(const sc_fxval&)
    DECL_CTOR_T(const sc_fxval_fast&)
    DECL_CTOR_T(const sc_fxnum&)
    DECL_CTOR_T(const sc_fxnum_fast&)

#undef DECL_CTOR_T

    ~sc_fxnum();

public:

    // unary operators

    const sc_fxval  operator - () const;
    const sc_fxval& operator + () const;


    // unary functions

    friend void neg( sc_fxval&, const sc_fxnum& );
    friend void neg( sc_fxnum&, const sc_fxnum& );


    // binary operators

#define DECL_BIN_OP_T(op,tp)                                                  \
    friend const sc_fxval operator op ( const sc_fxnum&, tp );                \
    friend const sc_fxval operator op ( tp, const sc_fxnum& );

#define DECL_BIN_OP(op)                                                       \
    friend const sc_fxval operator op ( const sc_fxnum&, const sc_fxnum& );   \
    DECL_BIN_OP_T(op,short)                                                   \
    DECL_BIN_OP_T(op,unsigned short)                                          \
    DECL_BIN_OP_T(op,int)                                                     \
    DECL_BIN_OP_T(op,unsigned int)                                            \
    DECL_BIN_OP_T(op,long)                                                    \
    DECL_BIN_OP_T(op,unsigned long)                                           \
    DECL_BIN_OP_T(op,float)                                                   \
    DECL_BIN_OP_T(op,double)                                                  \
    DECL_BIN_OP_T(op,const char*)                                             \
    DECL_BIN_OP_T(op,int64)                                                   \
    DECL_BIN_OP_T(op,uint64)                                                  \
    DECL_BIN_OP_T(op,const sc_int_base&)                                      \
    DECL_BIN_OP_T(op,const sc_uint_base&)                                     \
    DECL_BIN_OP_T(op,const sc_signed&)                                        \
    DECL_BIN_OP_T(op,const sc_unsigned&)                                      \
    DECL_BIN_OP_T(op,const sc_fxval&)                                         \
    DECL_BIN_OP_T(op,const sc_fxval_fast&)                                    \
    DECL_BIN_OP_T(op,const sc_fxnum_fast&)

    DECL_BIN_OP(*)
    DECL_BIN_OP(+)
    DECL_BIN_OP(-)
//    DECL_BIN_OP(/) // BCB produces illegal character at this line
    friend const sc_fxval operator / ( const sc_fxnum&, const sc_fxnum& );
    DECL_BIN_OP_T(/,short)
    DECL_BIN_OP_T(/,unsigned short)
    DECL_BIN_OP_T(/,int)
    DECL_BIN_OP_T(/,unsigned int)
    DECL_BIN_OP_T(/,long)
    DECL_BIN_OP_T(/,unsigned long)
    DECL_BIN_OP_T(/,float)
    DECL_BIN_OP_T(/,double)
    DECL_BIN_OP_T(/,const char*)
    DECL_BIN_OP_T(/,int64)
    DECL_BIN_OP_T(/,uint64)
    DECL_BIN_OP_T(/,const sc_int_base&)
    DECL_BIN_OP_T(/,const sc_uint_base&)
    DECL_BIN_OP_T(/,const sc_signed&)
    DECL_BIN_OP_T(/,const sc_unsigned&)
    DECL_BIN_OP_T(/,const sc_fxval&)
    DECL_BIN_OP_T(/,const sc_fxval_fast&)
    DECL_BIN_OP_T(/,const sc_fxnum_fast&)

#undef DECL_BIN_OP_T
#undef DECL_BIN_OP

    friend const sc_fxval operator << ( const sc_fxnum&, int );
    friend const sc_fxval operator >> ( const sc_fxnum&, int );


    // binary functions

#define DECL_BIN_FNC_T(fnc,tp)                                                \
    friend void fnc ( sc_fxval&, const sc_fxnum&, tp );                       \
    friend void fnc ( sc_fxval&, tp, const sc_fxnum& );                       \
    friend void fnc ( sc_fxnum&, const sc_fxnum&, tp );                       \
    friend void fnc ( sc_fxnum&, tp, const sc_fxnum& );

#define DECL_BIN_FNC(fnc)                                                     \
    friend void fnc ( sc_fxval&, const sc_fxnum&, const sc_fxnum& );          \
    friend void fnc ( sc_fxnum&, const sc_fxnum&, const sc_fxnum& );          \
    DECL_BIN_FNC_T(fnc,short)                                                 \
    DECL_BIN_FNC_T(fnc,unsigned short)                                        \
    DECL_BIN_FNC_T(fnc,int)                                                   \
    DECL_BIN_FNC_T(fnc,unsigned int)                                          \
    DECL_BIN_FNC_T(fnc,long)                                                  \
    DECL_BIN_FNC_T(fnc,unsigned long)                                         \
    DECL_BIN_FNC_T(fnc,float)                                                 \
    DECL_BIN_FNC_T(fnc,double)                                                \
    DECL_BIN_FNC_T(fnc,const char*)                                           \
    DECL_BIN_FNC_T(fnc,int64)                                                 \
    DECL_BIN_FNC_T(fnc,uint64)                                                \
    DECL_BIN_FNC_T(fnc,const sc_int_base&)                                    \
    DECL_BIN_FNC_T(fnc,const sc_uint_base&)                                   \
    DECL_BIN_FNC_T(fnc,const sc_signed&)                                      \
    DECL_BIN_FNC_T(fnc,const sc_unsigned&)                                    \
    DECL_BIN_FNC_T(fnc,const sc_fxval&)                                       \
    DECL_BIN_FNC_T(fnc,const sc_fxval_fast&)                                  \
    DECL_BIN_FNC_T(fnc,const sc_fxnum_fast&)

    DECL_BIN_FNC(mult)
    DECL_BIN_FNC(div)
    DECL_BIN_FNC(add)
    DECL_BIN_FNC(sub)

#undef DECL_BIN_FNC_T
#undef DECL_BIN_FNC

    friend void lshift( sc_fxval&, const sc_fxnum&, int );
    friend void rshift( sc_fxval&, const sc_fxnum&, int );
    friend void lshift( sc_fxnum&, const sc_fxnum&, int );
    friend void rshift( sc_fxnum&, const sc_fxnum&, int );


    // relational (including equality) operators

#define DECL_REL_OP_T(op,tp)                                                  \
    friend bool operator op ( const sc_fxnum&, tp );                          \
    friend bool operator op ( tp, const sc_fxnum& );

#define DECL_REL_OP(op)                                                       \
    friend bool operator op ( const sc_fxnum&, const sc_fxnum& );             \
    DECL_REL_OP_T(op,short)                                                   \
    DECL_REL_OP_T(op,unsigned short)                                          \
    DECL_REL_OP_T(op,int)                                                     \
    DECL_REL_OP_T(op,unsigned int)                                            \
    DECL_REL_OP_T(op,long)                                                    \
    DECL_REL_OP_T(op,unsigned long)                                           \
    DECL_REL_OP_T(op,float)                                                   \
    DECL_REL_OP_T(op,double)                                                  \
    DECL_REL_OP_T(op,const char*)                                             \
    DECL_REL_OP_T(op,int64)                                                   \
    DECL_REL_OP_T(op,uint64)                                                  \
    DECL_REL_OP_T(op,const sc_int_base&)                                      \
    DECL_REL_OP_T(op,const sc_uint_base&)                                     \
    DECL_REL_OP_T(op,const sc_signed&)                                        \
    DECL_REL_OP_T(op,const sc_unsigned&)                                      \
    DECL_REL_OP_T(op,const sc_fxval&)                                         \
    DECL_REL_OP_T(op,const sc_fxval_fast&)                                    \
    DECL_REL_OP_T(op,const sc_fxnum_fast&)

    DECL_REL_OP(<)
    DECL_REL_OP(<=)
    DECL_REL_OP(>)
    DECL_REL_OP(>=)
    DECL_REL_OP(==)
    DECL_REL_OP(!=)

#undef DECL_REL_OP_T
#undef DECL_REL_OP


    // assignment operators

#define DECL_ASN_OP_T(op,tp)                                                  \
    sc_fxnum& operator op( tp );

#define DECL_ASN_OP(op)                                                       \
    DECL_ASN_OP_T(op,short)                                                   \
    DECL_ASN_OP_T(op,unsigned short)                                          \
    DECL_ASN_OP_T(op,int)                                                     \
    DECL_ASN_OP_T(op,unsigned int)                                            \
    DECL_ASN_OP_T(op,long)                                                    \
    DECL_ASN_OP_T(op,unsigned long)                                           \
    DECL_ASN_OP_T(op,float)                                                   \
    DECL_ASN_OP_T(op,double)                                                  \
    DECL_ASN_OP_T(op,const char*)                                             \
    DECL_ASN_OP_T(op,int64)                                                   \
    DECL_ASN_OP_T(op,uint64)                                                  \
    DECL_ASN_OP_T(op,const sc_int_base&)                                      \
    DECL_ASN_OP_T(op,const sc_uint_base&)                                     \
    DECL_ASN_OP_T(op,const sc_signed&)                                        \
    DECL_ASN_OP_T(op,const sc_unsigned&)                                      \
    DECL_ASN_OP_T(op,const sc_fxval&)                                         \
    DECL_ASN_OP_T(op,const sc_fxval_fast&)                                    \
    DECL_ASN_OP_T(op,const sc_fxnum&)                                         \
    DECL_ASN_OP_T(op,const sc_fxnum_fast&)

    DECL_ASN_OP(=)

    DECL_ASN_OP(*=)
    DECL_ASN_OP(/=)
    DECL_ASN_OP(+=)
    DECL_ASN_OP(-=)

    DECL_ASN_OP_T(<<=,int)
    DECL_ASN_OP_T(>>=,int)

#undef DECL_ASN_OP_T
#undef DECL_ASN_OP


    // auto-increment and auto-decrement

    const sc_fxval operator ++ ( int );
    const sc_fxval operator -- ( int );

    sc_fxnum& operator ++ ();
    sc_fxnum& operator -- ();


    // bit selection

    const sc_fxnum_bitref operator [] ( int ) const;
    sc_fxnum_bitref       operator [] ( int );

    const sc_fxnum_bitref bit( int ) const;
    sc_fxnum_bitref       bit( int );


    // part selection

    const sc_fxnum_subref operator () ( int, int ) const;
    sc_fxnum_subref       operator () ( int, int );

    const sc_fxnum_subref range( int, int ) const;
    sc_fxnum_subref       range( int, int );


    const sc_fxnum_subref operator () () const;
    sc_fxnum_subref       operator () ();

    const sc_fxnum_subref range() const;
    sc_fxnum_subref       range();


    // implicit conversion

    operator double() const;		// necessary evil!


    // explicit conversion to primitive types

    short          to_short() const;
    unsigned short to_ushort() const;
    int            to_int() const;
    unsigned int   to_uint() const;
    long           to_long() const;
    unsigned long  to_ulong() const;
    float          to_float() const;
    double         to_double() const;


    // explicit conversion to character string

    const sc_string to_string() const;
    const sc_string to_string( sc_numrep ) const;
    const sc_string to_string( sc_fmt ) const;
    const sc_string to_string( sc_numrep, sc_fmt ) const;

    const sc_string to_dec() const;
    const sc_string to_bin() const;
    const sc_string to_oct() const;
    const sc_string to_hex() const;


    // query value

    bool is_neg() const;
    bool is_zero() const;

    bool quantization_flag() const;
    bool overflow_flag() const;

    const sc_fxval& value() const;


    // query parameters

    int       wl() const;
    int       iwl() const;
    sc_q_mode q_mode() const;
    sc_o_mode o_mode() const;
    int       n_bits() const;

    const sc_fxtype_params& type_params() const;

    const sc_fxcast_switch& cast_switch() const;


    // print or dump content

    void print( ostream& = cout ) const;
    void dump( ostream& = cout ) const;

protected:

    bool set_bit( int, bool );

private:

    sc_fxval           _val;
    scfx_params        _params;
    bool               _q_flag;
    bool               _o_flag;
    sc_fxnum_observer* _observer;

};


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast
//
//  Base class for the fixed-point types; limited precision.
// ----------------------------------------------------------------------------

class sc_fxnum_fast
{

    friend class sc_fxnum_fast_bitref;
    friend class sc_fxnum_fast_subref;

    friend class isdb_sc_fxnum_fast_trace;
    friend class vcd_sc_fxnum_fast_trace;
    friend class wif_sc_fxnum_fast_trace;

protected:

    void cast();


    // constructors

    sc_fxnum_fast( const sc_fxtype_params&,
		   sc_enc,
		   const sc_fxcast_switch&,
		   sc_fxnum_fast_observer* );

#define DECL_CTOR_T(tp)                                                       \
    sc_fxnum_fast( tp,                                                        \
	           const sc_fxtype_params&,                                   \
	           sc_enc,                                                    \
	           const sc_fxcast_switch&,                                   \
	           sc_fxnum_fast_observer* );

    DECL_CTOR_T(short)
    DECL_CTOR_T(unsigned short)
    DECL_CTOR_T(int)
    DECL_CTOR_T(unsigned int)
    DECL_CTOR_T(long)
    DECL_CTOR_T(unsigned long)
    DECL_CTOR_T(float)
    DECL_CTOR_T(double)
    DECL_CTOR_T(const char*)
    DECL_CTOR_T(int64)
    DECL_CTOR_T(uint64)
    DECL_CTOR_T(const sc_int_base&)
    DECL_CTOR_T(const sc_uint_base&)
    DECL_CTOR_T(const sc_signed&)
    DECL_CTOR_T(const sc_unsigned&)
    DECL_CTOR_T(const sc_fxval&)
    DECL_CTOR_T(const sc_fxval_fast&)
    DECL_CTOR_T(const sc_fxnum&)
    DECL_CTOR_T(const sc_fxnum_fast&)

#undef DECL_CTOR_T

    ~sc_fxnum_fast();

public:

    // unary operators

    const sc_fxval_fast  operator - () const;
    const sc_fxval_fast& operator + () const;


    // unary functions

    friend void neg( sc_fxval_fast&, const sc_fxnum_fast& );
    friend void neg( sc_fxnum_fast&, const sc_fxnum_fast& );


    // binary operators

#define DECL_BIN_OP_T(op,tp)                                                  \
    friend const sc_fxval_fast operator op ( const sc_fxnum_fast&, tp );      \
    friend const sc_fxval_fast operator op ( tp, const sc_fxnum_fast& );

#define DECL_BIN_OP(op)                                                       \
    friend const sc_fxval_fast operator op ( const sc_fxnum_fast&,            \
					     const sc_fxnum_fast& );          \
    DECL_BIN_OP_T(op,short)                                                   \
    DECL_BIN_OP_T(op,unsigned short)                                          \
    DECL_BIN_OP_T(op,int)                                                     \
    DECL_BIN_OP_T(op,unsigned int)                                            \
    DECL_BIN_OP_T(op,long)                                                    \
    DECL_BIN_OP_T(op,unsigned long)                                           \
    DECL_BIN_OP_T(op,float)                                                   \
    DECL_BIN_OP_T(op,double)                                                  \
    DECL_BIN_OP_T(op,const char*)                                             \
    DECL_BIN_OP_T(op,int64)                                                   \
    DECL_BIN_OP_T(op,uint64)                                                  \
    DECL_BIN_OP_T(op,const sc_int_base&)                                      \
    DECL_BIN_OP_T(op,const sc_uint_base&)                                     \
    DECL_BIN_OP_T(op,const sc_signed&)                                        \
    DECL_BIN_OP_T(op,const sc_unsigned&)                                      \
    DECL_BIN_OP_T(op,const sc_fxval_fast&)

    DECL_BIN_OP(*)
    DECL_BIN_OP(+)
    DECL_BIN_OP(-)
//    DECL_BIN_OP(/) // BCB produces illegal character
    friend const sc_fxval_fast operator / ( const sc_fxnum_fast&,
					     const sc_fxnum_fast& );
    DECL_BIN_OP_T(/,short)
    DECL_BIN_OP_T(/,unsigned short)
    DECL_BIN_OP_T(/,int)
    DECL_BIN_OP_T(/,unsigned int)
    DECL_BIN_OP_T(/,long)
    DECL_BIN_OP_T(/,unsigned long)
    DECL_BIN_OP_T(/,float)
    DECL_BIN_OP_T(/,double)
    DECL_BIN_OP_T(/,const char*)
    DECL_BIN_OP_T(/,int64)
    DECL_BIN_OP_T(/,uint64)
    DECL_BIN_OP_T(/,const sc_int_base&)
    DECL_BIN_OP_T(/,const sc_uint_base&)
    DECL_BIN_OP_T(/,const sc_signed&)
    DECL_BIN_OP_T(/,const sc_unsigned&)
    DECL_BIN_OP_T(/,const sc_fxval_fast&)

#undef DECL_BIN_OP_T
#undef DECL_BIN_OP

    friend const sc_fxval_fast operator << ( const sc_fxnum_fast&, int );
    friend const sc_fxval_fast operator >> ( const sc_fxnum_fast&, int );


    // binary functions

#define DECL_BIN_FNC_T(fnc,tp)                                                \
    friend void fnc ( sc_fxval_fast&, const sc_fxnum_fast&, tp );             \
    friend void fnc ( sc_fxval_fast&, tp, const sc_fxnum_fast& );             \
    friend void fnc ( sc_fxnum_fast&, const sc_fxnum_fast&, tp );             \
    friend void fnc ( sc_fxnum_fast&, tp, const sc_fxnum_fast& );

#define DECL_BIN_FNC(fnc)                                                     \
    friend void fnc ( sc_fxval_fast&, const sc_fxnum_fast&,                   \
		                      const sc_fxnum_fast& );                 \
    friend void fnc ( sc_fxnum_fast&, const sc_fxnum_fast&,                   \
		                      const sc_fxnum_fast& );                 \
    DECL_BIN_FNC_T(fnc,short)                                                 \
    DECL_BIN_FNC_T(fnc,unsigned short)                                        \
    DECL_BIN_FNC_T(fnc,int)                                                   \
    DECL_BIN_FNC_T(fnc,unsigned int)                                          \
    DECL_BIN_FNC_T(fnc,long)                                                  \
    DECL_BIN_FNC_T(fnc,unsigned long)                                         \
    DECL_BIN_FNC_T(fnc,float)                                                 \
    DECL_BIN_FNC_T(fnc,double)                                                \
    DECL_BIN_FNC_T(fnc,const char*)                                           \
    DECL_BIN_FNC_T(fnc,int64)                                                 \
    DECL_BIN_FNC_T(fnc,uint64)                                                \
    DECL_BIN_FNC_T(fnc,const sc_int_base&)                                    \
    DECL_BIN_FNC_T(fnc,const sc_uint_base&)                                   \
    DECL_BIN_FNC_T(fnc,const sc_signed&)                                      \
    DECL_BIN_FNC_T(fnc,const sc_unsigned&)                                    \
    DECL_BIN_FNC_T(fnc,const sc_fxval&)                                       \
    DECL_BIN_FNC_T(fnc,const sc_fxval_fast&)                                  \
    DECL_BIN_FNC_T(fnc,const sc_fxnum&)

    DECL_BIN_FNC(mult)
    DECL_BIN_FNC(div)
    DECL_BIN_FNC(add)
    DECL_BIN_FNC(sub)

#undef DECL_BIN_FNC_T
#undef DECL_BIN_FNC

    friend void lshift( sc_fxval_fast&, const sc_fxnum_fast&, int );
    friend void rshift( sc_fxval_fast&, const sc_fxnum_fast&, int );
    friend void lshift( sc_fxnum_fast&, const sc_fxnum_fast&, int );
    friend void rshift( sc_fxnum_fast&, const sc_fxnum_fast&, int );


    // relational (including equality) operators

#define DECL_REL_OP_T(op,tp)                                                  \
    friend bool operator op ( const sc_fxnum_fast&, tp );                     \
    friend bool operator op ( tp, const sc_fxnum_fast& );

#define DECL_REL_OP(op)                                                       \
    friend bool operator op ( const sc_fxnum_fast&, const sc_fxnum_fast& );   \
    DECL_REL_OP_T(op,short)                                                   \
    DECL_REL_OP_T(op,unsigned short)                                          \
    DECL_REL_OP_T(op,int)                                                     \
    DECL_REL_OP_T(op,unsigned int)                                            \
    DECL_REL_OP_T(op,long)                                                    \
    DECL_REL_OP_T(op,unsigned long)                                           \
    DECL_REL_OP_T(op,float)                                                   \
    DECL_REL_OP_T(op,double)                                                  \
    DECL_REL_OP_T(op,const char*)                                             \
    DECL_REL_OP_T(op,int64)                                                   \
    DECL_REL_OP_T(op,uint64)                                                  \
    DECL_REL_OP_T(op,const sc_int_base&)                                      \
    DECL_REL_OP_T(op,const sc_uint_base&)                                     \
    DECL_REL_OP_T(op,const sc_signed&)                                        \
    DECL_REL_OP_T(op,const sc_unsigned&)                                      \
    DECL_REL_OP_T(op,const sc_fxval_fast&)

    DECL_REL_OP(<)
    DECL_REL_OP(<=)
    DECL_REL_OP(>)
    DECL_REL_OP(>=)
    DECL_REL_OP(==)
    DECL_REL_OP(!=)

#undef DECL_REL_OP_T
#undef DECL_REL_OP


    // assignment operators

#define DECL_ASN_OP_T(op,tp)                                                  \
    sc_fxnum_fast& operator op( tp );

#define DECL_ASN_OP(op)                                                       \
    DECL_ASN_OP_T(op,short)                                                   \
    DECL_ASN_OP_T(op,unsigned short)                                          \
    DECL_ASN_OP_T(op,int)                                                     \
    DECL_ASN_OP_T(op,unsigned int)                                            \
    DECL_ASN_OP_T(op,long)                                                    \
    DECL_ASN_OP_T(op,unsigned long)                                           \
    DECL_ASN_OP_T(op,float)                                                   \
    DECL_ASN_OP_T(op,double)                                                  \
    DECL_ASN_OP_T(op,const char*)                                             \
    DECL_ASN_OP_T(op,int64)                                                   \
    DECL_ASN_OP_T(op,uint64)                                                  \
    DECL_ASN_OP_T(op,const sc_int_base&)                                      \
    DECL_ASN_OP_T(op,const sc_uint_base&)                                     \
    DECL_ASN_OP_T(op,const sc_signed&)                                        \
    DECL_ASN_OP_T(op,const sc_unsigned&)                                      \
    DECL_ASN_OP_T(op,const sc_fxval&)                                         \
    DECL_ASN_OP_T(op,const sc_fxval_fast&)                                    \
    DECL_ASN_OP_T(op,const sc_fxnum&)                                         \
    DECL_ASN_OP_T(op,const sc_fxnum_fast&)

    DECL_ASN_OP(=)

    DECL_ASN_OP(*=)
    DECL_ASN_OP(/=)
    DECL_ASN_OP(+=)
    DECL_ASN_OP(-=)

    DECL_ASN_OP_T(<<=,int)
    DECL_ASN_OP_T(>>=,int)

#undef DECL_ASN_OP_T
#undef DECL_ASN_OP


    // auto-increment and auto-decrement

    const sc_fxval_fast operator ++ ( int );
    const sc_fxval_fast operator -- ( int );

    sc_fxnum_fast& operator ++ ();
    sc_fxnum_fast& operator -- ();


    // bit selection

    const sc_fxnum_fast_bitref operator [] ( int ) const;
    sc_fxnum_fast_bitref       operator [] ( int );

    const sc_fxnum_fast_bitref bit( int ) const;
    sc_fxnum_fast_bitref       bit( int );


    // part selection

    const sc_fxnum_fast_subref operator () ( int, int ) const;
    sc_fxnum_fast_subref       operator () ( int, int );

    const sc_fxnum_fast_subref range( int, int ) const;
    sc_fxnum_fast_subref       range( int, int );


    const sc_fxnum_fast_subref operator () () const;
    sc_fxnum_fast_subref       operator () ();

    const sc_fxnum_fast_subref range() const;
    sc_fxnum_fast_subref       range();


    // implicit conversion

    operator double() const;		// necessary evil!


    // explicit conversion to primitive types

    short          to_short() const;
    unsigned short to_ushort() const;
    int            to_int() const;
    unsigned int   to_uint() const;
    long           to_long() const;
    unsigned long  to_ulong() const;
    float          to_float() const;
    double         to_double() const;


    // explicit conversion to character string

    const sc_string to_string() const;
    const sc_string to_string( sc_numrep ) const;
    const sc_string to_string( sc_fmt ) const;
    const sc_string to_string( sc_numrep, sc_fmt ) const;

    const sc_string to_dec() const;
    const sc_string to_bin() const;
    const sc_string to_oct() const;
    const sc_string to_hex() const;


    // query value

    bool is_neg() const;
    bool is_zero() const;

    bool quantization_flag() const;
    bool overflow_flag() const;

    const sc_fxval_fast& value() const;


    // query parameters

    int       wl() const;
    int       iwl() const;
    sc_q_mode q_mode() const;
    sc_o_mode o_mode() const;
    int       n_bits() const;

    const sc_fxtype_params& type_params() const;

    const sc_fxcast_switch& cast_switch() const;


    // print or dump content

    void print( ostream& = cout ) const;
    void dump( ostream& = cout ) const;

protected:

    bool set_bit( int, bool );

private:

    sc_fxval_fast           _val;
    scfx_params             _params;
    bool                    _q_flag;
    bool                    _o_flag;
    sc_fxnum_fast_observer* _observer;

};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum
//
//  Base class for the fixed-point types; arbitrary precision.
// ----------------------------------------------------------------------------

inline
void
sc_fxnum::cast()
{
    _SC_ERROR_IF( _val.is_nan() || _val.is_inf(), _SC_ID_INVALID_VALUE );

    if( _params.cast_switch() == SC_ON )
        _val.cast( _params, _q_flag, _o_flag );
}


// constructors

inline
sc_fxnum::sc_fxnum( const sc_fxtype_params& type_params_,
		    sc_enc enc_,
		    const sc_fxcast_switch& cast_sw,
		    sc_fxnum_observer* observer_ )
: _val(),
  _params( type_params_, enc_, cast_sw ),
  _q_flag( false ),
  _o_flag( false ),
  _observer( observer_ )
{}

#define DEFN_CTOR_T(tp)                                                       \
inline                                                                        \
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

DEFN_CTOR_T(short)
DEFN_CTOR_T(unsigned short)
DEFN_CTOR_T(int)
DEFN_CTOR_T(unsigned int)
DEFN_CTOR_T(long)
DEFN_CTOR_T(unsigned long)
DEFN_CTOR_T(float)
DEFN_CTOR_T(double)
DEFN_CTOR_T(const char*)
DEFN_CTOR_T(int64)
DEFN_CTOR_T(uint64)
DEFN_CTOR_T(const sc_signed&)
DEFN_CTOR_T(const sc_unsigned&)
DEFN_CTOR_T(const sc_fxval&)
DEFN_CTOR_T(const sc_fxval_fast&)
DEFN_CTOR_T(const sc_fxnum&)
DEFN_CTOR_T(const sc_fxnum_fast&)

#undef DEFN_CTOR_T

inline
sc_fxnum::~sc_fxnum()
{}


// unary operators

inline
const sc_fxval
sc_fxnum::operator - () const
{
    return sc_fxval( - _val );
}

inline
const sc_fxval&
sc_fxnum::operator + () const
{
    return _val;
}


// unary functions

inline
void
neg( sc_fxval& c, const sc_fxnum& a )
{
    neg( c, a._val );
}

inline
void
neg( sc_fxnum& c, const sc_fxnum& a )
{
    neg( c._val, a._val );
    c.cast();
}


// binary operators

#define DEFN_BIN_OP_T_A(op,tp)                                                \
inline                                                                        \
const sc_fxval                                                                \
operator op ( const sc_fxnum& a, tp b )                                       \
{                                                                             \
    return sc_fxval( a._val op b );                                           \
}                                                                             \
                                                                              \
inline                                                                        \
const sc_fxval                                                                \
operator op ( tp a, const sc_fxnum& b )                                       \
{                                                                             \
    return sc_fxval( a op b._val );                                           \
}

#define DEFN_BIN_OP_T_B(op,tp)                                                \
inline                                                                        \
const sc_fxval                                                                \
operator op ( const sc_fxnum& a, tp b )                                       \
{                                                                             \
    return sc_fxval( a._val op b.value() );                                   \
}                                                                             \
                                                                              \
inline                                                                        \
const sc_fxval                                                                \
operator op ( tp a, const sc_fxnum& b )                                       \
{                                                                             \
    return sc_fxval( a.value() op b._val );                                   \
}

#define DEFN_BIN_OP(op)                                                       \
inline                                                                        \
const sc_fxval                                                                \
operator op ( const sc_fxnum& a, const sc_fxnum& b )                          \
{                                                                             \
    return sc_fxval( a._val op b._val );                                      \
}                                                                             \
                                                                              \
DEFN_BIN_OP_T_A(op,short)                                                     \
DEFN_BIN_OP_T_A(op,unsigned short)                                            \
DEFN_BIN_OP_T_A(op,int)                                                       \
DEFN_BIN_OP_T_A(op,unsigned int)                                              \
DEFN_BIN_OP_T_A(op,long)                                                      \
DEFN_BIN_OP_T_A(op,unsigned long)                                             \
DEFN_BIN_OP_T_A(op,float)                                                     \
DEFN_BIN_OP_T_A(op,double)                                                    \
DEFN_BIN_OP_T_A(op,const char*)                                               \
DEFN_BIN_OP_T_A(op,int64)                                                     \
DEFN_BIN_OP_T_A(op,uint64)                                                    \
DEFN_BIN_OP_T_A(op,const sc_signed&)                                          \
DEFN_BIN_OP_T_A(op,const sc_unsigned&)                                        \
DEFN_BIN_OP_T_A(op,const sc_fxval&)                                           \
DEFN_BIN_OP_T_A(op,const sc_fxval_fast&)                                      \
DEFN_BIN_OP_T_B(op,const sc_fxnum_fast&)

DEFN_BIN_OP(*)
DEFN_BIN_OP(+)
DEFN_BIN_OP(-)
// DEFN_BIN_OP(/) // BCB produces illegal character
inline
const sc_fxval
operator / ( const sc_fxnum& a, const sc_fxnum& b )
{
    return sc_fxval( a._val / b._val );
}

DEFN_BIN_OP_T_A(/,short)
DEFN_BIN_OP_T_A(/,unsigned short)
DEFN_BIN_OP_T_A(/,int)
DEFN_BIN_OP_T_A(/,unsigned int)
DEFN_BIN_OP_T_A(/,long)
DEFN_BIN_OP_T_A(/,unsigned long)
DEFN_BIN_OP_T_A(/,float)
DEFN_BIN_OP_T_A(/,double)
DEFN_BIN_OP_T_A(/,const char*)
DEFN_BIN_OP_T_A(/,int64)
DEFN_BIN_OP_T_A(/,uint64)
DEFN_BIN_OP_T_A(/,const sc_signed&)
DEFN_BIN_OP_T_A(/,const sc_unsigned&)
DEFN_BIN_OP_T_A(/,const sc_fxval&)
DEFN_BIN_OP_T_A(/,const sc_fxval_fast&)
DEFN_BIN_OP_T_B(/,const sc_fxnum_fast&)

#undef DEFN_BIN_OP_T_A
#undef DEFN_BIN_OP_T_B
#undef DEFN_BIN_OP


inline
const sc_fxval
operator << ( const sc_fxnum& a, int b )
{
    return sc_fxval( a._val << b );
}

inline
const sc_fxval
operator >> ( const sc_fxnum& a, int b )
{
    return sc_fxval( a._val >> b );
}


// binary functions

#define DEFN_BIN_FNC_T_A(fnc,tp)                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval& c, const sc_fxnum& a, tp b )                                  \
{                                                                             \
    fnc( c, a._val, b );                                                      \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval& c, tp a, const sc_fxnum& b )                                  \
{                                                                             \
    fnc( c, a, b._val );                                                      \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxnum& c, const sc_fxnum& a, tp b )                                  \
{                                                                             \
    fnc( c._val, a._val, b );                                                 \
    c.cast();                                                                 \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxnum& c, tp a, const sc_fxnum& b )                                  \
{                                                                             \
    fnc( c._val, a, b._val );                                                 \
    c.cast();                                                                 \
}

#define DEFN_BIN_FNC_T_B(fnc,tp)                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval& c, const sc_fxnum& a, tp b )                                  \
{                                                                             \
    fnc( c, a._val, b.value() );                                              \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval& c, tp a, const sc_fxnum& b )                                  \
{                                                                             \
    fnc( c, a.value(), b._val );                                              \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxnum& c, const sc_fxnum& a, tp b )                                  \
{                                                                             \
    fnc( c._val, a._val, b.value() );                                         \
    c.cast();                                                                 \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxnum& c, tp a, const sc_fxnum& b )                                  \
{                                                                             \
    fnc( c._val, a.value(), b._val );                                         \
    c.cast();                                                                 \
}

#define DEFN_BIN_FNC(fnc)                                                     \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval& c, const sc_fxnum& a, const sc_fxnum& b )                     \
{                                                                             \
    fnc( c, a._val, b._val );                                                 \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxnum& c, const sc_fxnum& a, const sc_fxnum& b )                     \
{                                                                             \
    fnc( c._val, a._val, b._val );                                            \
    c.cast();                                                                 \
}                                                                             \
                                                                              \
DEFN_BIN_FNC_T_A(fnc,short)                                                   \
DEFN_BIN_FNC_T_A(fnc,unsigned short)                                          \
DEFN_BIN_FNC_T_A(fnc,int)                                                     \
DEFN_BIN_FNC_T_A(fnc,unsigned int)                                            \
DEFN_BIN_FNC_T_A(fnc,long)                                                    \
DEFN_BIN_FNC_T_A(fnc,unsigned long)                                           \
DEFN_BIN_FNC_T_A(fnc,float)                                                   \
DEFN_BIN_FNC_T_A(fnc,double)                                                  \
DEFN_BIN_FNC_T_A(fnc,const char*)                                             \
DEFN_BIN_FNC_T_A(fnc,int64)                                                   \
DEFN_BIN_FNC_T_A(fnc,uint64)                                                  \
DEFN_BIN_FNC_T_A(fnc,const sc_signed&)                                        \
DEFN_BIN_FNC_T_A(fnc,const sc_unsigned&)                                      \
DEFN_BIN_FNC_T_A(fnc,const sc_fxval&)                                         \
DEFN_BIN_FNC_T_A(fnc,const sc_fxval_fast&)                                    \
DEFN_BIN_FNC_T_B(fnc,const sc_fxnum_fast&)

DEFN_BIN_FNC(mult)
DEFN_BIN_FNC(div)
DEFN_BIN_FNC(add)
DEFN_BIN_FNC(sub)

#undef DEFN_BIN_FNC_T_A
#undef DEFN_BIN_FNC_T_B
#undef DEFN_BIN_FNC


inline
void
lshift( sc_fxval& c, const sc_fxnum& a, int b )
{
    lshift( c, a._val, b );
}

inline
void
rshift( sc_fxval& c, const sc_fxnum& a, int b )
{
    rshift( c, a._val, b );
}

inline
void
lshift( sc_fxnum& c, const sc_fxnum& a, int b )
{
    lshift( c._val, a._val, b );
    c.cast();
}

inline
void
rshift( sc_fxnum& c, const sc_fxnum& a, int b )
{
    rshift( c._val, a._val, b );
    c.cast();
}


// relational (including equality) operators

#define DEFN_REL_OP_T_A(op,tp)                                                \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxnum& a, tp b )                                       \
{                                                                             \
    return ( a._val op b );                                                   \
}                                                                             \
                                                                              \
inline                                                                        \
bool                                                                          \
operator op ( tp a, const sc_fxnum& b )                                       \
{                                                                             \
    return ( a op b._val );                                                   \
}

#define DEFN_REL_OP_T_B(op,tp)                                                \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxnum& a, tp b )                                       \
{                                                                             \
    return ( a._val op b.value() );                                           \
}                                                                             \
                                                                              \
inline                                                                        \
bool                                                                          \
operator op ( tp a, const sc_fxnum& b )                                       \
{                                                                             \
    return ( a.value() op b._val );                                           \
}

#define DEFN_REL_OP(op)                                                       \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxnum& a, const sc_fxnum& b )                          \
{                                                                             \
    return ( a._val op b._val );                                              \
}                                                                             \
                                                                              \
DEFN_REL_OP_T_A(op,short)                                                     \
DEFN_REL_OP_T_A(op,unsigned short)                                            \
DEFN_REL_OP_T_A(op,int)                                                       \
DEFN_REL_OP_T_A(op,unsigned int)                                              \
DEFN_REL_OP_T_A(op,long)                                                      \
DEFN_REL_OP_T_A(op,unsigned long)                                             \
DEFN_REL_OP_T_A(op,float)                                                     \
DEFN_REL_OP_T_A(op,double)                                                    \
DEFN_REL_OP_T_A(op,const char*)                                               \
DEFN_REL_OP_T_A(op,int64)                                                     \
DEFN_REL_OP_T_A(op,uint64)                                                    \
DEFN_REL_OP_T_A(op,const sc_signed&)                                          \
DEFN_REL_OP_T_A(op,const sc_unsigned&)                                        \
DEFN_REL_OP_T_A(op,const sc_fxval&)                                           \
DEFN_REL_OP_T_A(op,const sc_fxval_fast&)                                      \
DEFN_REL_OP_T_B(op,const sc_fxnum_fast&)

DEFN_REL_OP(<)
DEFN_REL_OP(<=)
DEFN_REL_OP(>)
DEFN_REL_OP(>=)
DEFN_REL_OP(==)
DEFN_REL_OP(!=)

#undef DEFN_REL_OP_T_A
#undef DEFN_REL_OP_T_B
#undef DEFN_REL_OP


// assignment operators

#define DEFN_ASN_OP_T_A(op,tp)                                                \
inline                                                                        \
sc_fxnum&                                                                     \
sc_fxnum::operator op ( tp b )                                                \
{                                                                             \
    _val op b;                                                                \
    cast();                                                                   \
    return *this;                                                             \
}

#define DEFN_ASN_OP_T_B(op,tp)                                                \
inline                                                                        \
sc_fxnum&                                                                     \
sc_fxnum::operator op ( tp b )                                                \
{                                                                             \
    _val op b.value();                                                        \
    cast();                                                                   \
    return *this;                                                             \
}

#define DEFN_ASN_OP(op)                                                       \
DEFN_ASN_OP_T_A(op,short)                                                     \
DEFN_ASN_OP_T_A(op,unsigned short)                                            \
DEFN_ASN_OP_T_A(op,int)                                                       \
DEFN_ASN_OP_T_A(op,unsigned int)                                              \
DEFN_ASN_OP_T_A(op,long)                                                      \
DEFN_ASN_OP_T_A(op,unsigned long)                                             \
DEFN_ASN_OP_T_A(op,float)                                                     \
DEFN_ASN_OP_T_A(op,double)                                                    \
DEFN_ASN_OP_T_A(op,const char*)                                               \
DEFN_ASN_OP_T_A(op,int64)                                                     \
DEFN_ASN_OP_T_A(op,uint64)                                                    \
DEFN_ASN_OP_T_A(op,const sc_signed&)                                          \
DEFN_ASN_OP_T_A(op,const sc_unsigned&)                                        \
DEFN_ASN_OP_T_A(op,const sc_fxval&)                                           \
DEFN_ASN_OP_T_A(op,const sc_fxval_fast&)                                      \
DEFN_ASN_OP_T_B(op,const sc_fxnum&)                                           \
DEFN_ASN_OP_T_B(op,const sc_fxnum_fast&)

DEFN_ASN_OP(=)

DEFN_ASN_OP(*=)
DEFN_ASN_OP(/=)
DEFN_ASN_OP(+=)
DEFN_ASN_OP(-=)

DEFN_ASN_OP_T_A(<<=,int)
DEFN_ASN_OP_T_A(>>=,int)

#undef DEFN_ASN_OP_T_A
#undef DEFN_ASN_OP_T_B
#undef DEFN_ASN_OP


// auto-increment and auto-decrement

inline
const sc_fxval
sc_fxnum::operator ++ ( int )
{
    sc_fxval c = _val ++;
    cast();
    return sc_fxval( c );
}

inline
const sc_fxval
sc_fxnum::operator -- ( int )
{
    sc_fxval c = _val --;
    cast();
    return sc_fxval( c );
}

inline
sc_fxnum&
sc_fxnum::operator ++ ()
{
    ++ _val;
    cast();
    return *this;
}

inline
sc_fxnum&
sc_fxnum::operator -- ()
{
    -- _val;
    cast();
    return *this;
}


// bit selection

inline
const sc_fxnum_bitref
sc_fxnum::operator [] ( int i ) const
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    return sc_fxnum_bitref( const_cast<sc_fxnum&>( *this ),
			    i - _params.fwl() );
}

inline
sc_fxnum_bitref
sc_fxnum::operator [] ( int i )
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    return sc_fxnum_bitref( *this, i - _params.fwl() );
}

inline
const sc_fxnum_bitref
sc_fxnum::bit( int i ) const
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    return sc_fxnum_bitref( const_cast<sc_fxnum&>( *this ),
			    i - _params.fwl() );
}

inline
sc_fxnum_bitref
sc_fxnum::bit( int i )
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    return sc_fxnum_bitref( *this, i - _params.fwl() );
}


// part selection

inline
const sc_fxnum_subref
sc_fxnum::operator () ( int i, int j ) const
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    _SC_ERROR_IF( j < 0 || j >= _params.wl(), _SC_ID_OUT_OF_RANGE );

    return sc_fxnum_subref( const_cast<sc_fxnum&>( *this ),
			    i - _params.fwl(), j - _params.fwl() );
}

inline
sc_fxnum_subref
sc_fxnum::operator () ( int i, int j )
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    _SC_ERROR_IF( j < 0 || j >= _params.wl(), _SC_ID_OUT_OF_RANGE );

    return sc_fxnum_subref( *this, i - _params.fwl(), j - _params.fwl() );
}

inline
const sc_fxnum_subref
sc_fxnum::range( int i, int j ) const
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    _SC_ERROR_IF( j < 0 || j >= _params.wl(), _SC_ID_OUT_OF_RANGE );

    return sc_fxnum_subref( const_cast<sc_fxnum&>( *this ),
			    i - _params.fwl(), j - _params.fwl() );
}

inline
sc_fxnum_subref
sc_fxnum::range( int i, int j )
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    _SC_ERROR_IF( j < 0 || j >= _params.wl(), _SC_ID_OUT_OF_RANGE );

    return sc_fxnum_subref( *this, i - _params.fwl(), j - _params.fwl() );
}


inline
const sc_fxnum_subref
sc_fxnum::operator () () const
{
    return this->operator () ( _params.wl() - 1, 0 );
}

inline
sc_fxnum_subref
sc_fxnum::operator () ()
{
    return this->operator () ( _params.wl() - 1, 0 );
}

inline
const sc_fxnum_subref
sc_fxnum::range() const
{
    return this->range( _params.wl() - 1, 0 );
}

inline
sc_fxnum_subref
sc_fxnum::range()
{
    return this->range( _params.wl() - 1, 0 );
}


// implicit conversion

inline
sc_fxnum::operator double() const
{
    return _val.to_double();
}


// explicit conversion to primitive types

inline
short
sc_fxnum::to_short() const
{
    return _val.to_short();
}

inline
unsigned short
sc_fxnum::to_ushort() const
{
    return _val.to_ushort();
}

inline
int
sc_fxnum::to_int() const
{
    return _val.to_int();
}

inline
unsigned int
sc_fxnum::to_uint() const
{
    return _val.to_uint();
}

inline
long
sc_fxnum::to_long() const
{
    return _val.to_long();
}

inline
unsigned long
sc_fxnum::to_ulong() const
{
    return _val.to_ulong();
}

inline
float
sc_fxnum::to_float() const
{
    return _val.to_float();
}

inline
double
sc_fxnum::to_double() const
{
    return _val.to_double();
}


// query value

inline
bool
sc_fxnum::is_neg() const
{
    return _val.is_neg();
}

inline
bool
sc_fxnum::is_zero() const
{
    return _val.is_zero();
}


inline
bool
sc_fxnum::quantization_flag() const
{
    return _q_flag;
}

inline
bool
sc_fxnum::overflow_flag() const
{
    return _o_flag;
}


inline
const sc_fxval&
sc_fxnum::value() const
{
    return _val;
}


// query parameters

inline
int
sc_fxnum::wl() const
{
    return _params.wl();
}

inline
int
sc_fxnum::iwl() const
{
    return _params.iwl();
}

inline
sc_q_mode
sc_fxnum::q_mode() const
{
    return _params.q_mode();
}

inline
sc_o_mode
sc_fxnum::o_mode() const
{
    return _params.o_mode();
}

inline
int
sc_fxnum::n_bits() const
{
    return _params.n_bits();
}


inline
const sc_fxtype_params&
sc_fxnum::type_params() const
{
    return _params.type_params();
}


inline
const sc_fxcast_switch&
sc_fxnum::cast_switch() const
{
    return _params.cast_switch();
}


inline
bool
sc_fxnum::set_bit( int i, bool high )
{
    return _val.set_bit( i, _params, high );
}


inline
ostream&
operator << ( ostream& os, const sc_fxnum& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast
//
//  Base class for the fixed-point types; limited precision.
// ----------------------------------------------------------------------------

inline
void
sc_fxnum_fast::cast()
{
    _SC_ERROR_IF( _val.is_nan() || _val.is_inf(), _SC_ID_INVALID_VALUE );

    if( _params.cast_switch() == SC_ON )
        _val.cast( _params, _q_flag, _o_flag );
}


// constructors

inline
sc_fxnum_fast::sc_fxnum_fast( const sc_fxtype_params& type_params_,
			      sc_enc enc_,
			      const sc_fxcast_switch& cast_sw,
			      sc_fxnum_fast_observer* observer_ )
: _val(),
  _params( type_params_, enc_, cast_sw ),
  _q_flag( false ),
  _o_flag( false ),
  _observer( observer_ )
{}

#define DEFN_CTOR_T(tp)                                                       \
inline                                                                        \
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

DEFN_CTOR_T(short)
DEFN_CTOR_T(unsigned short)
DEFN_CTOR_T(int)
DEFN_CTOR_T(unsigned int)
DEFN_CTOR_T(long)
DEFN_CTOR_T(unsigned long)
DEFN_CTOR_T(float)
DEFN_CTOR_T(double)
DEFN_CTOR_T(const char*)
DEFN_CTOR_T(int64)
DEFN_CTOR_T(uint64)
DEFN_CTOR_T(const sc_signed&)
DEFN_CTOR_T(const sc_unsigned&)
DEFN_CTOR_T(const sc_fxval&)
DEFN_CTOR_T(const sc_fxval_fast&)
DEFN_CTOR_T(const sc_fxnum&)
DEFN_CTOR_T(const sc_fxnum_fast&)

#undef DEFN_CTOR_T

inline
sc_fxnum_fast::~sc_fxnum_fast()
{}


// unary operators

inline
const sc_fxval_fast
sc_fxnum_fast::operator - () const
{
    return sc_fxval_fast( - _val );
}

inline
const sc_fxval_fast&
sc_fxnum_fast::operator + () const
{
    return _val;
}


// unary functions

inline
void
neg( sc_fxval_fast& c, const sc_fxnum_fast& a )
{
    neg( c, a._val );
}

inline
void
neg( sc_fxnum_fast& c, const sc_fxnum_fast& a )
{
    neg( c._val, a._val );
    c.cast();
}


// binary operators

#define DEFN_BIN_OP_T(op,tp)                                                  \
inline                                                                        \
const sc_fxval_fast                                                           \
operator op ( const sc_fxnum_fast& a, tp b )                                  \
{                                                                             \
    return sc_fxval_fast( a._val op b );                                      \
}                                                                             \
                                                                              \
inline                                                                        \
const sc_fxval_fast                                                           \
operator op ( tp a, const sc_fxnum_fast& b )                                  \
{                                                                             \
    return sc_fxval_fast( a op b._val );                                      \
}

#define DEFN_BIN_OP(op)                                                       \
inline                                                                        \
const sc_fxval_fast                                                           \
operator op ( const sc_fxnum_fast& a, const sc_fxnum_fast& b )                \
{                                                                             \
    return sc_fxval_fast( a._val op b._val );                                 \
}                                                                             \
                                                                              \
DEFN_BIN_OP_T(op,short)                                                       \
DEFN_BIN_OP_T(op,unsigned short)                                              \
DEFN_BIN_OP_T(op,int)                                                         \
DEFN_BIN_OP_T(op,unsigned int)                                                \
DEFN_BIN_OP_T(op,long)                                                        \
DEFN_BIN_OP_T(op,unsigned long)                                               \
DEFN_BIN_OP_T(op,float)                                                       \
DEFN_BIN_OP_T(op,double)                                                      \
DEFN_BIN_OP_T(op,const char*)                                                 \
DEFN_BIN_OP_T(op,int64)                                                       \
DEFN_BIN_OP_T(op,uint64)                                                      \
DEFN_BIN_OP_T(op,const sc_signed&)                                            \
DEFN_BIN_OP_T(op,const sc_unsigned&)                                          \
DEFN_BIN_OP_T(op,const sc_fxval_fast&)

DEFN_BIN_OP(*)
DEFN_BIN_OP(+)
DEFN_BIN_OP(-)
// DEFN_BIN_OP(/) // BCB produces illegal character
inline
const sc_fxval_fast
operator / ( const sc_fxnum_fast& a, const sc_fxnum_fast& b )
{
    return sc_fxval_fast( a._val / b._val );
}

DEFN_BIN_OP_T(/,short)
DEFN_BIN_OP_T(/,unsigned short)
DEFN_BIN_OP_T(/,int)
DEFN_BIN_OP_T(/,unsigned int)
DEFN_BIN_OP_T(/,long)
DEFN_BIN_OP_T(/,unsigned long)
DEFN_BIN_OP_T(/,float)
DEFN_BIN_OP_T(/,double)
DEFN_BIN_OP_T(/,const char*)
DEFN_BIN_OP_T(/,int64)
DEFN_BIN_OP_T(/,uint64)
DEFN_BIN_OP_T(/,const sc_signed&)
DEFN_BIN_OP_T(/,const sc_unsigned&)
DEFN_BIN_OP_T(/,const sc_fxval_fast&)

#undef DEFN_BIN_OP_T
#undef DEFN_BIN_OP


inline
const sc_fxval_fast
operator << ( const sc_fxnum_fast& a, int b )
{
    return sc_fxval_fast( a._val << b );
}

inline
const sc_fxval_fast
operator >> ( const sc_fxnum_fast& a, int b )
{
    return sc_fxval_fast( a._val >> b );
}


// binary functions

#define DEFN_BIN_FNC_T(fnc,tp)                                                \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval_fast& c, const sc_fxnum_fast& a, tp b )                        \
{                                                                             \
    fnc( c, a._val, b );                                                      \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval_fast& c, tp a, const sc_fxnum_fast& b )                        \
{                                                                             \
    fnc( c, a, b._val );                                                      \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxnum_fast& c, const sc_fxnum_fast& a, tp b )                        \
{                                                                             \
    fnc( c._val, a._val, b );                                                 \
    c.cast();                                                                 \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxnum_fast& c, tp a, const sc_fxnum_fast& b )                        \
{                                                                             \
    fnc( c._val, a, b._val );                                                 \
    c.cast();                                                                 \
}

#define DEFN_BIN_FNC(fnc)                                                     \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval_fast& c, const sc_fxnum_fast& a, const sc_fxnum_fast& b )      \
{                                                                             \
    fnc( c, a._val, b._val );                                                 \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxnum_fast& c, const sc_fxnum_fast& a, const sc_fxnum_fast& b )      \
{                                                                             \
    fnc( c._val, a._val, b._val );                                            \
    c.cast();                                                                 \
}                                                                             \
                                                                              \
DEFN_BIN_FNC_T(fnc,short)                                                     \
DEFN_BIN_FNC_T(fnc,unsigned short)                                            \
DEFN_BIN_FNC_T(fnc,int)                                                       \
DEFN_BIN_FNC_T(fnc,unsigned int)                                              \
DEFN_BIN_FNC_T(fnc,long)                                                      \
DEFN_BIN_FNC_T(fnc,unsigned long)                                             \
DEFN_BIN_FNC_T(fnc,float)                                                     \
DEFN_BIN_FNC_T(fnc,double)                                                    \
DEFN_BIN_FNC_T(fnc,const char*)                                               \
DEFN_BIN_FNC_T(fnc,int64)                                                     \
DEFN_BIN_FNC_T(fnc,uint64)                                                    \
DEFN_BIN_FNC_T(fnc,const sc_signed&)                                          \
DEFN_BIN_FNC_T(fnc,const sc_unsigned&)                                        \
DEFN_BIN_FNC_T(fnc,const sc_fxval&)                                           \
DEFN_BIN_FNC_T(fnc,const sc_fxval_fast&)                                      \
DEFN_BIN_FNC_T(fnc,const sc_fxnum&)

DEFN_BIN_FNC(mult)
DEFN_BIN_FNC(div)
DEFN_BIN_FNC(add)
DEFN_BIN_FNC(sub)

#undef DEFN_BIN_FNC_T
#undef DEFN_BIN_FNC


inline
void
lshift( sc_fxval_fast& c, const sc_fxnum_fast& a, int b )
{
    lshift( c, a._val, b );
}

inline
void
rshift( sc_fxval_fast& c, const sc_fxnum_fast& a, int b )
{
    rshift( c, a._val, b );
}

inline
void
lshift( sc_fxnum_fast& c, const sc_fxnum_fast& a, int b )
{
    lshift( c._val, a._val, b );
    c.cast();
}

inline
void
rshift( sc_fxnum_fast& c, const sc_fxnum_fast& a, int b )
{
    rshift( c._val, a._val, b );
    c.cast();
}


// relational (including equality) operators

#define DEFN_REL_OP_T(op,tp)                                                \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxnum_fast& a, tp b )                                  \
{                                                                             \
    return ( a._val op b );                                                   \
}                                                                             \
                                                                              \
inline                                                                        \
bool                                                                          \
operator op ( tp a, const sc_fxnum_fast& b )                                  \
{                                                                             \
    return ( a op b._val );                                                   \
}

#define DEFN_REL_OP(op)                                                       \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxnum_fast& a, const sc_fxnum_fast& b )                \
{                                                                             \
    return ( a._val op b._val );                                              \
}                                                                             \
                                                                              \
DEFN_REL_OP_T(op,short)                                                       \
DEFN_REL_OP_T(op,unsigned short)                                              \
DEFN_REL_OP_T(op,int)                                                         \
DEFN_REL_OP_T(op,unsigned int)                                                \
DEFN_REL_OP_T(op,long)                                                        \
DEFN_REL_OP_T(op,unsigned long)                                               \
DEFN_REL_OP_T(op,float)                                                       \
DEFN_REL_OP_T(op,double)                                                      \
DEFN_REL_OP_T(op,const char*)                                                 \
DEFN_REL_OP_T(op,int64)                                                       \
DEFN_REL_OP_T(op,uint64)                                                      \
DEFN_REL_OP_T(op,const sc_signed&)                                            \
DEFN_REL_OP_T(op,const sc_unsigned&)                                          \
DEFN_REL_OP_T(op,const sc_fxval_fast&)

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
inline                                                                        \
sc_fxnum_fast&                                                                \
sc_fxnum_fast::operator op ( tp b )                                           \
{                                                                             \
    _val op b;                                                                \
    cast();                                                                   \
    return *this;                                                             \
}

#define DEFN_ASN_OP_T_B(op,tp)                                                \
inline                                                                        \
sc_fxnum_fast&                                                                \
sc_fxnum_fast::operator op ( tp b )                                           \
{                                                                             \
    _val op b.value();                                                        \
    cast();                                                                   \
    return *this;                                                             \
}

#define DEFN_ASN_OP(op)                                                       \
DEFN_ASN_OP_T_A(op,short)                                                     \
DEFN_ASN_OP_T_A(op,unsigned short)                                            \
DEFN_ASN_OP_T_A(op,int)                                                       \
DEFN_ASN_OP_T_A(op,unsigned int)                                              \
DEFN_ASN_OP_T_A(op,long)                                                      \
DEFN_ASN_OP_T_A(op,unsigned long)                                             \
DEFN_ASN_OP_T_A(op,float)                                                     \
DEFN_ASN_OP_T_A(op,double)                                                    \
DEFN_ASN_OP_T_A(op,const char*)                                               \
DEFN_ASN_OP_T_A(op,int64)                                                     \
DEFN_ASN_OP_T_A(op,uint64)                                                    \
DEFN_ASN_OP_T_A(op,const sc_signed&)                                          \
DEFN_ASN_OP_T_A(op,const sc_unsigned&)                                        \
DEFN_ASN_OP_T_A(op,const sc_fxval&)                                           \
DEFN_ASN_OP_T_A(op,const sc_fxval_fast&)                                      \
DEFN_ASN_OP_T_B(op,const sc_fxnum&)                                           \
DEFN_ASN_OP_T_B(op,const sc_fxnum_fast&)

DEFN_ASN_OP(=)

DEFN_ASN_OP(*=)
DEFN_ASN_OP(/=)
DEFN_ASN_OP(+=)
DEFN_ASN_OP(-=)

DEFN_ASN_OP_T_A(<<=,int)
DEFN_ASN_OP_T_A(>>=,int)

#undef DEFN_ASN_OP_T_A
#undef DEFN_ASN_OP_T_B
#undef DEFN_ASN_OP


// auto-increment and auto-decrement

inline
const sc_fxval_fast
sc_fxnum_fast::operator ++ ( int )
{
    sc_fxval_fast c = _val ++;
    cast();
    return sc_fxval_fast( c );
}

inline
const sc_fxval_fast
sc_fxnum_fast::operator -- ( int )
{
    sc_fxval_fast c = _val --;
    cast();
    return sc_fxval_fast( c );
}

inline
sc_fxnum_fast&
sc_fxnum_fast::operator ++ ()
{
    ++ _val;
    cast();
    return *this;
}

inline
sc_fxnum_fast&
sc_fxnum_fast::operator -- ()
{
    -- _val;
    cast();
    return *this;
}


// bit selection

inline
const sc_fxnum_fast_bitref
sc_fxnum_fast::operator [] ( int i ) const
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    return sc_fxnum_fast_bitref( const_cast<sc_fxnum_fast&>( *this ),
				 i - _params.fwl() );
}

inline
sc_fxnum_fast_bitref
sc_fxnum_fast::operator [] ( int i )
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    return sc_fxnum_fast_bitref( *this, i - _params.fwl() );
}

inline
const sc_fxnum_fast_bitref
sc_fxnum_fast::bit( int i ) const
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    return sc_fxnum_fast_bitref( const_cast<sc_fxnum_fast&>( *this ),
				 i - _params.fwl() );
}

inline
sc_fxnum_fast_bitref
sc_fxnum_fast::bit( int i )
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    return sc_fxnum_fast_bitref( *this, i - _params.fwl() );
}


// part selection

inline
const sc_fxnum_fast_subref
sc_fxnum_fast::operator () ( int i, int j ) const
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    _SC_ERROR_IF( j < 0 || j >= _params.wl(), _SC_ID_OUT_OF_RANGE );

    return sc_fxnum_fast_subref( const_cast<sc_fxnum_fast&>( *this ),
				 i - _params.fwl(), j - _params.fwl() );
}

inline
sc_fxnum_fast_subref
sc_fxnum_fast::operator () ( int i, int j )
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    _SC_ERROR_IF( j < 0 || j >= _params.wl(), _SC_ID_OUT_OF_RANGE );

    return sc_fxnum_fast_subref( *this, i - _params.fwl(), j - _params.fwl() );
}

inline
const sc_fxnum_fast_subref
sc_fxnum_fast::range( int i, int j ) const
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    _SC_ERROR_IF( j < 0 || j >= _params.wl(), _SC_ID_OUT_OF_RANGE );

    return sc_fxnum_fast_subref( const_cast<sc_fxnum_fast&>( *this ),
				 i - _params.fwl(), j - _params.fwl() );
}

inline
sc_fxnum_fast_subref
sc_fxnum_fast::range( int i, int j )
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    _SC_ERROR_IF( j < 0 || j >= _params.wl(), _SC_ID_OUT_OF_RANGE );

    return sc_fxnum_fast_subref( *this, i - _params.fwl(), j - _params.fwl() );
}

inline
const sc_fxnum_fast_subref
sc_fxnum_fast::operator () () const
{
    return this->operator () ( _params.wl() - 1, 0 );
}

inline
sc_fxnum_fast_subref
sc_fxnum_fast::operator () ()
{
    return this->operator () ( _params.wl() - 1, 0 );
}

inline
const sc_fxnum_fast_subref
sc_fxnum_fast::range() const
{
    return this->range( _params.wl() - 1, 0 );
}

inline
sc_fxnum_fast_subref
sc_fxnum_fast::range()
{
    return this->range( _params.wl() - 1, 0 );
}


// implicit conversion

inline
sc_fxnum_fast::operator double() const
{
    return _val.to_double();
}


// explicit conversion to primitive types

inline
short
sc_fxnum_fast::to_short() const
{
    return _val.to_short();
}

inline
unsigned short
sc_fxnum_fast::to_ushort() const
{
    return _val.to_ushort();
}

inline
int
sc_fxnum_fast::to_int() const
{
    return _val.to_int();
}

inline
unsigned int
sc_fxnum_fast::to_uint() const
{
    return _val.to_uint();
}

inline
long
sc_fxnum_fast::to_long() const
{
    return _val.to_long();
}

inline
unsigned long
sc_fxnum_fast::to_ulong() const
{
    return _val.to_ulong();
}

inline
float
sc_fxnum_fast::to_float() const
{
    return _val.to_float();
}

inline
double
sc_fxnum_fast::to_double() const
{
    return _val.to_double();
}


// query value

inline
bool
sc_fxnum_fast::is_neg() const
{
    return _val.is_neg();
}

inline
bool
sc_fxnum_fast::is_zero() const
{
    return _val.is_zero();
}


inline
bool
sc_fxnum_fast::quantization_flag() const
{
    return _q_flag;
}

inline
bool
sc_fxnum_fast::overflow_flag() const
{
    return _o_flag;
}


inline
const sc_fxval_fast&
sc_fxnum_fast::value() const
{
    return _val;
}


// query parameters

inline
int
sc_fxnum_fast::wl() const
{
    return _params.wl();
}

inline
int
sc_fxnum_fast::iwl() const
{
    return _params.iwl();
}

inline
sc_q_mode
sc_fxnum_fast::q_mode() const
{
    return _params.q_mode();
}

inline
sc_o_mode
sc_fxnum_fast::o_mode() const
{
    return _params.o_mode();
}

inline
int
sc_fxnum_fast::n_bits() const
{
    return _params.n_bits();
}


inline
const sc_fxtype_params&
sc_fxnum_fast::type_params() const
{
    return _params.type_params();
}


inline
const sc_fxcast_switch&
sc_fxnum_fast::cast_switch() const
{
    return _params.cast_switch();
}


inline
bool
sc_fxnum_fast::set_bit( int i, bool high )
{
    return _val.set_bit( i, _params, high );
}


inline
ostream&
operator << ( ostream& os, const sc_fxnum_fast& a )
{
    a.print( os );
    return os;
}


#endif

// Taf!
