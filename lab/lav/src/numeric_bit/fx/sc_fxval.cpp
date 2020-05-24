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

    sc_fxval.cpp - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/



#include <ctype.h>
#include <math.h>
#include <string.h>

#include "sc_fxval.h"
#include "sc_fxnum.h"
#include "sc_bv.h"

#if defined(__BCPLUSPLUS__)
#pragma hdrstop
#endif


// ----------------------------------------------------------------------------
//  CLASS : sc_fxval
//
//  Fixed-point value type; arbitrary precision.
// ----------------------------------------------------------------------------

// public constructors

#define DEFN_CTOR_A(tp)                                                       \
sc_fxval::sc_fxval( tp a )                                                    \
: _rep( new scfx_rep( a.num ) )                                               \
{}

DEFN_CTOR_A(const sc_int_base&)
DEFN_CTOR_A(const sc_uint_base&)

#undef DEFN_CTOR_A

sc_fxval::sc_fxval( const sc_fxnum& a )
: _rep( new scfx_rep( *a.value()._rep ) )
{}

sc_fxval::sc_fxval( const sc_fxnum_fast& a )
: _rep( new scfx_rep( a.value().to_double() ) )
{}


// binary operators

#define DEFN_BIN_OP_T(op,tp)                                                  \
const sc_fxval                                                                \
operator op ( const sc_fxval& a, tp b )                                       \
{                                                                             \
    return ( a op sc_fxval( b ) );                                            \
}                                                                             \
                                                                              \
const sc_fxval                                                                \
operator op ( tp a, const sc_fxval& b )                                       \
{                                                                             \
    return ( sc_fxval( a ) op b );                                            \
}

#define DEFN_BIN_OP(op)                                                       \
DEFN_BIN_OP_T(op,const sc_int_base&)                                          \
DEFN_BIN_OP_T(op,const sc_uint_base&)                                         \
DEFN_BIN_OP_T(op,const sc_fxnum_fast&)

DEFN_BIN_OP(*)
DEFN_BIN_OP(+)
DEFN_BIN_OP(-)
// DEFN_BIN_OP(/) // BCB produces illegal symbol
DEFN_BIN_OP_T(/,const sc_int_base&)
DEFN_BIN_OP_T(/,const sc_uint_base&)
DEFN_BIN_OP_T(/,const sc_fxnum_fast&)

#undef DEFN_BIN_OP_T
#undef DEFN_BIN_OP


// binary functions

#define DEFN_BIN_FNC_T(fnc,tp)                                                \
void                                                                          \
fnc ( sc_fxval& c, const sc_fxval& a, tp b )                                  \
{                                                                             \
    fnc ( c, a, sc_fxval( b ) );                                              \
}                                                                             \
                                                                              \
void                                                                          \
fnc ( sc_fxval& c, tp a, const sc_fxval& b )                                  \
{                                                                             \
    fnc ( c, sc_fxval( a ), b );                                              \
}

#define DEFN_BIN_FNC(fnc)                                                     \
DEFN_BIN_FNC_T(fnc,const sc_int_base&)                                        \
DEFN_BIN_FNC_T(fnc,const sc_uint_base&)                                       \
DEFN_BIN_FNC_T(fnc,const sc_fxnum_fast&)

DEFN_BIN_FNC(mult)
DEFN_BIN_FNC(div)
DEFN_BIN_FNC(add)
DEFN_BIN_FNC(sub)

#undef DEFN_BIN_FNC_T
#undef DEFN_BIN_FNC


// relational (including equality) operators

#define DEFN_REL_OP_T(op,tp)                                                  \
bool                                                                          \
operator op ( const sc_fxval& a, tp b )                                       \
{                                                                             \
    return ( a op sc_fxval( b ) );                                            \
}                                                                             \
                                                                              \
bool                                                                          \
operator op ( tp a, const sc_fxval& b )                                       \
{                                                                             \
    return ( sc_fxval( a ) op b );                                            \
}

#define DEFN_REL_OP(op)                                                       \
DEFN_REL_OP_T(op,const sc_int_base&)                                          \
DEFN_REL_OP_T(op,const sc_uint_base&)                                         \
DEFN_REL_OP_T(op,const sc_fxnum_fast&)

DEFN_REL_OP(<)
DEFN_REL_OP(<=)
DEFN_REL_OP(>)
DEFN_REL_OP(>=)
DEFN_REL_OP(==)
DEFN_REL_OP(!=)

#undef DEFN_REL_OP_T
#undef DEFN_REL_OP


// assignment operators

#define DEFN_ASN_OP_T(op,tp)                                                  \
sc_fxval&                                                                     \
sc_fxval::operator op ( tp b )                                                \
{                                                                             \
    return ( operator op ( sc_fxval( b ) ) );                                 \
}

#define DEFN_ASN_OP(op)                                                       \
DEFN_ASN_OP_T(op,const sc_int_base&)                                          \
DEFN_ASN_OP_T(op,const sc_uint_base&)                                         \
DEFN_ASN_OP_T(op,const sc_fxnum&)                                             \
DEFN_ASN_OP_T(op,const sc_fxnum_fast&)

DEFN_ASN_OP(=)

DEFN_ASN_OP(*=)
DEFN_ASN_OP(/=)
DEFN_ASN_OP(+=)
DEFN_ASN_OP(-=)

