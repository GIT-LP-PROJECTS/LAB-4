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

    scfx_otherdefs.cpp - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include "sc_fxnum.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_signed
// ----------------------------------------------------------------------------

// assignment operators

sc_signed&
sc_signed::operator = ( const sc_fxval& v )
{
    if( v.is_nan() || v.is_inf() )
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point value argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}

sc_signed&
sc_signed::operator = ( const sc_fxval_fast& v )
{
    if( v.is_nan() || v.is_inf() )
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point value argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}

sc_signed&
sc_signed::operator = ( const sc_fxnum& v )
{
    if( v.value().is_nan() || v.value().is_inf() )
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.value().get_bit( i );

    return *this;
}

sc_signed&
sc_signed::operator = ( const sc_fxnum_fast& v )
{
    if( v.value().is_nan() || v.value().is_inf() )
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.value().get_bit( i );

    return *this;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned
// ----------------------------------------------------------------------------

// assignment operators

sc_unsigned&
sc_unsigned::operator = ( const sc_fxval& v )
{
    if( v.is_nan() || v.is_inf() )
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point value argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}

sc_unsigned&
sc_unsigned::operator = ( const sc_fxval_fast& v )
{
    if( v.is_nan() || v.is_inf() )
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point value argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}

sc_unsigned&
sc_unsigned::operator = ( const sc_fxnum& v )
{
    if( v.value().is_nan() || v.value().is_inf() )
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.value().get_bit( i );

    return *this;
}

sc_unsigned&
sc_unsigned::operator = ( const sc_fxnum_fast& v )
{
    if( v.value().is_nan() || v.value().is_inf() )
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.value().get_bit( i );

    return *this;
}


// Taf!
