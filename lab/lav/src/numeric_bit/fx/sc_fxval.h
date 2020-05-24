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

    sc_fxval.h - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_FXVAL_H
#define SC_FXVAL_H


#ifndef   SCFX_REP_H
#include "scfx_rep.h"
#endif

#ifndef   SC_SIGNED_H
#include "numeric_bit/sc_signed.h"
#endif

#ifndef   SC_UNSIGNED_H
#include "numeric_bit/sc_unsigned.h"
#endif


class sc_fxval_fast;
class sc_fxnum;
class sc_fxnum_fast;

class sc_int_base;
class sc_uint_base;


// ----------------------------------------------------------------------------
//  CLASS : sc_fxval
//
//  Fixed-point value type; arbitrary precision.
// ----------------------------------------------------------------------------

class sc_fxval
{

    friend class sc_fxnum;
    friend class sc_fxnum_bitref;
    friend class sc_fxnum_subref;

    sc_fxval( scfx_rep* );

public:

    sc_fxval();
    sc_fxval( short );
    sc_fxval( unsigned short );
    sc_fxval( int );
    sc_fxval( unsigned int );
    sc_fxval( long );
    sc_fxval( unsigned long );
    sc_fxval( float );
    sc_fxval( double );
    sc_fxval( const char* );
    sc_fxval( int64 );
    sc_fxval( uint64 );
    sc_fxval( const sc_int_base& );
    sc_fxval( const sc_uint_base& );
    sc_fxval( const sc_signed& );
    sc_fxval( const sc_unsigned& );
    sc_fxval( const sc_fxval& );
    sc_fxval( const sc_fxval_fast& );
    sc_fxval( const sc_fxnum& );
    sc_fxval( const sc_fxnum_fast& );

    ~sc_fxval();


    // unary operators

    const sc_fxval  operator - () const;
    const sc_fxval& operator + () const;


    // unary functions

    friend void neg( sc_fxval&, const sc_fxval& );


    // binary operators

#define DECL_BIN_OP_T(op,tp)                                                  \
    friend const sc_fxval operator op ( const sc_fxval&, tp );                \
    friend const sc_fxval operator op ( tp, const sc_fxval& );

#define DECL_BIN_OP(op)                                                       \
    friend const sc_fxval operator op ( const sc_fxval&, const sc_fxval& );   \
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
    DECL_BIN_OP_T(op,const sc_fxval_fast&)                                    \
    DECL_BIN_OP_T(op,const sc_fxnum_fast&)

    DECL_BIN_OP(*)
    DECL_BIN_OP(+)
    DECL_BIN_OP(-)
//    DECL_BIN_OP(/) // BCB produces illegal character
    friend const sc_fxval operator / ( const sc_fxval&, const sc_fxval& );
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
    DECL_BIN_OP_T(/,const sc_fxnum_fast&)

#undef DECL_BIN_OP_T
#undef DECL_BIN_OP

    friend const sc_fxval operator << ( const sc_fxval&, int );
    friend const sc_fxval operator >> ( const sc_fxval&, int );


    // binary functions

#define DECL_BIN_FNC_T(fnc,tp)                                                \
    friend void fnc ( sc_fxval&, const sc_fxval&, tp );                       \
    friend void fnc ( sc_fxval&, tp, const sc_fxval& );

#define DECL_BIN_FNC(fnc)                                                     \
    friend void fnc ( sc_fxval&, const sc_fxval&, const sc_fxval& );          \
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
    DECL_BIN_FNC_T(fnc,const sc_fxval_fast&)                                  \
    DECL_BIN_FNC_T(fnc,const sc_fxnum_fast&)

    DECL_BIN_FNC(mult)
    DECL_BIN_FNC(div)
    DECL_BIN_FNC(add)
    DECL_BIN_FNC(sub)

#undef DECL_BIN_FNC_T
#undef DECL_BIN_FNC

    friend void lshift( sc_fxval&, const sc_fxval&, int );
    friend void rshift( sc_fxval&, const sc_fxval&, int );


    // relational (including equality) operators

#define DECL_REL_OP_T(op,tp)                                                  \
    friend bool operator op ( const sc_fxval&, tp );                          \
    friend bool operator op ( tp, const sc_fxval& );

#define DECL_REL_OP(op)                                                       \
    friend bool operator op ( const sc_fxval&, const sc_fxval& );             \
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
    sc_fxval& operator op( tp );

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

    sc_fxval& operator ++ ();
    sc_fxval& operator -- ();


    // implicit conversion

    operator double() const;            // necessary evil!


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
    bool is_nan() const;
    bool is_inf() const;
    bool is_normal() const;
    
    bool rounding_flag() const;


    // print or dump content

    void print( ostream& = cout ) const;
    void dump( ostream& = cout ) const;


    // ...

    bool get_bit( int ) const;

protected:

    void cast( const scfx_params&, bool&, bool& );

    const char* to_string( sc_numrep, sc_fmt, const scfx_params& ) const;

    const sc_fxval b_not(                  const scfx_params& ) const;
    const sc_fxval b_and( const sc_fxval&, const scfx_params& ) const;
    const sc_fxval b_xor( const sc_fxval&, const scfx_params& ) const;
    const sc_fxval b_or ( const sc_fxval&, const scfx_params& ) const;

    bool   set( int, const scfx_params& );
    bool clear( int, const scfx_params& );

