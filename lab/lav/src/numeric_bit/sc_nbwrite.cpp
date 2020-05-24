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

    sc_nbwrite.cpp - Definition of the interface between
    sc_signed/sc_unsigned and sc_logic_vector/sc_bool_vector.

    Original Authors: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)
                      Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)

*****************************************************************************/


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
#include <assert.h>

#include "sc_bool_vector.h"
#include "sc_logic_vector.h"
#include "sc_signed.h"
#include "sc_unsigned.h"


sc_signal_bool_vector&
sc_signal_bool_vector::write(const sc_unsigned& nv)
{

    int len = length();
    int minlen = MINT(len, nv.length());

    int i = 0;

    for ( ; i < minlen; ++i)
        write_help(i, nv.test(i));

    for ( ; i < len; ++i)
        write_help(i, char(0)); /* zero-extend */

    return *this;

}


sc_signal_bool_vector&
sc_signal_bool_vector::write(const sc_signed& nv)
{

    int len = length();
    int minlen = MINT(len, nv.length());

    int i = 0;
    for ( ; i < minlen; ++i)
        write_help(i, nv.test(i));

    bit sgn = nv.sign();

    for ( ; i < len; ++i)
        write_help(i, char(sgn)); /* sign-extend */

    return *this;

}


sc_signal_logic_vector&
sc_signal_logic_vector::write(const sc_unsigned& nv)
{

    int len = length();
    int minlen = MINT(len, nv.length());

    int i = 0;
    for ( ; i < minlen; ++i)
        write_help(i, char(nv.test(i)));

    for ( ; i < len; ++i)
        write_help(i, char(0)); /* zero-extend */

    return *this;

}


sc_signal_logic_vector&
sc_signal_logic_vector::write(const sc_signed& nv)
{

    int len = length();
    int minlen = MINT(len, nv.length());

    int i = 0;
    for ( ; i < minlen; ++i)
        write_help(i, char(nv.test(i)));

    bit sgn = nv.sign();

    for ( ; i < len; ++i)
        write_help(i, char(sgn)); /* sign-extend */

    return *this;

}

// End of file
