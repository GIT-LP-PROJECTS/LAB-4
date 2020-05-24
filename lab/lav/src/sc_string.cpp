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

    sc_string.cpp -- Implementation of a simple string class.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

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
#include <assert.h>
#include <cstring>
#include <ctype.h>
#ifdef __BCPLUSPLUS__
#pragma hdrstop
#endif
#include "sc_string.h"
#include "sc_exception.h"
#include <stdio.h>
inline int
roundup( int n, int m )
{
    return ((n - 1) / m + 1) * m;
}

class sc_string_rep {
    friend class sc_string;
    friend ostream& operator<<( ostream&, const sc_string& );
    friend istream& operator>>( istream&, sc_string& );
    friend sc_string operator+( const char*, const sc_string& );

    sc_string_rep( int size = 16 )
    {
        ref_count = 1;
        alloc = roundup( size, 16 );
        str = new char[alloc];
        *str = '\0';
    }
    sc_string_rep( const char* s )
    {
        ref_count = 1;
        if (s) {
            alloc = 1 + strlen(s);
            str = strcpy( new char[alloc], s );
        }
        else {
            alloc = 16;
            str = strcpy( new char[alloc], "" );
        }
    }
    sc_string_rep( const char* s, int n); // get first n chars from the string
    ~sc_string_rep()
    {
        assert( ref_count == 0 );
        delete[] str;
    }

    void resize( int new_size );
    void set_string( const char* s );

    int ref_count;
    int alloc;
    char* str;
};

sc_string_rep::sc_string_rep( const char* s, int n)
{
    ref_count = 1;
    if (s && n>0) {
        alloc = 1 + n;
        str = strncpy( new char[alloc], s,n );
        str[n] = 00;
    }
    else {
        alloc = 16;
        str = strcpy( new char[alloc], "" );
    }
}

void
sc_string_rep::resize( int new_size )
{
    if (new_size <= alloc) return;
    alloc = roundup( new_size, 16 );
    char* new_str = strcpy( new char[alloc], str );
    delete[] str;
    str = new_str;
}

void
sc_string_rep::set_string( const char* s )
{
    int len = strlen(s);
    resize( len + 1 );
    strcpy( str, s );
}

sc_string::sc_string(int size)
{
    rep = new sc_string_rep(size);
}

sc_string::sc_string( const char* s )
{
    rep = new sc_string_rep(s);
}

sc_string::sc_string( const char* s, int n )
{
    rep = new sc_string_rep(s,n);
}

sc_string::sc_string( const sc_string& s )
{
    rep = s.rep;
    rep->ref_count++;
}

sc_string::sc_string( sc_string_rep* r )
{
    rep = r;
}

sc_string::~sc_string()
{
    if (--(rep->ref_count) == 0)
        delete rep;
}

int
sc_string::length() const
{
    return strlen(rep->str);
}

sc_string
sc_string::operator+( const char* s ) const
{
    int len = length();
    sc_string_rep* r = new sc_string_rep( len + strlen(s) + 1 );
    strcpy( r->str, rep->str );
    strcpy( r->str + len, s );
    return sc_string(r);
}
sc_string sc_string::operator+(char c) const
{
    int len = length();
    sc_string_rep* r = new sc_string_rep( len + 2 );
    strcpy( r->str, rep->str );
    r->str[len] = c;
    r->str[len+1] = 00;
    return sc_string(r);
}

sc_string
operator+( const char* s, const sc_string& t )
{
    int len = strlen(s);
    sc_string_rep* r = new sc_string_rep( len + t.length() + 1 );
    strcpy( r->str, s );
    strcpy( r->str + len, t );
    return sc_string(r);
}

sc_string
sc_string::operator+( const sc_string& s ) const
{
    int len = length();
    sc_string_rep* r = new sc_string_rep( len + s.length() + 1 );
    strcpy( r->str, rep->str );
    strcpy( r->str + len, s.rep->str );
    return sc_string(r);
}

sc_string&
sc_string::operator=( const char* s )
{
    if (rep->ref_count > 1) {
        --rep->ref_count;
        rep = new sc_string_rep(s);
    }
    else {
        rep->set_string(s);
    }
    return *this;
}

sc_string&
sc_string::operator=( const sc_string& s )
{
    if (&s == this)
        return *this;
    if (--(rep->ref_count) == 0)
        delete rep;
    rep = s.rep;
    rep->ref_count++;
    return *this;
}

