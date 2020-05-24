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

    sc_nbassign.cpp -- Definition of the interface between
    sc_signed/sc_unsigned and sc_logic_vector/sc_bool_vector.

    Original Author: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#define SC_NUMERIC_BIT_IMPLEMENTATION
#ifndef _MSC_VER
#include <iostream>
using std::ostream;
using std::istream;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::hex;
#else
// MSVC6.0 has bugs in standard library
#include <iostream.h>
#endif
#ifdef __BCPLUSPLUS__
#pragma hdrstop
#endif
#include <assert.h>

#include "sc_bool_vector.h"
#include "sc_logic_vector.h"
#include "sc_signed.h"
#include "sc_unsigned.h"


sc_signed::sc_signed(const sc_logic_vector& v)
{

  nbits = v.length();
  ndigits = DIV_CEIL(nbits);

#ifdef MAX_NBITS
  test_bound(nbits);
#else
  digit = new digit_type[ndigits];
#endif

  for (register length_type i = nbits - 1; i >= 0; --i)
    safe_set(i, v.get_data_ptr(i)->to_bool(), digit);

  convert_2C_to_SM();

}


sc_signed::sc_signed(const sc_bool_vector& v)
{

  nbits = v.length();
  ndigits = DIV_CEIL(nbits);

#ifdef MAX_NBITS
  test_bound(nbits);
#else
  digit = new digit_type[ndigits];
#endif

  for (register length_type i = nbits - 1; i >= 0; --i)
    safe_set(i, *(v.get_data_ptr(i)), digit);

  convert_2C_to_SM();

}


sc_signed&
sc_signed::operator=(const sc_logic_vector& v)
{

  length_type nb = MINT(nbits, v.length());

  vec_zero(DIV_CEIL(nb), ndigits, digit);

  for (register length_type i = nb - 1; i >= 0; --i)
    safe_set(i, v.get_data_ptr(i)->to_bool(), digit);

  convert_2C_to_SM();

  return *this;

}


sc_signed&
sc_signed::operator=(const sc_bool_vector& v)
{

  length_type nb = MINT(nbits, v.length());

  vec_zero(DIV_CEIL(nb), ndigits, digit);

  for (register length_type i = nb - 1; i >= 0; --i)
    safe_set(i, *(v.get_data_ptr(i)), digit);

  convert_2C_to_SM();

  return *this;

}


sc_unsigned::sc_unsigned(const sc_logic_vector& v)
{

  sgn = SC_POS;
  nbits = v.length() + 1;
  ndigits = DIV_CEIL(nbits);

#ifdef MAX_NBITS
  test_bound(nbits - 1);
#else
  digit = new digit_type[ndigits];
#endif

  digit[ndigits - 1] = 0;

  for (register length_type i = nbits - 2; i >= 0; --i)
    safe_set(i, v.get_data_ptr(i)->to_bool(), digit);

}


sc_unsigned::sc_unsigned(const sc_bool_vector& v)
{

  sgn = SC_POS;
  nbits = v.length() + 1;
  ndigits = DIV_CEIL(nbits);

#ifdef MAX_NBITS
  test_bound(nbits - 1);
#else
  digit = new digit_type[ndigits];
#endif

  digit[ndigits - 1] = 0;
  
  for (register length_type i = nbits - 2; i >= 0; --i)
    safe_set(i, *(v.get_data_ptr(i)), digit);

}


sc_unsigned&
sc_unsigned::operator=(const sc_logic_vector& v)
{

  length_type nb = MINT(nbits, v.length());

  vec_zero(DIV_CEIL(nb), ndigits, digit);

  for (register length_type i = nb - 1; i >= 0; --i)
    safe_set(i, v.get_data_ptr(i)->to_bool(), digit);

  convert_2C_to_SM();

  return *this;

}

sc_unsigned&
sc_unsigned::operator=(const sc_bool_vector& v)
{

  length_type nb = MINT(nbits, v.length());

  vec_zero(DIV_CEIL(nb), ndigits, digit);

  for (register length_type i = nb - 1; i >= 0; --i)
    safe_set(i, *(v.get_data_ptr(i)), digit);

  convert_2C_to_SM();

  return *this;

}


sc_bool_vector&
sc_bool_vector::operator=(const sc_unsigned& v)
{

    int len = length();
    int minlen = MINT(len, v.length());

    int i = 0;

    for ( ; i < minlen; ++i)
        assign_help(i, v.test(i));

    for ( ; i < len; ++i)
        assign_help(i, 0);  /* zero-extend */

    return *this;

}


sc_bool_vector&
sc_bool_vector::operator=(const sc_signed& v)
{

    int len = length();
    int minlen = MINT(len, v.length());

    int i = 0;

    for ( ; i < minlen; ++i)
        assign_help(i, v.test(i));

    bit sgn = v.sign();

    for ( ; i < len; ++i)
        assign_help(i, sgn); /* sign-extend */

    return *this;

}


sc_logic_vector&
sc_logic_vector::operator=(const sc_unsigned& v)
{

    int len = length();
    int minlen = MINT(len, v.length());

    int i = 0;

    for ( ; i < minlen; ++i)
        assign_help(i, char(v.test(i)));

    for ( ; i < len; ++i)
        assign_help(i, '0'); /* zero-extend */

    return *this;

}

sc_logic_vector&
sc_logic_vector::operator=(const sc_signed& v)
{
    int len = length();
    int minlen = MINT(len, v.length());

    int i = 0;

    for ( ; i < minlen; ++i)
        assign_help(i, char(v.test(i)));

    bit sgn = v.sign();

    for ( ; i < len; ++i)
        assign_help(i, char(sgn)); /* sign-extend */

    return *this;

}

// End of file
