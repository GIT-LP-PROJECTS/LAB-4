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

    sc_err_handler.h -- Provide flexible error reporting / user debugging

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_ERR_HANDLER_H
#define SC_ERR_HANDLER_H

#ifndef _MSC_VER
#include <iostream>
using std::ostream;
using std::istream;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
#else
// MSVC6.0 has bugs in standard library
#include <iostream.h>
#endif

extern ostream& sc_err;

enum sc_err_enum {
    SC_ERR_ARRAY_BOUND,
    SC_ERR_ARRAY_LENGTH,
    SC_ERR_PARAMETER,
    SC_ERR_LAMBDA,
    SC_ERR_SC_LOGIC,
    SC_ERR_WAIT,
    SC_ERR_WATCHING
};

typedef void (*sc_err_handler)(sc_err_enum);

extern sc_err_handler sc_set_handler(sc_err_enum, sc_err_handler);
extern void sc_call_handler(sc_err_enum, const char*, int sev = -1);

#endif