sc_string&
sc_string::operator+=( const char* s )
{
    int oldlen = length();
    int slen   = strlen(s);
    if (rep->ref_count > 1) {
        sc_string_rep* oldrep = rep;
        --rep->ref_count;
        rep = new sc_string_rep( oldlen + slen + 1 );
        strcpy( rep->str, oldrep->str );
        strcpy( rep->str + oldlen, s );
    }
    else {
        rep->resize( oldlen + slen + 1 );
        strcpy( rep->str + oldlen, s );
    }
    return *this;
}

sc_string& sc_string::operator+=(char c)
{
    int oldlen = length();
    if (rep->ref_count > 1) {
        sc_string_rep* oldrep = rep;
        --rep->ref_count;
        rep = new sc_string_rep( oldlen + 2 );
        strcpy( rep->str, oldrep->str );
        rep->str[oldlen]=c;
        rep->str[oldlen+1]=00;
    }
    else {
        rep->resize( oldlen + 2 );
        rep->str[oldlen]=c;
        rep->str[oldlen+1]=00;
    }
    return *this;
}

sc_string&
sc_string::operator+=( const sc_string& s )
{
    return this->operator+=( s.rep->str );
}

int
sc_string::cmp( const char* s ) const
{
    return strcmp( rep->str, s );
}

int
sc_string::cmp( const sc_string& s ) const
{
    return strcmp( rep->str, s.rep->str );
}

const char* sc_string::c_str() const
{
  return rep->str;
}

// get substring
sc_string sc_string::substr(int first,int last) const
{
  if(first<0 || last<0 || first>=last || first>=length() || last>=length())
    return "";
  return sc_string(rep->str+first, last-first+1);
}


sc_string sc_string::make_str(long n) // convert integer to string
{
  char buf[32];
  sprintf(buf,"%ld",n);
  return sc_string(buf);
}


#define DEFINE_RELOP(op) \
bool sc_string::operator ## op( const char* s ) const \
{						\
    return strcmp( rep->str, s ) op 0;		\
}						\
bool sc_string::operator ## op( const sc_string& s ) const \
{						\
    return strcmp( rep->str, s.rep->str ) op 0;	\
}

DEFINE_RELOP(==)
DEFINE_RELOP(!=)
DEFINE_RELOP(<)
DEFINE_RELOP(<=)
DEFINE_RELOP(>)
DEFINE_RELOP(>=)

sc_string::operator const char*() const
{
    return rep->str;
}

char
sc_string::operator[]( int i ) const
{
    return rep->str[i];
}

void
sc_string::set( int i, char c )
{
    if (rep->ref_count > 1) {
        rep->ref_count--;
        rep = new sc_string_rep(rep->str);
    }
    rep->str[i] = c;
}

void
sc_string::print(ostream& os) const
{
    os << rep->str;
}

/*---------------------------------------------------------------------------*/

ostream&
operator<<( ostream& os, const sc_string& s )
{
    return os << s.rep->str;
}

istream&
operator>>( istream& is, sc_string& s )
{
    if (s.rep->ref_count > 1) {
        --s.rep->ref_count;
        s.rep = new sc_string_rep;
    }
    int i = 0;
    char* p = s.rep->str;
    char ch;

        /* skip white spaces */
    while (is.get(ch) && isspace(ch))
        ;

    for ( ; is.good() && !isspace(ch); is.get(ch)) {
        if (i > s.rep->alloc - 2) {
            s.rep->resize( (int) (s.rep->alloc * 1.5) );
            p = s.rep->str + i;
        }
        *p++ = ch;
        i++;
    }
    *p = '\0';
    return is;
}

// ----------------------------------------------------------------------------
//  ENUM : sc_numrep
//
//  Enumeration of number representations for character string conversion.
// ----------------------------------------------------------------------------

const sc_string
to_string( sc_numrep numrep )
{
    switch( numrep )
    {
        case SC_DEC:
	    return sc_string( "SC_DEC" );
        case SC_BIN:
	    return sc_string( "SC_BIN" );
        case SC_BIN_US:
	    return sc_string( "SC_BIN_US" );
        case SC_BIN_SM:
	    return sc_string( "SC_BIN_SM" );
        case SC_OCT:
	    return sc_string( "SC_OCT" );
        case SC_OCT_US:
	    return sc_string( "SC_OCT_US" );
        case SC_OCT_SM:
	    return sc_string( "SC_OCT_SM" );
        case SC_HEX:
	    return sc_string( "SC_HEX" );
        case SC_HEX_US:
	    return sc_string( "SC_HEX_US" );
        case SC_HEX_SM:
	    return sc_string( "SC_HEX_SM" );
        case SC_CSD:
	    return sc_string( "SC_CSD" );
	default:
	    return sc_string( "unknown" );
    }
}