    bool set_bit( int, const scfx_params&, bool );

    bool get_slice( int, int, const scfx_params&, sc_bv_base& ) const;
    bool set_slice( int, int, const scfx_params&, const sc_bv_base& );

    void get_type( int&, int&, sc_enc& ) const;

    const sc_fxval quantization( const scfx_params&, bool& ) const;
    const sc_fxval     overflow( const scfx_params&, bool& ) const;

private:

    scfx_rep* _rep;

};


// ----------------------------------------------------------------------------
//  CLASS : sc_fxval_fast
//
//  Fixed-point value type; limited precision.
// ----------------------------------------------------------------------------

class sc_fxval_fast
{

    friend class sc_fxnum_fast;
    friend class sc_fxnum_fast_bitref;
    friend class sc_fxnum_fast_subref;

public:

    sc_fxval_fast();
    sc_fxval_fast( short );
    sc_fxval_fast( unsigned short );
    sc_fxval_fast( int );
    sc_fxval_fast( unsigned int );
    sc_fxval_fast( long );
    sc_fxval_fast( unsigned long );
    sc_fxval_fast( float );
    sc_fxval_fast( double );
    sc_fxval_fast( const char* );
    sc_fxval_fast( int64 );
    sc_fxval_fast( uint64 );
    sc_fxval_fast( const sc_int_base& );
    sc_fxval_fast( const sc_uint_base& );
    sc_fxval_fast( const sc_signed& );
    sc_fxval_fast( const sc_unsigned& );
    sc_fxval_fast( const sc_fxval& );
    sc_fxval_fast( const sc_fxval_fast& );
    sc_fxval_fast( const sc_fxnum& );
    sc_fxval_fast( const sc_fxnum_fast& );

    ~sc_fxval_fast();


    // unary operators

    const sc_fxval_fast  operator - () const;
    const sc_fxval_fast& operator + () const;


    // unary functions

    friend void neg( sc_fxval_fast&, const sc_fxval_fast& );


    // binary operators

#define DECL_BIN_OP_T(op,tp)                                                  \
    friend const sc_fxval_fast operator op ( const sc_fxval_fast&, tp );      \
    friend const sc_fxval_fast operator op ( tp, const sc_fxval_fast& );

#define DECL_BIN_OP(op)                                                       \
    friend const sc_fxval_fast operator op ( const sc_fxval_fast&,            \
					     const sc_fxval_fast& );          \
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
    DECL_BIN_OP_T(op,const sc_unsigned&)

    DECL_BIN_OP(*)
    DECL_BIN_OP(+)
    DECL_BIN_OP(-)
//    DECL_BIN_OP(/) // BCB produces illegal character
    friend const sc_fxval_fast operator / ( const sc_fxval_fast&,
					     const sc_fxval_fast& );
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

#undef DECL_BIN_OP_T
#undef DECL_BIN_OP

    friend const sc_fxval_fast operator << ( const sc_fxval_fast&, int );
    friend const sc_fxval_fast operator >> ( const sc_fxval_fast&, int );


    // binary functions

#define DECL_BIN_FNC_T(fnc,tp)                                                \
    friend void fnc ( sc_fxval_fast&, const sc_fxval_fast&, tp );             \
    friend void fnc ( sc_fxval_fast&, tp, const sc_fxval_fast& );

#define DECL_BIN_FNC(fnc)                                                     \
    friend void fnc ( sc_fxval_fast&, const sc_fxval_fast&,                   \
                      const sc_fxval_fast& );                                 \
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
    DECL_BIN_FNC_T(fnc,const sc_fxnum&)

    DECL_BIN_FNC(mult)
    DECL_BIN_FNC(div)
    DECL_BIN_FNC(add)
    DECL_BIN_FNC(sub)

#undef DECL_BIN_FNC_T
#undef DECL_BIN_FNC

    friend void lshift( sc_fxval_fast&, const sc_fxval_fast&, int );
    friend void rshift( sc_fxval_fast&, const sc_fxval_fast&, int );


    // relational (including equality) operators

#define DECL_REL_OP_T(op,tp)                                                  \
    friend bool operator op ( const sc_fxval_fast&, tp );                     \
    friend bool operator op ( tp, const sc_fxval_fast& );

#define DECL_REL_OP(op)                                                       \
    friend bool operator op ( const sc_fxval_fast&, const sc_fxval_fast& );   \
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
    DECL_REL_OP_T(op,const sc_unsigned&)

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
    sc_fxval_fast& operator op( tp );

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

    sc_fxval_fast& operator ++ ();
    sc_fxval_fast& operator -- ();


    // implicit conversion

    operator double() const;            // necessary evil!


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
    bool is_nan() const;
    bool is_inf() const;
    bool is_normal() const;

    bool rounding_flag() const;


    // print or dump content

    void print( ostream& = cout ) const;
    void dump( ostream& = cout ) const;


    // ...

    bool get_bit( int ) const;

protected:

    void cast( const scfx_params&, bool&, bool& );

    const char* to_string( sc_numrep, sc_fmt, const scfx_params& ) const;

    const sc_fxval_fast b_not( const scfx_params& ) const;
    const sc_fxval_fast b_and( const sc_fxval_fast&,
			       const scfx_params& ) const;
    const sc_fxval_fast b_xor( const sc_fxval_fast&,
			       const scfx_params& ) const;
    const sc_fxval_fast b_or ( const sc_fxval_fast&,
			       const scfx_params& ) const;

