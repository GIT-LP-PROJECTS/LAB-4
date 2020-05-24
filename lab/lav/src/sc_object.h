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

    sc_object.h -- Abstract base class of all SystemC `simulation' objects.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_OBJECT_H
#define SC_OBJECT_H

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

class sc_trace_file;
class sc_simcontext;

class sc_object {
    friend class sc_object_manager;

public:
    const char* name() const { return m_name; }
    const char* basename() const;

    void print() const;
    virtual void print(ostream& os) const;

    // dump() is more detailed than print()
    void dump() const;
    virtual void dump(ostream& os) const;

    virtual void trace( sc_trace_file* tf ) const;

    static const char* kind_string;
    virtual const char* kind() const;

    sc_simcontext* simcontext() const { return simc; }

protected:
    sc_object();
    sc_object(const char* nm);
    virtual ~sc_object();

private:
    void sc_object_init(const char* nm);

private:
    /* Each simulation object is associated with a simulation context */ 
    sc_simcontext* simc;   
    char* m_name;
};

const char HIERARCHY_CHAR = '.';
extern bool sc_enable_name_checking;

#endif
