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

    sc_string.h -- Implementation of a simple string class.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_STRING_H
#define SC_STRING_H

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

#ifndef __SUNPRO_CC
#endif

// ----------------------------------------------------------------------------
//  ENUM : sc_numrep
//
//  Enumeration of number representations for character string conversion.
// ----------------------------------------------------------------------------

enum sc_numrep
{
    SC_NOBASE = 0,
    SC_BIN    = 2,
    SC_OCT    = 8,
    SC_DEC    = 10,
    SC_HEX    = 16,
    SC_BIN_US,
    SC_BIN_SM,
    SC_OCT_US,
    SC_OCT_SM,
    SC_HEX_US,
    SC_HEX_SM,
    SC_CSD
};

class sc_string_rep;

class sc_string {
    friend ostream& operator<<( ostream& os, const sc_string& s );
    friend istream& operator>>( istream& is, sc_string& s );

public:
    sc_string( int size=16 );
    sc_string( const char* s ); // must be 0-ended
    sc_string( const char* s, int n ); // get first n chars from the string
    sc_string( const sc_string& s );
    ~sc_string();

    sc_string operator+( const char* s ) const;
    sc_string operator+( char c) const;
    sc_string operator+( const sc_string& s ) const;

    friend sc_string operator+( const char* s, const sc_string& t );

    sc_string& operator=( const char* s );
    sc_string& operator=( const sc_string& s );

    sc_string& operator+=( const char* s );
    sc_string& operator+=( char c);
    sc_string& operator+=( const sc_string& s );

    int cmp( const char* s ) const;
    int cmp( const sc_string& s ) const;

    const char* c_str() const;
    // get substring
    sc_string substr(int,int) const;
    static sc_string make_str(long n); // convert long to string

#define DECL_RELOP(op) \
    bool operator ## op( const char* s ) const; \
    bool operator ## op( const sc_string& s ) const

    DECL_RELOP(==);
    DECL_RELOP(!=);
    DECL_RELOP(<);
    DECL_RELOP(<=);
    DECL_RELOP(>);
    DECL_RELOP(>=);
#undef DECL_RELOP

    int length() const;

    operator const char*() const;
    char operator[](int i) const;
    void set( int i, char c );

    void print(ostream& os = cout) const;

private:
    sc_string( sc_string_rep* r );
    sc_string_rep* rep;
};

const sc_string to_string( sc_numrep );

inline
ostream&
operator << ( ostream& os, sc_numrep numrep )
{
    return os << to_string( numrep );
}


#endif