    bool   set( int, const scfx_params& );
    bool clear( int, const scfx_params& );

    bool set_bit( int, const scfx_params&, bool );

    bool get_slice( int, int, const scfx_params&, sc_bv_base& ) const;
    bool set_slice( int, int, const scfx_params&, const sc_bv_base& );

private:

    double _val;

};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  CLASS : sc_fxval
//
//  Fixed-point value type; arbitrary precision.
// ----------------------------------------------------------------------------

// private constructor

inline
sc_fxval::sc_fxval( scfx_rep* a )
: _rep( a )
{}


// public constructors

inline
sc_fxval::sc_fxval()
: _rep( new scfx_rep )
{}

#define DEFN_CTOR_A(tp)                                                       \
inline                                                                        \
sc_fxval::sc_fxval( tp a )                                                    \
: _rep( new scfx_rep( a ) )                                                   \
{}

DEFN_CTOR_A(short)
DEFN_CTOR_A(unsigned short)
DEFN_CTOR_A(int)
DEFN_CTOR_A(unsigned int)
DEFN_CTOR_A(long)
DEFN_CTOR_A(unsigned long)
DEFN_CTOR_A(float)
DEFN_CTOR_A(double)
DEFN_CTOR_A(const char*)
DEFN_CTOR_A(int64)
DEFN_CTOR_A(uint64)
DEFN_CTOR_A(const sc_signed&)
DEFN_CTOR_A(const sc_unsigned&)

#undef DEFN_CTOR_A

inline
sc_fxval::sc_fxval( const sc_fxval& a )
: _rep( new scfx_rep( *a._rep ) )
{}

inline
sc_fxval::sc_fxval( const sc_fxval_fast& a )
: _rep( new scfx_rep( a.to_double() ) )
{}


inline
sc_fxval::~sc_fxval()
{
    delete _rep;
}


// unary operators

inline
const sc_fxval
sc_fxval::operator - () const
{
    return sc_fxval( ::negate( *_rep ) );
}

inline
const sc_fxval&
sc_fxval::operator + () const
{
    return *this;
}


// unary functions

inline
void
neg( sc_fxval& c, const sc_fxval& a )
{
    delete c._rep;
    c._rep = ::negate( *a._rep );
}


// binary operators

inline
const sc_fxval
operator * ( const sc_fxval& a, const sc_fxval& b )
{
    return sc_fxval( ::multiply( *a._rep, *b._rep ) );
}

inline
const sc_fxval
operator / ( const sc_fxval& a, const sc_fxval& b )
{
    return sc_fxval( ::divide( *a._rep, *b._rep ) );
}

inline
const sc_fxval
operator + ( const sc_fxval& a, const sc_fxval& b )
{
    return sc_fxval( ::add( *a._rep, *b._rep ) );
}

inline
const sc_fxval
operator - ( const sc_fxval& a, const sc_fxval& b )
{
    return sc_fxval( ::subtract( *a._rep, *b._rep ) );
}

#define DEFN_BIN_OP_T(op,tp)                                                  \
inline                                                                        \
const sc_fxval                                                                \
operator op ( const sc_fxval& a, tp b )                                       \
{                                                                             \
    return ( a op sc_fxval( b ) );                                            \
}                                                                             \
                                                                              \
inline                                                                        \
const sc_fxval                                                                \
operator op ( tp a, const sc_fxval& b )                                       \
{                                                                             \
    return ( sc_fxval( a ) op b );                                            \
}

#define DEFN_BIN_OP(op)                                                       \
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
// DEFN_BIN_OP(/)
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
const sc_fxval
operator << ( const sc_fxval& a, int b )
{
    return sc_fxval( ::lshift( *a._rep, b ) );
}

inline
const sc_fxval
operator >> ( const sc_fxval& a, int b )
{
    return sc_fxval( ::rshift( *a._rep, b ) );
}


// binary functions

inline
void
mult( sc_fxval& c, const sc_fxval& a, const sc_fxval& b )
{
    delete c._rep;
    c._rep = ::multiply( *a._rep, *b._rep );
}

inline
void
div( sc_fxval& c, const sc_fxval& a, const sc_fxval& b )
{
    delete c._rep;
    c._rep = ::divide( *a._rep, *b._rep );
}

inline
void
add( sc_fxval& c, const sc_fxval& a, const sc_fxval& b )
{
    delete c._rep;
    c._rep = ::add( *a._rep, *b._rep );
}

inline
void
sub( sc_fxval& c, const sc_fxval& a, const sc_fxval& b )
{
    delete c._rep;
    c._rep = ::subtract( *a._rep, *b._rep );
}

#define DEFN_BIN_FNC_T(fnc,tp)                                                \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval& c, const sc_fxval& a, tp b )                                  \
{                                                                             \
    fnc ( c, a, sc_fxval( b ) );                                              \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval& c, tp a, const sc_fxval& b )                                  \
{                                                                             \
    fnc ( c, sc_fxval( a ), b );                                              \
}

#define DEFN_BIN_FNC(fnc)                                                     \
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
DEFN_BIN_FNC_T(fnc,const sc_fxval_fast&)