#undef DEFN_ASN_OP_T
#undef DEFN_ASN_OP


// explicit conversion to character string

const sc_string
sc_fxval::to_string() const
{
    return _rep->to_string( SC_DEC, SC_E );
}

const sc_string
sc_fxval::to_string( sc_numrep numrep ) const
{
    return _rep->to_string( numrep, SC_E );
}

const sc_string
sc_fxval::to_string( sc_fmt fmt ) const
{
    return _rep->to_string( SC_DEC, fmt );
}

const sc_string
sc_fxval::to_string( sc_numrep numrep, sc_fmt fmt ) const
{
    return _rep->to_string( numrep, fmt );
}


const sc_string
sc_fxval::to_dec() const
{
    return _rep->to_string( SC_DEC, SC_E );
}

const sc_string
sc_fxval::to_bin() const
{
    return _rep->to_string( SC_BIN, SC_E );
}

const sc_string
sc_fxval::to_oct() const
{
    return _rep->to_string( SC_OCT, SC_E );
}

const sc_string
sc_fxval::to_hex() const
{
    return _rep->to_string( SC_HEX, SC_E );
}


// print or dump content

void
sc_fxval::print( ostream& os ) const
{
    _rep->print( os );
}

void
sc_fxval::dump( ostream& os ) const
{
    os << "sc_fxval" << endl;
    os << "(" << endl;
    os << "rep = ";
    _rep->dump( os );
    // TO BE COMPLETED
    // os << "r_flag   = " << _r_flag << endl;
    // os << "observer = ";
    // if( _observer != 0 )
    //     _observer->dump( os );
    // else
    //     os << "0" << endl;
    os << ")" << endl;
}


// protected methods and friend functions

