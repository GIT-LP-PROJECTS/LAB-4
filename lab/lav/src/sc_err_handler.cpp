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

    sc_err_handler.cpp -- Provide flexible error reporting / user debugging

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include <stdlib.h>
#include <cstring>
#ifndef _MSC_VER
#include <fstream>
using std::ofstream;
using std::flush;
#else
#include <fstream.h>
#endif

#include "sc_err_handler.h"
#include "sc_simcontext.h"

static ostream&
prepare_sc_err()
{
	ostream* retval = &cerr; /* default sc_err to cerr */
    const char* filename = getenv("SYSTEMC_LOG");
    if (filename) {
        if (strcmp(filename,"stdout")==0 || strcmp(filename,"-")==0)
            retval = &cout;
        else if (strcmp(filename,"stderr") != 0) {
            static ofstream str(filename);
            retval = &str;
        }
    }
    return *retval;
}

ostream& sc_err = prepare_sc_err();

struct sc_err_struct {
    sc_err_enum    err_type;
    const char*    name;
    int            severity; /* 0 = info, 1 = warn, 2 = error, 3 = die */
    sc_err_handler handler;
};

static void sc_err_default_handler(sc_err_enum err_type);

sc_err_struct sc_err_table[] = {
    { SC_ERR_ARRAY_BOUND,  "SC_ERR_ARRAY_BOUND",  3, sc_err_default_handler },
    { SC_ERR_ARRAY_LENGTH, "SC_ERR_ARRAY_LENGTH", 3, sc_err_default_handler },
    { SC_ERR_PARAMETER,    "SC_ERR_PARAMETER",    1, sc_err_default_handler },
    { SC_ERR_LAMBDA,       "SC_ERR_LAMBDA",       1, sc_err_default_handler },
    { SC_ERR_SC_LOGIC,     "SC_ERR_SC_LOGIC",     1, sc_err_default_handler },
    { SC_ERR_WAIT,         "SC_ERR_WAIT",         3, sc_err_default_handler },
    { SC_ERR_WATCHING,     "SC_ERR_WATCHING",     3, sc_err_default_handler }
};

static void sc_err_default_handler(sc_err_enum)
{

}

sc_err_handler
sc_set_handler(sc_err_enum err_type, sc_err_handler handler)
{
    sc_err_handler old_handler = sc_err_table[err_type].handler;
    sc_err_table[err_type].handler = handler;
    return old_handler;
}

void
sc_call_handler(sc_err_enum err_type, const char* msg, int sev )
{
    sc_simcontext* context = sc_get_curr_simcontext();
    double tm = context ? context->time_stamp() : 0.0;
    const char* errlevel[] = { "info", "warning", "error", "fatal" };
    sc_err << "SystemC " << errlevel[sc_err_table[err_type].severity];
    sc_err << " [" << sc_err_table[err_type].name << "] at time ";
    sc_err << tm << ": " << msg << endl << flush;

    /* Call the error handler */
    sc_err_table[err_type].handler(err_type);
    if (sev < 0) sev = sc_err_table[err_type].severity;
    if (sev > 2)
        abort();
}