DEFN_BIN_FNC(mult)
DEFN_BIN_FNC(div)
DEFN_BIN_FNC(add)
DEFN_BIN_FNC(sub)

#undef DEFN_BIN_FNC_T
#undef DEFN_BIN_FNC


inline
void
lshift( sc_fxval& c, const sc_fxval& a, int b )
{
    delete c._rep;
    c._rep = ::lshift( *a._rep, b );
}

inline
void
rshift( sc_fxval& c, const sc_fxval& a, int b )
{
    delete c._rep;
    c._rep = ::rshift( *a._rep, b );
}


// relational (including equality) operators

inline
bool
operator < ( const sc_fxval& a, const sc_fxval& b )
{
    int result = ::compare( *a._rep, *b._rep );
    return ( result < 0 );
}

inline
bool
operator <= ( const sc_fxval& a, const sc_fxval& b )
{
    int result = ::compare( *a._rep, *b._rep );
    return ( result <= 0 );
}

inline
bool
operator > ( const sc_fxval& a, const sc_fxval& b )
{
    int result = ::compare( *a._rep, *b._rep );
    return ( result > 0 && result != 2 );
}

inline
bool
operator >= ( const sc_fxval& a, const sc_fxval& b )
{
    int result = ::compare( *a._rep, *b._rep );
    return ( result >= 0 && result != 2 );
}

inline
bool
operator == ( const sc_fxval& a, const sc_fxval& b )
{
    int result = ::compare( *a._rep, *b._rep );
    return ( result == 0 );
}

inline
bool
operator != ( const sc_fxval& a, const sc_fxval& b)
{
    int result = ::compare( *a._rep, *b._rep );
    return ( result != 0 );
}

#define DEFN_REL_OP_T(op,tp)                                                  \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxval& a, tp b )                                       \
{                                                                             \
    return ( a op sc_fxval( b ) );                                            \
}                                                                             \
                                                                              \
inline                                                                        \
bool                                                                          \
operator op ( tp a, const sc_fxval& b )                                       \
{                                                                             \
    return ( sc_fxval( a ) op b );                                            \
}

#define DEFN_REL_OP(op)                                                       \
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

inline
sc_fxval&
sc_fxval::operator = ( const sc_fxval& a )
{
    if( &a != this )
    {
	*_rep = *a._rep;
    }
    return *this;
}

inline
sc_fxval&
sc_fxval::operator *= ( const sc_fxval& b )
{
    scfx_rep* new_rep = ::multiply( *_rep, *b._rep );
    delete _rep;
    _rep = new_rep;
    return *this;
}

inline
sc_fxval&
sc_fxval::operator /= ( const sc_fxval& b )
{
    scfx_rep* new_rep = ::divide( *_rep, *b._rep );
    delete _rep;
    _rep = new_rep;
    return *this;
}

inline
sc_fxval&
sc_fxval::operator += ( const sc_fxval& b )
{
    scfx_rep* new_rep = ::add( *_rep, *b._rep );
    delete _rep;
    _rep = new_rep;
    return *this;
}

inline
sc_fxval&
sc_fxval::operator -= ( const sc_fxval& b )
{
    scfx_rep* new_rep = ::subtract( *_rep, *b._rep );
    delete _rep;
    _rep = new_rep;
    return *this;
}

#define DEFN_ASN_OP_T(op,tp)                                                  \
inline                                                                        \
sc_fxval&                                                                     \
sc_fxval::operator op ( tp b )                                                \
{                                                                             \
    return ( operator op ( sc_fxval( b ) ) );                                 \
}

#define DEFN_ASN_OP(op)                                                       \
DEFN_ASN_OP_T(op,short)                                                       \
DEFN_ASN_OP_T(op,unsigned short)                                              \
DEFN_ASN_OP_T(op,int)                                                         \
DEFN_ASN_OP_T(op,unsigned int)                                                \
DEFN_ASN_OP_T(op,long)                                                        \
DEFN_ASN_OP_T(op,unsigned long)                                               \
DEFN_ASN_OP_T(op,float)                                                       \
DEFN_ASN_OP_T(op,double)                                                      \
DEFN_ASN_OP_T(op,const char*)                                                 \
DEFN_ASN_OP_T(op,int64)                                                       \
DEFN_ASN_OP_T(op,uint64)                                                      \
DEFN_ASN_OP_T(op,const sc_signed&)                                            \
DEFN_ASN_OP_T(op,const sc_unsigned&)                                          \
DEFN_ASN_OP_T(op,const sc_fxval_fast&)

DEFN_ASN_OP(=)

DEFN_ASN_OP(*=)
DEFN_ASN_OP(/=)
DEFN_ASN_OP(+=)
DEFN_ASN_OP(-=)

#undef DEFN_ASN_OP_T
#undef DEFN_ASN_OP


inline
sc_fxval&
sc_fxval::operator <<= ( int b )
{
    _rep->lshift( b );
    return *this;
}

inline
sc_fxval&
sc_fxval::operator >>= ( int b )
{
    _rep->rshift( b );
    return *this;
}


// auto-increment and auto-decrement

inline
const sc_fxval
sc_fxval::operator ++ ( int )
{
    sc_fxval c = *this;
    (*this) += 1;
    return c;
}

inline
const sc_fxval
sc_fxval::operator -- ( int )
{
    sc_fxval c = *this;
    (*this) -= 1;
    return c;
}

