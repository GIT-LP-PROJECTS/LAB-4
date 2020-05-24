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

    sc_fxnum_bitref.cpp - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include "sc_fxnum_bitref.h"

#include "sc_fxnum.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_bitref
//
//  Proxy class for bit-selection in class sc_fxnum, behaves like sc_bit.
// ----------------------------------------------------------------------------

bool
sc_fxnum_bitref::get() const
{
    return _num._val.get_bit( _idx );
}

void
sc_fxnum_bitref::set( bool high )
{
    _num._val.set_bit( _idx, _num._params, high );
}


// print or dump content

void
sc_fxnum_bitref::print( ostream& os ) const
{
    if( get() )
	os << "1";
    else
	os << "0";
}

void
sc_fxnum_bitref::dump( ostream& os ) const
{
    os << "sc_fxnum_bitref" << endl;
    os << "(" << endl;
    os << "num = ";
    _num.dump( os );
    os << "idx = " << _idx << endl;
    os << ")" << endl;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast_bitref
//
//  Proxy class for bit-selection in class sc_fxnum_fast, behaves like sc_bit.
// ----------------------------------------------------------------------------

bool
sc_fxnum_fast_bitref::get() const
{
    return _num._val.get_bit( _idx );
}

void
sc_fxnum_fast_bitref::set( bool high )
{
    _num._val.set_bit( _idx, _num._params, high );
}


// print or dump content

void
sc_fxnum_fast_bitref::print( ostream& os ) const
{
    if( get() )
	os << "1";
    else
	os << "0";
}

void
sc_fxnum_fast_bitref::dump( ostream& os ) const
{
    os << "sc_fxnum_fast_bitref" << endl;
    os << "(" << endl;
    os << "num = ";
    _num.dump( os );
    os << "idx = " << _idx << endl;
    os << ")" << endl;
}


// Taf!