const char*
sc_fxval::to_string( sc_numrep numrep, sc_fmt fmt,
		     const scfx_params& params ) const
{
    return _rep->to_string( numrep, fmt, &params );
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxval_fast
//
//  Fixed-point value types; limited precision.
// ----------------------------------------------------------------------------

#define _SCFX_FAIL_IF(cnd)                                                    \
{                                                                             \
    if( ( cnd ) )                                                             \
        return static_cast<double>( scfx_ieee_double::nan() );                \
}


static
double
from_string( const char* s )
{
    _SCFX_FAIL_IF( s == 0 || *s == 0 );

    scfx_string s2;
    s2 += s;
    s2 += '\0';

    bool sign_char;
    int sign = scfx_parse_sign( s, sign_char );

    sc_numrep numrep = scfx_parse_prefix( s );

    int base = 0;

    switch( numrep )
    {
	case SC_DEC:
	{
	    base = 10;
	    if( scfx_is_nan( s ) )  // special case: NaN
		return static_cast<double>( scfx_ieee_double::nan() );
	    if( scfx_is_inf( s ) )  // special case: Infinity
		return static_cast<double>( scfx_ieee_double::inf( sign ) );
	    break;
	}
	case SC_BIN:
	case SC_BIN_US:
	{
	    _SCFX_FAIL_IF( sign_char );
	    base = 2;
	    break;
	}
	
	case SC_BIN_SM:
	{
	    base = 2;
	    break;
	}
	case SC_OCT:
	case SC_OCT_US:
	{
	    _SCFX_FAIL_IF( sign_char );
	    base = 8;
	    break;
	}
	case SC_OCT_SM:
	{
	    base = 8;
	    break;
	}
	case SC_HEX:
	case SC_HEX_US:
	{
	    _SCFX_FAIL_IF( sign_char );
	    base = 16;
	    break;
	}
	case SC_HEX_SM:
	{
	    base = 16;
	    break;
	}
	case SC_CSD:
	{
	    _SCFX_FAIL_IF( sign_char );
	    base = 2;
	    scfx_csd2tc( s2 );
	    s = (const char*) s2 + 4;
	    numrep = SC_BIN;
	    break;
	}
       default:;// Martin, what is default???
    }

    //
    // find end of mantissa and count the digits and points
    //

    const char *end = s;
    bool based_point = false;
    int int_digits = 0;
    int frac_digits = 0;

    while( *end )
    {
	if( scfx_exp_start( end ) )
	    break;
	
	if( *end == '.' )
	{
	    _SCFX_FAIL_IF( based_point );
	    based_point = true;
	}
	else
	{
	    _SCFX_FAIL_IF( ! scfx_is_digit( *end, numrep ) );
	    if( based_point )
		frac_digits ++;
	    else
		int_digits ++;
	}

	end ++;
    }

    _SCFX_FAIL_IF( int_digits == 0 && frac_digits == 0 );

    // [ exponent ]
    
    int exponent = 0;

    if( *end )
    {
	for( const char *e = end + 2; *e; e ++ )
	    _SCFX_FAIL_IF( ! scfx_is_digit( *e, SC_DEC ) );
	exponent = atoi( end + 1 );
    }

    //
    // convert the mantissa
    //

    double integer = 0.0;

    if( int_digits != 0 )
    {

	bool first_digit = true;

	for( ; s < end; s ++ )
	{
	    if( *s == '.' )
		break;
	    
	    if( first_digit )
	    {
		integer = scfx_to_digit( *s, numrep );
		switch( numrep )
		{
		    case SC_BIN:
		    case SC_OCT:
		    case SC_HEX:
		    {
			if( integer >= ( base >> 1 ) )
			    integer -= base;  // two's complement
			break;
		    }
		    default:
			;
		}
		first_digit = false;
	    }
            else
	    {
		integer *= base;
		integer += scfx_to_digit( *s, numrep );
	    }
	}
    }

    // [ . fraction ]

    double fraction = 0.0;
    
    if( frac_digits != 0 )
    {
	s ++;  // skip '.'

	bool first_digit = ( int_digits == 0 );

	double scale = 1.0;

	for( ; s < end; s ++ )
	{
	    scale /= base;
	    
	    if( first_digit )
	    {
		fraction = scfx_to_digit( *s, numrep );
		switch( numrep )
		{
		    case SC_BIN:
		    case SC_OCT:
		    case SC_HEX:
		    {
			if( fraction >= ( base >> 1 ) )
			    fraction -= base;  // two's complement
			break;
		    }
		    default:
			;
		}
		fraction *= scale;
		first_digit = false;
	    }
	    else
		fraction += scfx_to_digit( *s, numrep ) * scale;
	}
    }

    double exp = ( exponent != 0 ) ? pow( (double) base, (double) exponent )
	                           : 1;

    return ( sign * ( integer + fraction ) * exp );
}

#undef _SCFX_FAIL_IF


static
void
convert_dec( scfx_ieee_double id, double& mant, int& exp )
{
    // extract binary mantissa and exponent

    int exp_bin = id.exponent();
    id.exponent( 0 );
    double mant_bin = static_cast<double>( id );

    // compute decimal exponent and mantissa

    double scale_bin = scfx_pow2( exp_bin );
    double scale_dec = 1.0;
    int exp_dec = 0;
    if( scale_bin > 1.0 )
    {
	while( scale_dec * 10.0 < scale_bin )
	{
	    scale_dec *= 10.0;
	    exp_dec += 1;
	}
    }
    else if( scale_bin < 1.0 )
    {
	while( scale_dec > scale_bin )
	{
	    scale_dec /= 10.0;
	    exp_dec -= 1;
	}
    }

    exp = exp_dec;
    mant = mant_bin * ( scale_bin / scale_dec );
    if( mant >= 10.0 )
    {
	mant /= 10.0;
	exp += 1;
    }
}


static
void
print_dec( scfx_string& s, scfx_ieee_double id, sc_fmt fmt )
{
    if( id.negative() != 0 )
    {
	id.negative( 0 );
	s += '-';
    }

    if( id.is_zero() )
    {
	s += '0';
	return;
    }

    double mant;
    int exp;
    convert_dec( id, mant, exp );

    if( ++ exp <= 0 )
    {
	s += '.';
	if( fmt == SC_F )
	{
	    for( ; exp < 0; ++ exp )
	        s += '0';
	}
    }

    int digit;

    digit = static_cast<int>( mant );
    mant -= static_cast<double>( digit );
    s += digit + '0';
    if( exp > 0 )
    {
        if( -- exp == 0 && mant != 0.0 )
	    s += '.';
    }

    while( mant != 0.0 )
    {
	mant *= 10.0;
	digit = static_cast<int>( mant );
	mant -= static_cast<double>( digit );
	s += digit + '0';
	if( exp > 0 )
	{
	    if( -- exp == 0 && mant != 0.0 )
	        s += '.';
	}
    }

    if( exp > 0 && fmt == SC_F )
    {
        for( ; exp > 0; -- exp )
	    s += '0';
    }

    scfx_print_exp( s, exp );
}


static
void
print_other( scfx_string& s, const scfx_ieee_double& id,
	     sc_numrep numrep, sc_fmt fmt, const scfx_params* params )
{
    scfx_ieee_double id2 = id;

    sc_numrep numrep2 = numrep;

    if( numrep == SC_BIN_SM ||
	numrep == SC_OCT_SM ||
	numrep == SC_HEX_SM )
    {
	if( id2.negative() != 0 )
	{
	    s += '-';
	    id2.negative( 0 );
	}
	switch( numrep )
	{
	    case SC_BIN_SM:
		numrep2 = SC_BIN_US;
		break;
	    case SC_OCT_SM:
		numrep2 = SC_OCT_US;
		break;
	    case SC_HEX_SM:
		numrep2 = SC_HEX_US;
		break;
	    default:
		;
	}
    }

    scfx_print_prefix( s, numrep );

    numrep = numrep2;

    sc_fxval_fast a( id2 );

    int msb, lsb;

    if( params != 0 )
    {
	msb = params->iwl() - 1;
	lsb = params->iwl() - params->wl();

	if( params->enc() == SC_TC &&
	    ( numrep == SC_BIN_US ||
	      numrep == SC_OCT_US ||
	      numrep == SC_HEX_US ) &&
	    params->wl() > 1 )
	    -- msb;
    }
    else
    {
	if( a.is_zero() )
	{
	    msb = 0;
	    lsb = 0;
	}
	else
	{
	    msb = id2.exponent() + 1;
	    while( a.get_bit( msb ) == a.get_bit( msb - 1 ) )
		-- msb;

	    if( numrep == SC_BIN_US ||
		numrep == SC_OCT_US ||
		numrep == SC_HEX_US )
		-- msb;

	    lsb = id2.exponent() - 52;
	    while( ! a.get_bit( lsb ) )
		++ lsb;
	}
    }

    int step;

    switch( numrep )
    {
	case SC_BIN:
	case SC_BIN_US:
	case SC_CSD:
	    step = 1;
	   break;
	case SC_OCT:
	case SC_OCT_US:
	    step = 3;
	    break;
	case SC_HEX:
	case SC_HEX_US:
	    step = 4;
	    break;
	default:
	    step = 0;
    }

    msb = (int) ceil( double( msb + 1 ) / step ) * step - 1;

    lsb = (int) floor( double( lsb ) / step ) * step;

    if( msb < 0 )
    {
	s += '.';
	if( fmt == SC_F )
	{
	    int sign = ( id2.negative() != 0 ) ? ( 1 << step ) - 1 : 0;
	    for( int i = ( msb + 1 ) / step; i < 0; i ++ )
	    {
		if( sign < 10 )
		    s += sign + '0';
		else
		    s += sign + 'a' - 10;
	    }
	}
    }

    int i = msb;
    while( i >= lsb )
    {
        int value = 0;
        for( int j = step - 1; j >= 0; -- j )
	{
            value += static_cast<int>( a.get_bit( i ) ) << j;
            -- i;
        }
        if( value < 10 )
            s += value + '0';
	else
            s += value + 'a' - 10;
	if( i == -1 )
	    s += '.';
    }

    if( lsb > 0 && fmt == SC_F )
    {
	for( int i = lsb / step; i > 0; i -- )
	    s += '0';
    }

    if( s[s.length() - 1] == '.' )
	s.discard( 1 );

    if( fmt != SC_F )
    {
	if( msb < 0 )
	    scfx_print_exp( s, ( msb + 1 ) / step );
	else if( lsb > 0 )
	    scfx_print_exp( s, lsb / step );
    }

    if( numrep == SC_CSD )
	scfx_tc2csd( s );
}


static
const char*
to_string( const scfx_ieee_double& id, sc_numrep numrep, sc_fmt fmt,
	   const scfx_params* params = 0 )
{
    static scfx_string s;

    s.clear();

    if( id.is_nan() )
        scfx_print_nan( s );
    else if( id.is_inf() )
        scfx_print_inf( s, id.negative() );
    else if( id.negative() && ! id.is_zero() &&
	     ( numrep == SC_BIN_US ||
	       numrep == SC_OCT_US ||
	       numrep == SC_HEX_US ) )
        s += "negative";
    else if( numrep == SC_DEC )
        ::print_dec( s, id, fmt );
    else
        ::print_other( s, id, numrep, fmt, params );

    return s;
}


// public constructors

sc_fxval_fast::sc_fxval_fast( const char* a )
: _val( ::from_string( a ) )
{}

sc_fxval_fast::sc_fxval_fast( const sc_int_base& a )
: _val( static_cast<double>( a.num ) )
{}

sc_fxval_fast::sc_fxval_fast( const sc_uint_base& a )
#if defined( _MSC_VER ) && ! defined( _32BIT_ )
: _val( static_cast<double>( static_cast<int64>( a.num ) ) )
#else
: _val( static_cast<double>( a.num ) )
#endif
{}

sc_fxval_fast::sc_fxval_fast( const sc_fxnum& a )
: _val( a.to_double() )
{}

sc_fxval_fast::sc_fxval_fast( const sc_fxnum_fast& a )
: _val( a.to_double() )
{}


// binary operators

#if defined( _MSC_VER ) && ! defined( _32BIT_ )

#define DEFN_BIN_OP_T_B(op,tp)                                                \
const sc_fxval_fast                                                           \
operator op ( const sc_fxval_fast& a, tp b )                                  \
{                                                                             \
    return sc_fxval_fast( a._val op static_cast<double>( static_cast<int64>( b.num ) ) ); \
}                                                                             \
                                                                              \
const sc_fxval_fast                                                           \
operator op ( tp a, const sc_fxval_fast& b )                                  \
{                                                                             \
    return sc_fxval_fast( static_cast<double>( static_cast<int64>( a.num ) ) op b._val ); \
}

#else

#define DEFN_BIN_OP_T_B(op,tp)                                                \
const sc_fxval_fast                                                           \
operator op ( const sc_fxval_fast& a, tp b )                                  \
{                                                                             \
    return sc_fxval_fast( a._val op static_cast<double>( b.num ) );           \
}                                                                             \
                                                                              \
const sc_fxval_fast                                                           \
operator op ( tp a, const sc_fxval_fast& b )                                  \
{                                                                             \
    return sc_fxval_fast( static_cast<double>( a.num ) op b._val );           \
}

#endif

#define DEFN_BIN_OP(op)                                                       \
DEFN_BIN_OP_T_B(op,const sc_int_base&)                                        \
DEFN_BIN_OP_T_B(op,const sc_uint_base&)

DEFN_BIN_OP(*)
DEFN_BIN_OP(+)
DEFN_BIN_OP(-)
// DEFN_BIN_OP(/) // BCB produces illegal symbol
DEFN_BIN_OP_T_B(/,const sc_int_base&)
DEFN_BIN_OP_T_B(/,const sc_uint_base&)

#undef DEFN_BIN_OP_T_B
#undef DEFN_BIN_OP


// binary functions

#if defined( _MSC_VER ) && ! defined( _32BIT_ )

#define DEFN_BIN_FNC_T_B(fnc,op,tp)                                           \
void                                                                          \
fnc ( sc_fxval_fast& c, const sc_fxval_fast& a, tp b )                        \
{                                                                             \
    c._val = a._val op static_cast<double>( static_cast<int64>( b.num ) );    \
}                                                                             \
                                                                              \
void                                                                          \
fnc ( sc_fxval_fast& c, tp a, const sc_fxval_fast& b )                        \
{                                                                             \
    c._val = static_cast<double>( static_cast<int64>( a.num ) ) op b._val;    \
}

#else

#define DEFN_BIN_FNC_T_B(fnc,op,tp)                                           \
void                                                                          \
fnc ( sc_fxval_fast& c, const sc_fxval_fast& a, tp b )                        \
{                                                                             \
    c._val = a._val op static_cast<double>( b.num );                          \
}                                                                             \
                                                                              \
void                                                                          \
fnc ( sc_fxval_fast& c, tp a, const sc_fxval_fast& b )                        \
{                                                                             \
    c._val = static_cast<double>( a.num ) op b._val;                          \
}

#endif

#define DEFN_BIN_FNC_T_E(fnc,op,tp)                                           \
void                                                                          \
fnc ( sc_fxval_fast& c, const sc_fxval_fast& a, tp b )                        \
{                                                                             \
    c._val = static_cast<double>( a op b );                                   \
}                                                                             \
                                                                              \
void                                                                          \
fnc ( sc_fxval_fast& c, tp a, const sc_fxval_fast& b )                        \
{                                                                             \
    c._val = static_cast<double>( a op b );                                   \
}

#define DEFN_BIN_FNC(fnc,op)                                                  \
DEFN_BIN_FNC_T_B(fnc,op,const sc_int_base&)                                   \
DEFN_BIN_FNC_T_B(fnc,op,const sc_uint_base&)                                  \
DEFN_BIN_FNC_T_E(fnc,op,const sc_fxval&)                                      \
DEFN_BIN_FNC_T_E(fnc,op,const sc_fxnum&)

DEFN_BIN_FNC(mult,*)
DEFN_BIN_FNC(div,/)
DEFN_BIN_FNC(add,+)
DEFN_BIN_FNC(sub,-)

#undef DEFN_BIN_FNC_T_B
#undef DEFN_BIN_FNC_T_E
#undef DEFN_BIN_FNC


// relational (including equality) operators

#if defined( _MSC_VER ) && ! defined( _32BIT_ )

#define DEFN_REL_OP_T_B(op,tp)                                                \
bool                                                                          \
operator op ( const sc_fxval_fast& a, tp b )                                  \
{                                                                             \
    return ( a._val op static_cast<double>( static_cast<int64>( b.num ) ) );  \
}                                                                             \
                                                                              \
bool                                                                          \
operator op ( tp a, const sc_fxval_fast& b )                                  \
{                                                                             \
    return ( static_cast<double>( static_cast<int64>( a.num ) ) op b._val );  \
}

#else

#define DEFN_REL_OP_T_B(op,tp)                                                \
bool                                                                          \
operator op ( const sc_fxval_fast& a, tp b )                                  \
{                                                                             \
    return ( a._val op static_cast<double>( b.num ) );                        \
}                                                                             \
                                                                              \
bool                                                                          \
operator op ( tp a, const sc_fxval_fast& b )                                  \
{                                                                             \
    return ( static_cast<double>( a.num ) op b._val );                        \
}

#endif

#define DEFN_REL_OP(op)                                                       \
DEFN_REL_OP_T_B(op,const sc_int_base&)                                        \
DEFN_REL_OP_T_B(op,const sc_uint_base&)

DEFN_REL_OP(<)
DEFN_REL_OP(<=)
DEFN_REL_OP(>)
DEFN_REL_OP(>=)
DEFN_REL_OP(==)
DEFN_REL_OP(!=)

#undef DEFN_REL_OP_T_B
#undef DEFN_REL_OP


// assignment operators

#if defined( _MSC_VER ) && ! defined( _32BIT_ )

#define DEFN_ASN_OP_T_B(op,tp)                                                \
sc_fxval_fast&                                                                \
sc_fxval_fast::operator op ( tp b )                                           \
{                                                                             \
    _val op static_cast<double>( static_cast<int64>( b.num ) );               \
    return *this;                                                             \
}

#else

#define DEFN_ASN_OP_T_B(op,tp)                                                \
sc_fxval_fast&                                                                \
sc_fxval_fast::operator op ( tp b )                                           \
{                                                                             \
    _val op static_cast<double>( b.num );                                     \
    return *this;                                                             \
}

#endif

#define DEFN_ASN_OP_T_C(op,tp)                                                \
sc_fxval_fast&                                                                \
sc_fxval_fast::operator op ( tp b )                                           \
{                                                                             \
    _val op b.to_double();                                                    \
    return *this;                                                             \
}

#define DEFN_ASN_OP(op)                                                       \
DEFN_ASN_OP_T_B(op,const sc_int_base&)                                        \
DEFN_ASN_OP_T_B(op,const sc_uint_base&)                                       \
DEFN_ASN_OP_T_C(op,const sc_fxnum&)                                           \
DEFN_ASN_OP_T_C(op,const sc_fxnum_fast&)

DEFN_ASN_OP(=)

DEFN_ASN_OP(*=)
DEFN_ASN_OP(/=)
DEFN_ASN_OP(+=)
DEFN_ASN_OP(-=)

#undef DEFN_ASN_OP_T_B
#undef DEFN_ASN_OP_T_C
#undef DEFN_ASN_OP


// explicit conversion to character string

const sc_string
sc_fxval_fast::to_string() const
{
    return ::to_string( _val, SC_DEC, SC_E );
}

const sc_string
sc_fxval_fast::to_string( sc_numrep numrep ) const
{
    return ::to_string( _val, numrep, SC_E );
}

const sc_string
sc_fxval_fast::to_string( sc_fmt fmt ) const
{
    return ::to_string( _val, SC_DEC, fmt );
}

const sc_string
sc_fxval_fast::to_string( sc_numrep numrep, sc_fmt fmt ) const
{
    return ::to_string( _val, numrep, fmt );
}


const sc_string
sc_fxval_fast::to_dec() const
{
    return ::to_string( _val, SC_DEC, SC_E );
}

const sc_string
sc_fxval_fast::to_bin() const
{
    return ::to_string( _val, SC_BIN, SC_E );
}

const sc_string
sc_fxval_fast::to_oct() const
{
    return ::to_string( _val, SC_OCT, SC_E );
}

const sc_string
sc_fxval_fast::to_hex() const
{
    return ::to_string( _val, SC_HEX, SC_E );
}


// print or dump content

void
sc_fxval_fast::print( ostream& os ) const
{
    os << ::to_string( _val, SC_DEC, SC_E );
}

void
sc_fxval_fast::dump( ostream& os ) const
{
    os << "sc_fxval_fast" << endl;
    os << "(" << endl;
    os << "val = " << _val << endl;
    // TO BE COMPLETED
    // os << "r_flag   = " << _r_flag << endl;
    // os << "observer = ";
    // if( _observer != 0 )
    //     _observer->dump( os );
    // else
    //     os << "0" << endl;
    os << ")" << endl;
}


// ...

bool
sc_fxval_fast::get_bit( int i ) const
{
    scfx_ieee_double id( _val );
    if( id.is_zero() || id.is_nan() || id.is_inf() )
        return false;

    // convert to two's complement

    unsigned int m0 = id.mantissa0();
    unsigned int m1 = id.mantissa1();

    if( id.is_normal() )
        m0 += 1U << 20;

    if( id.negative() != 0 )
    {
	m0 = ~ m0;
	m1 = ~ m1;
	unsigned int tmp = m1;
	m1 += 1U;
	if( m1 <= tmp )
	    m0 += 1U;
    }

    // get the right bit

    int j = i - id.exponent();
    if( ( j += 20 ) >= 32 )
        return ( ( m0 & 1U << 31 ) != 0 );
    else if( j >= 0 )
        return ( ( m0 & 1U << j ) != 0 );
    else if( ( j += 32 ) >= 0 )
        return ( ( m1 & 1U << j ) != 0 );
    else
        return false;
}


// protected methods and friend functions

static
void
quantization( sc_fxval_fast& c, const scfx_params& params, bool& q_flag )
{
    int fwl = params.wl() - params.iwl();
    double scale = scfx_pow2( fwl );
    double val = scale * c.to_double();
    double int_part;
    double frac_part = modf( val, &int_part );

    q_flag = ( frac_part != 0.0 );

    if( q_flag )
    {
        val = int_part;

	switch( params.q_mode() )
	{
            case SC_TRN:			// truncation
	    {
	        if( val < 0.0 )
		    val -= 1.0;
		break;
	    }
            case SC_RND:			// rounding to plus infinity
	    {
		if( val >= 0.0 )
		{
		    if( frac_part >= 0.5 )
			val += 1.0;
		}
		else
		{
		    if( frac_part < -0.5 )
			val -= 1.0;
		}
		break;
	    }
            case SC_TRN_ZERO:			// truncation to zero
	    {
	        break;
	    }
            case SC_RND_INF:			// rounding to infinity
	    {
	        if( val >= 0.0 )
		{
		    if( frac_part >= 0.5 )
		        val += 1.0;
		}
		else
		{
		    if( frac_part <= -0.5 )
		        val -= 1.0;
		}
		break;
	    }
            case SC_RND_CONV:			// convergent rounding
	    {
	        if( val >= 0.0 )
		{
		    if( frac_part > 0.5 ||
			frac_part == 0.5 && fmod( int_part, 2.0 ) != 0.0 )
		        val += 1.0;
		}
		else
		{
		    if( frac_part < -0.5 ||
			frac_part == -0.5 && fmod( int_part, 2.0 ) != 0.0 )
		        val -= 1.0;
		}
		break;
	    }
            case SC_RND_ZERO:			// rounding to zero
	    {
	        if( val >= 0.0 )
		{
		    if( frac_part > 0.5 )
		        val += 1.0;
		}
		else
		{
		    if( frac_part < -0.5 )
		        val -= 1.0;
		}
		break;
	    }
            case SC_RND_MIN_INF:		// rounding to minus infinity
	    {
	        if( val >= 0.0 )
		{
		    if( frac_part > 0.5 )
		        val += 1.0;
		}
		else
		{
		    if( frac_part <= -0.5 )
		        val -= 1.0;
		}
		break;
	    }
            default:
	        ;
	}
    }

    val /= scale;
    c = val;
}

static
void
overflow( sc_fxval_fast& c, const scfx_params& params, bool& o_flag )
{
    int iwl = params.iwl();
    int fwl = params.wl() - iwl;
    double full_circle = scfx_pow2( iwl );
    double resolution = scfx_pow2( -fwl );
    double low, high;
    if( params.enc() == SC_TC )
    {
	high = full_circle / 2.0 - resolution;
	if( params.o_mode() == SC_SAT_SYM )
	    low = - high;
	else
	    low = - full_circle / 2.0;
    }
    else
    {
	low = 0.0;
	high = full_circle - resolution;
    }
    double val = c.to_double();

    bool under = ( val < low );
    bool over = ( val > high );

    o_flag = ( under || over );

    if( o_flag )
    {
        switch( params.o_mode() )
	{
            case SC_WRAP:			// wrap-around
	    {
		int n_bits = params.n_bits();

	        if( n_bits == 0 )
		{
		    // wrap-around all 'wl' bits
		    val -= floor( val / full_circle ) * full_circle;
		    if( val > high )
			val -= full_circle;
		}
		else if( n_bits < params.wl() )
		{
		    double X = scfx_pow2( iwl - n_bits );

		    // wrap-around least significant 'wl - n_bits' bits
		    val -= floor( val / X ) * X;
		    if( val > ( X - resolution ) )
			val -= X;
		    
		    // saturate most significant 'n_bits' bits
		    if( under )
		        val += low;
		    else
		    {
		        if( params.enc() == SC_TC )
			    val += full_circle / 2.0 - X;
			else
			    val += full_circle - X;
		    }
		}
		else
		{
		    // saturate all 'wl' bits
		    if( under )
			val = low;
		    else
			val = high;
		}
		break;
	    }
            case SC_SAT:			// saturation
            case SC_SAT_SYM:			// symmetrical saturation
	    {
	        if( under )
		    val = low;
		else
		    val = high;
		break;
	    }
            case SC_SAT_ZERO:			// saturation to zero
	    {
	        val = 0.0;
		break;
	    }
            case SC_WRAP_SM:			// sign magnitude wrap-around
	    {
		_SC_ASSERT( params.enc() == SC_TC,
			    "SC_WRAP_SM not defined for unsigned numbers" );
	
		int n_bits = params.n_bits();

		if( n_bits == 0 )
		{
		    // invert conditionally
		    if( c.get_bit( iwl ) != c.get_bit( iwl - 1 ) )
			val = -val - resolution;

		    // wrap-around all 'wl' bits
		    val -= floor( val / full_circle ) * full_circle;
		    if( val > high )
			val -= full_circle;
		}
		else if( n_bits == 1 )
		{
		    // invert conditionally
		    if( c.is_neg() != c.get_bit( iwl - 1 ) )
			val = -val - resolution;

		    // wrap-around all 'wl' bits
		    val -= floor( val / full_circle ) * full_circle;
		    if( val > high )
			val -= full_circle;
		}
		else if( n_bits < params.wl() )
		{
		    // invert conditionally
		    if( c.is_neg() == c.get_bit( iwl - n_bits ) )
			val = -val - resolution;
		    
		    double X = scfx_pow2( iwl - n_bits );

		    // wrap-around least significant 'wl - n_bits' bits
		    val -= floor( val / X ) * X;
		    if( val > ( X - resolution ) )
			val -= X;

		    // saturate most significant 'n_bits' bits
		    if( under )
		        val += low;
		    else
			val += full_circle / 2.0 - X;
		}
		else
		{
		    // saturate all 'wl' bits
		    if( under )
			val = low;
		    else
			val = high;
		}
	        break;
	    }
            default:
	        ;
	}

	c = val;
    }
}


void
sc_fxval_fast::cast( const scfx_params& params, bool& q_flag, bool& o_flag )
{
    q_flag = false;
    o_flag = false;

    // check for special cases

    scfx_ieee_double id( _val );
    if( id.is_zero() )
    {
	if( id.negative() != 0 )
	    _val = -_val;
	return;
    }

    // perform casting

    ::quantization( *this, params, q_flag );
    ::overflow( *this, params, o_flag );

    // check for special case: -0

    id = _val;
    if( id.is_zero() && id.negative() != 0 )
	_val = -_val;
}


const char*
sc_fxval_fast::to_string( sc_numrep numrep, sc_fmt fmt,
			  const scfx_params& params ) const
{
    return ::to_string( _val, numrep, fmt, &params );
}


const sc_fxval_fast
sc_fxval_fast::b_not( const scfx_params& params ) const
{
    scfx_ieee_double id_a( _val );
    
    if( id_a.is_nan() || id_a.is_inf() )
	return static_cast<double>( scfx_ieee_double::nan() );

    sc_fxval_fast c;
    int msb = params.iwl() - 1;
    int lsb = params.iwl() - static_cast<int>( params.wl() );
    for( int i = msb; i >= lsb; -- i )
    {
        if( ! get_bit( i ) )
	    c.set( i, params );
    }
    return c;
}

const sc_fxval_fast
sc_fxval_fast::b_and( const sc_fxval_fast& b, const scfx_params& params ) const
{
    scfx_ieee_double id_a( _val );
    scfx_ieee_double id_b( b._val );

    if( id_a.is_nan() || id_b.is_nan() ||
	id_a.is_inf() || id_b.is_inf() )
	return static_cast<double>( scfx_ieee_double::nan() );

    sc_fxval_fast c;
    int msb = params.iwl() - 1;
    int lsb = params.iwl() - static_cast<int>( params.wl() );
    for( int i = msb; i >= lsb; -- i )
    {
        if( get_bit( i ) && b.get_bit( i ) )
	    c.set( i, params );
    }
    return c;
}

const sc_fxval_fast
sc_fxval_fast::b_xor( const sc_fxval_fast& b, const scfx_params& params ) const
{
    scfx_ieee_double id_a( _val );
    scfx_ieee_double id_b( b._val );

    if( id_a.is_nan() || id_b.is_nan() ||
	id_a.is_inf() || id_b.is_inf() )
	return static_cast<double>( scfx_ieee_double::nan() );

    sc_fxval_fast c;
    int msb = params.iwl() - 1;
    int lsb = params.iwl() - static_cast<int>( params.wl() );
    for( int i = msb; i >= lsb; -- i )
    {
        if( get_bit( i ) != b.get_bit( i ) )
	    c.set( i, params );
    }
    return c;
}

const sc_fxval_fast
sc_fxval_fast::b_or( const sc_fxval_fast& b, const scfx_params& params ) const
{
    scfx_ieee_double id_a( _val );
    scfx_ieee_double id_b( b._val );

    if( id_a.is_nan() || id_b.is_nan() ||
	id_a.is_inf() || id_b.is_inf() )
	return static_cast<double>( scfx_ieee_double::nan() );

    sc_fxval_fast c;
    int msb = params.iwl() - 1;
    int lsb = params.iwl() - static_cast<int>( params.wl() );
    for( int i = msb; i >= lsb; -- i )
    {
        if( get_bit( i ) || b.get_bit( i ) )
	    c.set( i, params );
    }
    return c;
}


bool
sc_fxval_fast::set( int i, const scfx_params& params )
{
    scfx_ieee_double id( _val );
    if( id.is_nan() || id.is_inf() )
        return false;

    if( get_bit( i ) )
        return true;

    if( params.enc() == SC_TC && i == params.iwl() - 1 )
        _val -= scfx_pow2( i );
    else
        _val += scfx_pow2( i );

    return true;
}

bool
sc_fxval_fast::clear( int i, const scfx_params& params )
{
    scfx_ieee_double id( _val );
    if( id.is_nan() || id.is_inf() )
        return false;

    if( ! get_bit( i ) )
        return true;

    if( params.enc() == SC_TC && i == params.iwl() - 1 )
        _val += scfx_pow2( i );
    else
        _val -= scfx_pow2( i );

    return true;
}


bool
sc_fxval_fast::get_slice( int i, int j, const scfx_params&,
			  sc_bv_base& bv ) const
{
    scfx_ieee_double id( _val );
    if( id.is_nan() || id.is_inf() )
	return false;

    // convert to two's complement

    unsigned int m0 = id.mantissa0();
    unsigned int m1 = id.mantissa1();

    if( id.is_normal() )
        m0 += 1U << 20;

    if( id.negative() != 0 )
    {
	m0 = ~ m0;
	m1 = ~ m1;
	unsigned int tmp = m1;
	m1 += 1U;
	if( m1 <= tmp )
	    m0 += 1U;
    }

    // get the bits

    int l = j;
    for( int k = 0; k < static_cast<int>( bv.length() ); ++ k )
    {
	bool b = false;

        int n = l - id.exponent();
        if( ( n += 20 ) >= 32 )
	    b = ( ( m0 & 1U << 31 ) != 0 );
	else if( n >= 0 )
	    b = ( ( m0 & 1U << n ) != 0 );
	else if( ( n += 32 ) >= 0 )
	    b = ( ( m1 & 1U << n ) != 0 );

	bv[k] = b;

	if( i >= j )
	    ++ l;
	else
	    -- l;
    }

    return true;
}

bool
sc_fxval_fast::set_slice( int i, int j, const scfx_params& params,
			  const sc_bv_base& bv )
{
    scfx_ieee_double id( _val );
    if( id.is_nan() || id.is_inf() )
        return false;

    // set the bits

    int l = j;
    for( int k = 0; k < static_cast<int>( bv.length() ); ++ k )
    {
	if( bv[k] )
	{
	    if( ! get_bit( l ) )
	    {
		if( params.enc() == SC_TC && l == params.iwl() - 1 )
		    _val -= scfx_pow2( l );
		else
		    _val += scfx_pow2( l );
	    }
	}
	else
	{
	    if( get_bit( l ) )
	    {
		if( params.enc() == SC_TC && l == params.iwl() - 1 )
		    _val += scfx_pow2( l );
		else
		    _val -= scfx_pow2( l );
	    }
	}


	if( i >= j )
	    ++ l;
	else
	    -- l;
    }

    return true;
}


// Taf!