inline
sc_fxval&
sc_fxval::operator ++ ()
{
    (*this) += 1;
    return *this;
}

inline
sc_fxval&
sc_fxval::operator -- ()
{
    (*this) -= 1;
    return *this;
}


// implicit conversion

inline
sc_fxval::operator double() const
{
    return _rep->to_double();
}


// explicit conversion to primitive types

inline
short
sc_fxval::to_short() const
{
    return static_cast<short>( _rep->to_double() );
}

inline
unsigned short
sc_fxval::to_ushort() const
{
    return static_cast<unsigned short>( _rep->to_double() );
}

inline
int
sc_fxval::to_int() const
{
    return static_cast<int>( _rep->to_double() );
}

inline
unsigned int
sc_fxval::to_uint() const
{
    return static_cast<unsigned int>( _rep->to_double() );
}

inline
long
sc_fxval::to_long() const
{
    return static_cast<long>( _rep->to_double() );
}

inline
unsigned long
sc_fxval::to_ulong() const
{
    return static_cast<unsigned long>( _rep->to_double() );
}

inline
float
sc_fxval::to_float() const
{
    return static_cast<float>( _rep->to_double() );
}

inline
double
sc_fxval::to_double() const
{
    return _rep->to_double();
}


// query value

inline
bool
sc_fxval::is_neg() const
{
    return _rep->is_neg();
}

inline
bool
sc_fxval::is_zero() const
{
    return _rep->is_zero();
}

inline
bool
sc_fxval::is_nan() const
{
    return _rep->is_nan();
}

inline
bool
sc_fxval::is_inf() const
{
    return _rep->is_inf();
}

inline
bool
sc_fxval::is_normal() const
{
    return _rep->is_normal();
}


inline
bool
sc_fxval::rounding_flag() const
{
    return _rep->rounding_flag();
}


// ...

inline
bool
sc_fxval::get_bit( int i ) const
{
    return _rep->get_bit( i );
}


// protected methods and friend functions

inline
void
sc_fxval::cast( const scfx_params& params, bool& q_flag, bool& o_flag )
{
    _rep->cast( params, q_flag, o_flag );
}


inline
const sc_fxval
sc_fxval::b_not( const scfx_params& params ) const
{
    return sc_fxval( ::b_not( *_rep, params ) );
}

inline
const sc_fxval
sc_fxval::b_and( const sc_fxval& b, const scfx_params& params ) const
{
    return sc_fxval( ::b_and( *_rep, *b._rep, params ) );
}

inline
const sc_fxval
sc_fxval::b_xor( const sc_fxval& b, const scfx_params& params ) const
{
    return sc_fxval( ::b_xor( *_rep, *b._rep, params ) );
}

inline
const sc_fxval
sc_fxval::b_or( const sc_fxval& b, const scfx_params& params ) const
{
    return sc_fxval( ::b_or( *_rep, *b._rep, params ) );
}


inline
bool
sc_fxval::set( int i, const scfx_params& params )
{
    return _rep->set( i, params );
}

inline
bool
sc_fxval::clear( int i, const scfx_params& params )
{
    return _rep->clear( i, params );
}


inline
bool
sc_fxval::set_bit( int i, const scfx_params& params, bool high )
{
    if( high )
	return set( i, params );
    else
	return clear( i, params );
}


inline
bool
sc_fxval::get_slice( int i, int j, const scfx_params& params,
		     sc_bv_base& bv ) const
{
    return _rep->get_slice( i, j, params, bv );
}

inline
bool
sc_fxval::set_slice( int i, int j, const scfx_params& params,
		     const sc_bv_base& bv )
{
    return _rep->set_slice( i, j, params, bv );
}


inline
void
sc_fxval::get_type( int& wl, int& iwl, sc_enc& enc ) const
{
    _rep->get_type( wl, iwl, enc );
}


inline
const sc_fxval
sc_fxval::quantization( const scfx_params& params, bool& q_flag ) const
{
    return sc_fxval( ::quantization( *_rep, params, q_flag ) );
}

inline
const sc_fxval
sc_fxval::overflow( const scfx_params& params, bool& o_flag ) const
{
    return sc_fxval( ::overflow( *_rep, params, o_flag ) );
}


