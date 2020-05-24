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

    sc_exception.cpp -- implementation of the exception classes in SystemC

    Original Author: Gene Bushuyev. Synopsys, Inc. (gbush@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:


******************************************************************************/
//---------------------------------------------------------------------------
#include "sc_exception.h"

// don't use raw data from the ErrRec, it may change in the future
// use GetErrorMsg function instead
struct ErrRecType
{
  int ErrNo;
  const char* ErrMsg; // array of string literals
};
ErrRecType ErrRec[]=
{
  // general
  { 0, "unknown error"},
  { 1, ""},
  { 2, "not implemented"},
  { 1000, "out of bounds"},
  { 1001, "incompatible types for assignment"},
  { 1002, "incompatible types"},
  { 1003, "cannot perform conversion"},
  { 1004, "fiber doesn''t exist"},
  { 1005, "maximum number of processes per module exceeded"},
  { 1006, "value is not valid"},
  { 1007, "cannot perform requested operation"},
  { 1008, "sc_string doesn''t contain an integer"},
  // datatypes
  { 2000, "incompatible vectors"},
  { 2001, "zero length"},
  { 2002, "vector is too long"},
  { 2003, "vector contains 4-value logic"},
  { 2004, "sc_bv cannot contain values X and Z"},
  { 2005, "wrong base type in sc_string"},
  { 2006, "vector is not resizeable"},
  // processes
  { 3001, "internal error: sc_vc6_precess_helper"},
  // modules
  { 4001, "incorrect use of sc_module_name"},
  { 4002, "port specified outside of module"},
  // signals
  { 5001, "event method unavailable"},
  // legacy
  { 9001, "SC_ERR_ARRAY_BOUND"},
  { 9002, "SC_ERR_ARRAY_LENGTH"},
  { 9003,  "SC_ERR_PARAMETER"},
  { 9004,  "SC_ERR_LAMBDA"},
  { 9005,  "SC_ERR_SC_LOGIC"},
  { 9006,  "SC_ERR_WAIT"},
  { 9007,  "SC_ERR_WATCHING"}
};
// -------------------------------
static sc_string GetErrorMsg(int N)
// -------------------------------
{
  bool found = false;
  unsigned i=0;
  while(!found && i<sizeof(ErrRec))
  {
    found = ErrRec[i].ErrNo == N;
    i++;
  }
  if(found)
    return sc_string("(E")+sc_string::make_str(N)+") "+ErrRec[i-1].ErrMsg;
  else
    return GetErrorMsg(0);
}
//---------------------------------------------------------------------------
// base exception class
sc_exception::sc_exception () throw()
{
}
sc_exception::sc_exception(const sc_string& What) throw():What(What)
{
}
sc_exception::sc_exception(int Error)throw() :What(GetErrorMsg(Error))
{
}
sc_exception::sc_exception(int Error, const sc_string& Info) throw()
  :What(GetErrorMsg(Error)+" : "+Info)
{
}
sc_exception::sc_exception (const sc_exception& e) throw()
  :What(e.What)
{
}
sc_exception& sc_exception::operator= (const sc_exception& e) throw()
{
  What = e.What;
  return *this;
}
sc_exception::~sc_exception () throw()
{
}
const char* sc_exception::what() const throw()
{
  return str().c_str();
}
const sc_string sc_exception::str() const throw()
{
  return What;
}
// exception class for data types
sc_edata::sc_edata () throw()
{
}
sc_edata::sc_edata(const sc_string& What) throw():sc_exception(What)
{
}
sc_edata::sc_edata(int Error) throw() :sc_exception(Error)
{
}
sc_edata::sc_edata(int Error, const sc_string& Info) throw()
  :sc_exception(Error,Info)
{
}
sc_edata::sc_edata (const sc_edata& e) throw()
  :sc_exception(e)
{
}
sc_edata& sc_edata::operator= (const sc_edata& e) throw()
{
  What = e.What;
  return *this;
}
sc_edata::~sc_edata () throw()
{
}
const sc_string sc_edata::str() const throw()
{
  return "ERROR: "+What;
}
// exception class for signals
sc_esignal::sc_esignal () throw()
{
}
sc_esignal::sc_esignal(const sc_string& What) throw():sc_exception(What)
{
}
sc_esignal::sc_esignal(int Error) throw():sc_exception(Error)
{
}
sc_esignal::sc_esignal(int Error, const sc_string& Info) throw()
  :sc_exception(Error,Info)
{
}
sc_esignal::sc_esignal (const sc_esignal& e) throw()
  :sc_exception(e)
{
}
sc_esignal& sc_esignal::operator= (const sc_esignal& e) throw()
{
  What = e.What;
  return *this;
}
sc_esignal::~sc_esignal () throw()
{
}
const sc_string sc_esignal::str() const throw()
{
  return "ERROR:"+What;
}
// exception class for processes
sc_eprocess::sc_eprocess () throw()
{
}
sc_eprocess::sc_eprocess(const sc_string& What) throw():sc_exception(What)
{
}
sc_eprocess::sc_eprocess(int Error) throw():sc_exception(Error)
{
}
sc_eprocess::sc_eprocess(int Error, const sc_string& Info) throw()
  :sc_exception(Error,Info)
{
}
sc_eprocess::sc_eprocess (const sc_eprocess& e) throw():sc_exception(e)
{
}
sc_eprocess& sc_eprocess::operator= (const sc_eprocess& e) throw()
{
  What = e.What;
  return *this;
}
sc_eprocess::~sc_eprocess () throw()
{
}
const sc_string sc_eprocess::str() const throw()
{
  return "ERROR: "+What;
}
// exception class for modules
sc_emodule::sc_emodule () throw()
{
}
sc_emodule::sc_emodule(const sc_string& What) throw():sc_exception(What)
{
}
sc_emodule::sc_emodule(int Error) throw():sc_exception(Error)
{
}
sc_emodule::sc_emodule(int Error, const sc_string& Info) throw()
  :sc_exception(Error,Info)
{
}
sc_emodule::sc_emodule (const sc_emodule& e) throw()
  :sc_exception(e)
{
}
sc_emodule& sc_emodule::operator= (const sc_emodule& e) throw()
{
  What = e.What;
  return *this;
}
sc_emodule::~sc_emodule () throw()
{
}
const sc_string sc_emodule::str() const throw()
{
  return "ERROR: "+What;
}
