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

    sc_logic.cpp -- C++ implementation of logic type.  Behaves
                     pretty much the same way as HDLs logic type.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include <stdio.h>
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

#include "sc_logic.h"
#include "sc_logic_macros.h_"
//#include "sc_err_handler.h"
#include "sc_exception.h"

static bool check_01_table[Nsl] = { false, true, true, false };

void
sc_logic::check_01() const
{
    if (! check_01_table[val]) {
        char msg[80];
        sprintf( msg, "to_bool() called on sc_logic `%c'", to_char() );
        //sc_call_handler(SC_ERR_SC_LOGIC, msg);
        throw sc_edata(9005,msg);
    }
}

bool
sc_logic::is_01() const
{
    return check_01_table[val];
}

void
sc_logic::print(ostream& os) const
{
    os << to_char();
}

/*
 * Translation from char---note the following:
 *
 * (a) both '\000' and '0' are translated to logical ZERO
 * (b) both '\001' and '1' are translated to logical ONE
 * (c) case insensitive when translating from char
 * (d) '-' and 'D' are translated to logical DON'T-CARE
 */
const sc_logic::sc_logic_enum
sc_logic::char_table[256] =
{
    O, l, Z, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    O, l, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, Z, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, Z, X, X, X, X, X,

    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, l, X, X, X, O, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, l, X, X, X, O, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, O, l,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, O, l
};

/* translation to char */
const char
sc_logic::to_char_table[Nsl] =
{
    'X', '0', '1', 'Z'
};

/* translation to char, using only '0', '1', 'X', and 'Z' */
const char
sc_logic::to_4value_table[Nsl] =
{
    'X', '0', '1', 'Z'
};

/* convert to boolean type */
const bool
sc_logic::to_bool_table[Nsl] =
{
    0, 0, 1, 0
};

// convert to long for lv<> compatibility
const long sc_logic::to_long_table[Nsl] ={3, 0, 1, 2};

/* Table for computing the AND of two sc_logic */
const sc_logic::sc_logic_enum
sc_logic::and_table[Nsl][Nsl] =
{
  //  X  O  l  Z
  //-------------
    { X, O, X, X }, // X
    { O, O, O, O }, // O
    { X, O, l, X }, // l
    { X, O, X, X }  // Z
};

/* Table for computing the OR of two sc_logic */
const sc_logic::sc_logic_enum
sc_logic::or_table[Nsl][Nsl] =
{
  //  X  O  l  Z
  //------------
    { X, X, l, X }, // X
    { X, O, l, X }, // O
    { l, l, l, l }, // l
    { X, X, l, X }  // Z
};

/* Table for computing the XOR of two sc_logic */
const sc_logic::sc_logic_enum
sc_logic::xor_table[Nsl][Nsl] =
{
  //  X  O  l  Z
  //---------------------------------
    { X, X, X, X }, // X
    { X, O, l, X }, // O
    { X, l, O, X }, // l
    { X, X, X, X }  // Z
};

/* Table for computing the NOT of an sc_logic */
const sc_logic::sc_logic_enum
sc_logic::not_table[Nsl] =
{
  //  X  O  l  Z
  //------------
      X, l, O, X
};

const sc_logic sc_logic_X('X');
const sc_logic sc_logic_0('0');
const sc_logic sc_logic_1('1');
const sc_logic sc_logic_Z('Z');