inline
ostream&
operator << ( ostream& os, const sc_fxval& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxval_fast
//
//  Fixed-point value type; limited precision.
// ----------------------------------------------------------------------------

inline
sc_fxval_fast::sc_fxval_fast()
: _val( 0.0 )
{}

#define DEFN_CTOR_A(tp)                                                       \
inline                                                                        \
sc_fxval_fast::sc_fxval_fast( tp a )                                          \
: _val( static_cast<double>( a ) )                                            \
{}

DEFN_CTOR_A(short)
DEFN_CTOR_A(unsigned short)
DEFN_CTOR_A(int)
DEFN_CTOR_A(unsigned int)
DEFN_CTOR_A(long)
DEFN_CTOR_A(unsigned long)
DEFN_CTOR_A(float)
DEFN_CTOR_A(double)
DEFN_CTOR_A(int64)
DEFN_CTOR_A(uint64)

#undef DEFN_CTOR_A

inline
sc_fxval_fast::sc_fxval_fast( const sc_signed& a )
: _val( a.to_double() )
{}

inline
sc_fxval_fast::sc_fxval_fast( const sc_unsigned& a )
: _val( a.to_double() )
{}

inline
sc_fxval_fast::sc_fxval_fast( const sc_fxval& a )
: _val( a.to_double() )
{}

inline
sc_fxval_fast::sc_fxval_fast( const sc_fxval_fast& a )
: _val( a._val )
{}


inline
sc_fxval_fast::~sc_fxval_fast()
{}


// unary operators

inline
const sc_fxval_fast
sc_fxval_fast::operator - () const
{
    return sc_fxval_fast( - _val );
}

inline
const sc_fxval_fast&
sc_fxval_fast::operator + () const
{
    return *this;
}


// unary functions

inline
void
neg( sc_fxval_fast& c, const sc_fxval_fast& a )
{
    c._val = - a._val;
}


// binary operators

#define DEFN_BIN_OP_T_A(op,tp)                                                \
inline                                                                        \
const sc_fxval_fast                                                           \
operator op ( const sc_fxval_fast& a, tp b )                                  \
{                                                                             \
    return sc_fxval_fast( a._val op static_cast<double>( b ) );               \
}                                                                             \
                                                                              \
inline                                                                        \
const sc_fxval_fast                                                           \
operator op ( tp a, const sc_fxval_fast& b )                                  \
{                                                                             \
    return sc_fxval_fast( static_cast<double>( a ) op b._val );               \
}

#define DEFN_BIN_OP_T_C(op,tp)                                                \
inline                                                                        \
const sc_fxval_fast                                                           \
operator op ( const sc_fxval_fast& a, tp b )                                  \
{                                                                             \
    return sc_fxval_fast( a._val op b.to_double() );                          \
}                                                                             \
                                                                              \
inline                                                                        \
const sc_fxval_fast                                                           \
operator op ( tp a, const sc_fxval_fast& b )                                  \
{                                                                             \
    return sc_fxval_fast( a.to_double() op b._val );                          \
}

#define DEFN_BIN_OP_T_D(op,tp)                                                \
inline                                                                        \
const sc_fxval_fast                                                           \
operator op ( const sc_fxval_fast& a, tp b )                                  \
{                                                                             \
    sc_fxval_fast tmp( b );                                                   \
    return sc_fxval_fast( a._val op tmp._val );                               \
}                                                                             \
                                                                              \
inline                                                                        \
const sc_fxval_fast                                                           \
operator op ( tp a, const sc_fxval_fast& b )                                  \
{                                                                             \
    sc_fxval_fast tmp( a );                                                   \
    return sc_fxval_fast( tmp._val op b._val );                               \
}

#define DEFN_BIN_OP(op)                                                       \
inline                                                                        \
const sc_fxval_fast                                                           \
operator op ( const sc_fxval_fast& a, const sc_fxval_fast& b )                \
{                                                                             \
    return sc_fxval_fast( a._val op b._val );                                 \
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
DEFN_BIN_OP_T_D(op,const char*)                                               \
DEFN_BIN_OP_T_A(op,int64)                                                     \
DEFN_BIN_OP_T_A(op,uint64)                                                    \
DEFN_BIN_OP_T_C(op,const sc_signed&)                                          \
DEFN_BIN_OP_T_C(op,const sc_unsigned&)

DEFN_BIN_OP(*)
DEFN_BIN_OP(+)
DEFN_BIN_OP(-)
// DEFN_BIN_OP(/) // BCB produces illegal symbol
inline
const sc_fxval_fast
operator / ( const sc_fxval_fast& a, const sc_fxval_fast& b )
{
    return sc_fxval_fast( a._val / b._val );
}

DEFN_BIN_OP_T_A(/,short)
DEFN_BIN_OP_T_A(/,unsigned short)
DEFN_BIN_OP_T_A(/,int)
DEFN_BIN_OP_T_A(/,unsigned int)
DEFN_BIN_OP_T_A(/,long)
DEFN_BIN_OP_T_A(/,unsigned long)
DEFN_BIN_OP_T_A(/,float)
DEFN_BIN_OP_T_A(/,double)
DEFN_BIN_OP_T_D(/,const char*)
DEFN_BIN_OP_T_A(/,int64)
DEFN_BIN_OP_T_A(/,uint64)
DEFN_BIN_OP_T_C(/,const sc_signed&)
DEFN_BIN_OP_T_C(/,const sc_unsigned&)

#undef DEFN_BIN_OP_T_A
#undef DEFN_BIN_OP_T_C
#undef DEFN_BIN_OP_T_D
#undef DEFN_BIN_OP


inline
const sc_fxval_fast
operator << ( const sc_fxval_fast& a, int b )
{
    return sc_fxval_fast( a._val * scfx_pow2( b ) );
}

inline
const sc_fxval_fast
operator >> ( const sc_fxval_fast& a, int b )
{
    return sc_fxval_fast( a._val * scfx_pow2( -b ) );
}


// binary functions

#define DEFN_BIN_FNC_T_A(fnc,op,tp)                                           \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval_fast& c, const sc_fxval_fast& a, tp b )                        \
{                                                                             \
    c._val = a._val op static_cast<double>( b );                              \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval_fast& c, tp a, const sc_fxval_fast& b )                        \
{                                                                             \
    c._val = static_cast<double>( a ) op b._val;                              \
}

#define DEFN_BIN_FNC_T_C(fnc,op,tp)                                           \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval_fast& c, const sc_fxval_fast& a, tp b )                        \
{                                                                             \
    c._val = a._val op b.to_double();                                         \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval_fast& c, tp a, const sc_fxval_fast& b )                        \
{                                                                             \
    c._val = a.to_double() op b._val;                                         \
}

#define DEFN_BIN_FNC_T_D(fnc,op,tp)                                           \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval_fast& c, const sc_fxval_fast& a, tp b )                        \
{                                                                             \
    sc_fxval_fast tmp( b );                                                   \
    c._val = a._val op tmp._val;                                              \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval_fast& c, tp a, const sc_fxval_fast& b )                        \
{                                                                             \
    sc_fxval_fast tmp( a );                                                   \
    c._val = tmp._val op b._val;                                              \
}

#define DEFN_BIN_FNC(fnc,op)                                                  \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval_fast& c, const sc_fxval_fast& a, const sc_fxval_fast& b )      \
{                                                                             \
    c._val = a._val op b._val;                                                \
}                                                                             \
                                                                              \
DEFN_BIN_FNC_T_A(fnc,op,short)                                                \
DEFN_BIN_FNC_T_A(fnc,op,unsigned short)                                       \
DEFN_BIN_FNC_T_A(fnc,op,int)                                                  \
DEFN_BIN_FNC_T_A(fnc,op,unsigned int)                                         \
DEFN_BIN_FNC_T_A(fnc,op,long)                                                 \
DEFN_BIN_FNC_T_A(fnc,op,unsigned long)                                        \
DEFN_BIN_FNC_T_A(fnc,op,float)                                                \
DEFN_BIN_FNC_T_A(fnc,op,double)                                               \
DEFN_BIN_FNC_T_D(fnc,op,const char*)                                          \
DEFN_BIN_FNC_T_A(fnc,op,int64)                                                \
DEFN_BIN_FNC_T_A(fnc,op,uint64)                                               \
DEFN_BIN_FNC_T_C(fnc,op,const sc_signed&)                                     \
DEFN_BIN_FNC_T_C(fnc,op,const sc_unsigned&)

DEFN_BIN_FNC(mult,*)
DEFN_BIN_FNC(div,/)
DEFN_BIN_FNC(add,+)
DEFN_BIN_FNC(sub,-)

#undef DEFN_BIN_FNC_T_A
#undef DEFN_BIN_FNC_T_C
#undef DEFN_BIN_FNC_T_D
#undef DEFN_BIN_FNC


inline
void
lshift( sc_fxval_fast& c, const sc_fxval_fast& a, int b )
{
    c._val = a._val * scfx_pow2( b );
}

inline
void
rshift( sc_fxval_fast& c, const sc_fxval_fast& a, int b )
{
    c._val = a._val * scfx_pow2( -b );
}


// relational (including equality) operators

#define DEFN_REL_OP_T_A(op,tp)                                                \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxval_fast& a, tp b )                                  \
{                                                                             \
    return ( a._val op static_cast<double>( b ) );                            \
}                                                                             \
                                                                              \
inline                                                                        \
bool                                                                          \
operator op ( tp a, const sc_fxval_fast& b )                                  \
{                                                                             \
    return ( static_cast<double>( a ) op b._val );                            \
}

#define DEFN_REL_OP_T_C(op,tp)                                                \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxval_fast& a, tp b )                                  \
{                                                                             \
    return ( a._val op b.to_double() );                                       \
}                                                                             \
                                                                              \
inline                                                                        \
bool                                                                          \
operator op ( tp a, const sc_fxval_fast& b )                                  \
{                                                                             \
    return ( a.to_double() op b._val );                                       \
}

#define DEFN_REL_OP_T_D(op,tp)                                                \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxval_fast& a, tp b )                                  \
{                                                                             \
    sc_fxval_fast tmp( b );                                                   \
    return ( a._val op tmp._val );                                            \
}                                                                             \
                                                                              \
inline                                                                        \
bool                                                                          \
operator op ( tp a, const sc_fxval_fast& b )                                  \
{                                                                             \
    sc_fxval_fast tmp( a );                                                   \
    return ( tmp._val op b._val );                                            \
}

#define DEFN_REL_OP(op)                                                       \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxval_fast& a, const sc_fxval_fast& b )                \
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
DEFN_REL_OP_T_D(op,const char*)                                               \
DEFN_REL_OP_T_A(op,int64)                                                     \
DEFN_REL_OP_T_A(op,uint64)                                                    \
DEFN_REL_OP_T_C(op,const sc_signed&)                                          \
DEFN_REL_OP_T_C(op,const sc_unsigned&)

DEFN_REL_OP(<)
DEFN_REL_OP(<=)
DEFN_REL_OP(>)
DEFN_REL_OP(>=)
DEFN_REL_OP(==)
DEFN_REL_OP(!=)

#undef DEFN_REL_OP_T_A
#undef DEFN_REL_OP_T_C
#undef DEFN_REL_OP_T_D
#undef DEFN_REL_OP


// assignment operators

#define DEFN_ASN_OP_T_A(op,tp)                                                \
inline                                                                        \
sc_fxval_fast&                                                                \
sc_fxval_fast::operator op ( tp b )                                           \
{                                                                             \
    _val op static_cast<double>( b );                                         \
    return *this;                                                             \
}

#define DEFN_ASN_OP_T_C(op,tp)                                                \
inline                                                                        \
sc_fxval_fast&                                                                \
sc_fxval_fast::operator op ( tp b )                                           \
{                                                                             \
    _val op b.to_double();                                                    \
    return *this;                                                             \
}

#define DEFN_ASN_OP_T_D(op,tp)                                                \
inline                                                                        \
sc_fxval_fast&                                                                \
sc_fxval_fast::operator op ( tp b )                                           \
{                                                                             \
    sc_fxval_fast tmp( b );                                                   \
    _val op tmp._val;                                                         \
    return *this;                                                             \
}

#define DEFN_ASN_OP(op)                                                       \
inline                                                                        \
sc_fxval_fast&                                                                \
sc_fxval_fast::operator op ( const sc_fxval_fast& b )                         \
{                                                                             \
    _val op b._val;                                                           \
    return *this;                                                             \
}                                                                             \
                                                                              \
DEFN_ASN_OP_T_A(op,short)                                                     \
DEFN_ASN_OP_T_A(op,unsigned short)                                            \
DEFN_ASN_OP_T_A(op,int)                                                       \
DEFN_ASN_OP_T_A(op,unsigned int)                                              \
DEFN_ASN_OP_T_A(op,long)                                                      \
DEFN_ASN_OP_T_A(op,unsigned long)                                             \
DEFN_ASN_OP_T_A(op,float)                                                     \
DEFN_ASN_OP_T_A(op,double)                                                    \
DEFN_ASN_OP_T_D(op,const char*)                                               \
DEFN_ASN_OP_T_A(op,int64)                                                     \
DEFN_ASN_OP_T_A(op,uint64)                                                    \
DEFN_ASN_OP_T_C(op,const sc_signed&)                                          \
DEFN_ASN_OP_T_C(op,const sc_unsigned&)

DEFN_ASN_OP(=)

DEFN_ASN_OP(*=)
DEFN_ASN_OP(/=)
DEFN_ASN_OP(+=)
DEFN_ASN_OP(-=)

#undef DEFN_ASN_OP_T_A
#undef DEFN_ASN_OP_T_C
#undef DEFN_ASN_OP_T_D
#undef DEFN_ASN_OP


inline
sc_fxval_fast&
sc_fxval_fast::operator <<= ( int b )
{
    _val *= scfx_pow2( b );
    return *this;
}

inline
sc_fxval_fast&
sc_fxval_fast::operator >>= ( int b )
{
    _val *= scfx_pow2( -b );
    return *this;
}


// auto-increment and auto-decrement

inline
const sc_fxval_fast
sc_fxval_fast::operator ++ ( int )
{
    double c = _val;
    _val = _val + 1;
    return sc_fxval_fast( c );
}

inline
const sc_fxval_fast
sc_fxval_fast::operator -- ( int )
{
    double c = _val;
    _val = _val - 1;
    return sc_fxval_fast( c );
}

inline
sc_fxval_fast&
sc_fxval_fast::operator ++ ()
{
    _val = _val + 1;
    return *this;
}

inline
sc_fxval_fast&
sc_fxval_fast::operator -- ()
{
    _val = _val - 1;
    return *this;
}


// implicit conversion

inline
sc_fxval_fast::operator double() const
{
    return _val;
}


// explicit conversion to primitive types

inline
short
sc_fxval_fast::to_short() const
{
    return static_cast<short>( _val );
}

inline
unsigned short
sc_fxval_fast::to_ushort() const
{
    return static_cast<unsigned short>( _val );
}

inline
int
sc_fxval_fast::to_int() const
{
    return static_cast<int>( _val );
}

inline
unsigned int
sc_fxval_fast::to_uint() const
{
    return static_cast<unsigned int>( _val );
}

inline
long
sc_fxval_fast::to_long() const
{
    return static_cast<long>( _val );
}

inline
unsigned long
sc_fxval_fast::to_ulong() const
{
    return static_cast<unsigned long>( _val );
}

inline
float
sc_fxval_fast::to_float() const
{
    return static_cast<float>( _val );
}

inline
double
sc_fxval_fast::to_double() const
{
    return _val;
}


// query value

inline
bool
sc_fxval_fast::is_neg() const
{
    scfx_ieee_double id( _val );
    return ( id.negative() != 0 );
}

inline
bool
sc_fxval_fast::is_zero() const
{
    scfx_ieee_double id( _val );
    return id.is_zero();
}

inline
bool
sc_fxval_fast::is_nan() const
{
    scfx_ieee_double id( _val );
    return id.is_nan();
}

inline
bool
sc_fxval_fast::is_inf() const
{
    scfx_ieee_double id( _val );
    return id.is_inf();
}

inline
bool
sc_fxval_fast::is_normal() const
{
    scfx_ieee_double id( _val );
    return ( id.is_normal() || id.is_subnormal() || id.is_zero() );
}


inline
bool
sc_fxval_fast::rounding_flag() const
{
    // does not apply to sc_fxval_fast; included for API compatibility
    return false;
}


inline
bool
sc_fxval_fast::set_bit( int i, const scfx_params& params, bool high )
{
    if( high )
	return set( i, params );
    else
	return clear( i, params );
}


inline
ostream&
operator << ( ostream& os, const sc_fxval_fast& a )
{
    a.print( os );
    return os;
}


#endif

// Taf!
