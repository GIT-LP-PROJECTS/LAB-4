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

/*****************************************************************************

    sc_ver.cpp -- version and copyright information

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

*****************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    
******************************************************************************/


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

static const char copyright[] = "                   Copyright (c) 1988-2000 by Synopsys, Inc.";
static const char systemc_version[] = "\
               SystemC (TM) Version 1.0  --- " __DATE__ " " __TIME__ "\n\
                                  ALL RIGHTS RESERVED";

const char*
sc_copyright()
{
    return copyright;
}

const char*
sc_version()
{
    return systemc_version;
}
