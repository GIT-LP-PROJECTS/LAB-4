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

    sc_uint_defs.h : defines some of the sc_uint_base methods as well
    as the concatenation operator ,() for sc_uints.

    Original Author: Amit Rao. Synopsys, Inc. (arao@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
       Gene Bushuyev, Synopsys, Inc.(gbush@synopsys.com)
    Description of Modification:
       Constructors and assignments added for sc_bv<>, sc_lv<>

    Modifier Name & Affiliation:
    Description of Modification:

******************************************************************************/


#ifdef SC_UINT_H
#ifndef SC_UINT_DEFS_H
#define SC_UINT_DEFS_H

#include "../sc_bool_vector.h"
#include "../sc_logic_vector.h"
#include "sc_signed.h"
#include "sc_unsigned.h"

template<int W>  sc_uint<W>::sc_uint( const sc_bv<W>& v)
{
  width = W;
  int S = LLWIDTH - width;
  for( int i = width-1; i >= 0; i-- ){
    set( i, v[i]);
  }
  num = num << S >> S;
}
template<int W>  sc_uint<W>::sc_uint( const sc_lv<W>& v )
{
  width = W;
  int S = LLWIDTH - width;
  for( int i = width-1; i >= 0; i-- ){
    if(v[i]>1)
      throw "cannot convert logic to bit";
    set( i, v[i]);
  }
  num = num << S >> S;
}

#if defined(__BCPLUSPLUS__)
#pragma warn -8027
#endif
template<int W>
inline sc_uint<W>& sc_uint<W>::operator=( const sc_bv<W>& v )
{
  int S = LLWIDTH - width;
  for( int i = W-1 ; i >= 0; i-- ){
    set( i, v[i] );
  }
  num = num << S >> S;
  return *this;
}

template<int W>
inline sc_uint<W>& sc_uint<W>::operator=( const sc_lv<W>& v )
{
  int S = LLWIDTH - width;
  for( int i = W-1 ; i >= 0; i-- ){
    if(v[i]>1)
      throw "cannot convert logic to bit";
    set( i, v[i] );
  }
  num = num << S >> S;
  return *this;
}

inline sc_uint_base::sc_uint_base(const sc_uint_subref& v)
{
  width = v.left - v.right + 1; num = v.operator uint64();
}

inline sc_uint_base::sc_uint_base( const sc_bool_vector& v )
{
  width = v.length();
  for (int i = width - 1; i >= 0; i-- )
    set(i, *(v.get_data_ptr(i)) );
  num &= MASK(width);
}

inline sc_uint_base::sc_uint_base( const sc_logic_vector& v ) 
{
  width = v.length();
  for (int i = width - 1; i >= 0; i-- )
    set(i, v.get_data_ptr(i)->to_bool() );
  num &= MASK(width);
}
  
inline sc_uint_base::sc_uint_base( const sc_signed& v )
{
  width = v.length();
  
  for( int i = width - 1 ; i >= 0; i-- )
    set(i, v.test(i) );
  num &= MASK(width);
}

inline sc_uint_base::sc_uint_base( const sc_unsigned& v )
{
  width = v.length();
  
  for( int i = width - 1 ; i >= 0; i-- )
    set(i, v.test(i) );
  num &= MASK(width);
}

inline sc_uint_base& sc_uint_base::operator=( const sc_uint_subref& v )
{
  width = v.left - v.right + 1; 
  num = v.operator uint64(); 
  return *this; 
}

inline sc_uint_base& sc_uint_base::operator=( const sc_bool_vector& v )
{
  int vlen = v.length();
  int len = MIN( width, vlen );
  for( int i = len-1 ; i >= 0; i-- ){
    set( i, *(v.get_data_ptr(i)) );
  }
  num &= MASK(width);
  return *this;
}
  
inline sc_uint_base& sc_uint_base::operator=( const sc_logic_vector& v )
{
  int vlen = v.length();
  int len = MIN( width, vlen );
  

  for( int i = len-1; i >= 0; i-- ){
    set( i, v.get_data_ptr(i)->to_bool() );
  }
  
  num &= MASK(width);
  return *this;
}


inline sc_uint_base& sc_uint_base::operator=( const sc_unsigned& v )
{
  int vlen = v.length();
  int minlen = MIN(width, vlen);

  int i=0;
  for( ; i < minlen; ++i)
    set(i, v.test(i));
  for( ; i < width; ++i )
    set(i, 0); /* zero-extend */
 
  num &= MASK(width);
  return *this;
}

inline sc_uint_base& sc_uint_base::operator=( const sc_signed& v )
{
  int vlen = v.length();
  int minlen = MIN(width, vlen);

  int i = 0;
  for( ; i < minlen; ++i )
    set( i, v.test(i) );
  bit sgn = v.sign();
  for( ; i < width; ++i ) 
    set( i, sgn );     /* sign-extend */

  num &= MASK(width);
  
  return *this;
}

#if defined(__BCPLUSPLUS__)
#pragma warn .8027
#endif

inline sc_uint_bitref sc_uint_base::operator[] ( int i )
{
  return sc_uint_bitref( this, i ); 
}

inline sc_uint_subref sc_uint_base::range( int left, int right )
{
  return sc_uint_subref( this, left, right );
}

inline uint64 sc_uint_base::range( int left, int right ) const
{
  if( left < width - 1 )
#ifndef WIN32
    return ( (num & ((1ULL << (left + 1)) - 1)) >> right ); 
#else
    return ( (num & ((1i64 << (left + 1)) - 1)) >> right ); 
#endif 
  else
    return num >> right; 
}

// here we define concatenation operators taking args of all possible combinations.
// we define non-const and const versions. Note that the const versions still
// return non-const proxies as u can have (a.range(3,0), b.range(5,4) ) = ...
// In the above code, a.range() returns a temporary which gets bound to a const 
// sc_uint_subref&. So, the compiler call operator,(const sc_uint_subref&, const sc_uint_subref& )
// Since this is on the l-value we return a non-const sc_uint_concat

inline sc_uint_concat<sc_uint_base,sc_uint_base> operator ,( sc_uint_base& l, sc_uint_base& r)
{
  return sc_uint_concat<sc_uint_base,sc_uint_base>( l , r );
}

inline sc_uint_concat<sc_uint_base,sc_uint_base> operator ,( const sc_uint_base& l, const sc_uint_base& r ) 
{
  return sc_uint_concat<sc_uint_base,sc_uint_base>( const_cast<sc_uint_base&>(l), const_cast<sc_uint_base&>(r) );
}

inline sc_uint_concat<sc_uint_base,sc_uint_bitref> operator ,( sc_uint_base& l, sc_uint_bitref& r)
{
  return sc_uint_concat<sc_uint_base,sc_uint_bitref>( l, r );
}

inline sc_uint_concat<sc_uint_base,sc_uint_bitref> operator ,( const sc_uint_base& l, const sc_uint_bitref& r )
{
  return sc_uint_concat<sc_uint_base,sc_uint_bitref>( const_cast<sc_uint_base&>(l), const_cast<sc_uint_bitref&>(r) );
}

inline sc_uint_concat<sc_uint_base,sc_uint_subref> operator ,( sc_uint_base& l, sc_uint_subref& r)
{
  return sc_uint_concat<sc_uint_base,sc_uint_subref>( l, r );
}

inline sc_uint_concat<sc_uint_base,sc_uint_subref> operator ,( const sc_uint_base& l, const sc_uint_subref& r)
{
  return sc_uint_concat<sc_uint_base,sc_uint_subref>( const_cast<sc_uint_base&>(l), const_cast<sc_uint_subref&>(r) );
}

template<class T1, class T2>
inline sc_uint_concat<sc_uint_base,sc_uint_concat<T1,T2> > operator ,( sc_uint_base& l, sc_uint_concat<T1,T2>& r )
{
  return sc_uint_concat<sc_uint_base,sc_uint_concat<T1,T2> >( l, r );
}

template<class T1, class T2>
inline sc_uint_concat<sc_uint_base,sc_uint_concat<T1,T2> > operator ,( const sc_uint_base& l, const sc_uint_concat<T1, T2>& r )
{
  return sc_uint_concat<sc_uint_base,sc_uint_concat<T1,T2> >( const_cast<sc_uint_base&>(l), const_cast<sc_uint_concat<T1,T2>&>(r) );
}

inline sc_uint_concat<sc_uint_bitref,sc_uint_base> operator ,( sc_uint_bitref& l, sc_uint_base& r )
{
  return sc_uint_concat<sc_uint_bitref,sc_uint_base>( l, r );
}

inline sc_uint_concat<sc_uint_bitref,sc_uint_base> operator ,( const sc_uint_bitref& l, const sc_uint_base& r ) 
{
  return sc_uint_concat<sc_uint_bitref,sc_uint_base>( const_cast<sc_uint_bitref&>(l), const_cast<sc_uint_base&>(r) );
}

inline sc_uint_concat<sc_uint_bitref,sc_uint_bitref> operator,( sc_uint_bitref& l, sc_uint_bitref& r )
{
  return sc_uint_concat<sc_uint_bitref,sc_uint_bitref>(l, r);
}

inline sc_uint_concat<sc_uint_bitref,sc_uint_bitref> operator,( const sc_uint_bitref& l, const sc_uint_bitref& r )
{
  return sc_uint_concat<sc_uint_bitref,sc_uint_bitref>( const_cast<sc_uint_bitref&>(l), const_cast<sc_uint_bitref&>(r) );
}

inline sc_uint_concat<sc_uint_bitref,sc_uint_subref> operator,( sc_uint_bitref& l, sc_uint_subref& r )
{
  return sc_uint_concat<sc_uint_bitref,sc_uint_subref>( l, r );
}

inline sc_uint_concat<sc_uint_bitref,sc_uint_subref> operator,( const sc_uint_bitref& l, const sc_uint_subref& r )
{
  return sc_uint_concat<sc_uint_bitref,sc_uint_subref>( const_cast<sc_uint_bitref&>(l), const_cast<sc_uint_subref&>(r) );
}

template<class T1, class T2>
inline sc_uint_concat<sc_uint_bitref,sc_uint_concat<T1,T2> > operator,( sc_uint_bitref& l, sc_uint_concat<T1,T2>& r )
{
  return sc_uint_concat<sc_uint_bitref,sc_uint_concat<T1,T2> >( l, r );
}

template<class T1, class T2 >
inline sc_uint_concat<sc_uint_bitref,sc_uint_concat<T1,T2> > operator,( const sc_uint_bitref& l, const sc_uint_concat<T1,T2>& r )
{
  return sc_uint_concat<sc_uint_bitref,sc_uint_concat<T1,T2> >( const_cast<sc_uint_bitref&>(l), const_cast<sc_uint_concat<T1,T2>&>(r) );
}

inline sc_uint_concat<sc_uint_subref,sc_uint_base> operator,( sc_uint_subref& l, sc_uint_base& r )
{
  return sc_uint_concat<sc_uint_subref,sc_uint_base>( l, r );
}

inline sc_uint_concat<sc_uint_subref,sc_uint_base> operator,( const sc_uint_subref& l, const sc_uint_base& r )
{
  return sc_uint_concat<sc_uint_subref,sc_uint_base>( const_cast<sc_uint_subref&>(l), const_cast<sc_uint_base&>(r) );
}

inline sc_uint_concat<sc_uint_subref,sc_uint_bitref> operator,( sc_uint_subref& l, sc_uint_bitref& r )
{
  return sc_uint_concat<sc_uint_subref,sc_uint_bitref>( l, r );
}

inline sc_uint_concat<sc_uint_subref,sc_uint_bitref> operator,( const sc_uint_subref& l, const sc_uint_bitref& r )
{
  return sc_uint_concat<sc_uint_subref,sc_uint_bitref>( const_cast<sc_uint_subref&>(l), const_cast<sc_uint_bitref&>(r) );
}

inline sc_uint_concat<sc_uint_subref,sc_uint_subref> operator,( sc_uint_subref& l, sc_uint_subref& r )
{
  return sc_uint_concat<sc_uint_subref,sc_uint_subref>( l, r );
}

inline sc_uint_concat<sc_uint_subref,sc_uint_subref> operator,( const sc_uint_subref& l, const sc_uint_subref& r )
{
  return sc_uint_concat<sc_uint_subref,sc_uint_subref>( const_cast<sc_uint_subref&>(l), const_cast<sc_uint_subref&>(r) );
}

template<class T1, class T2>
inline sc_uint_concat<sc_uint_subref,sc_uint_concat<T1,T2> > operator,( sc_uint_subref& l, sc_uint_concat<T1,T2>& r )
{
  return sc_uint_concat<sc_uint_subref,sc_uint_concat<T1,T2> >( l, r );
}

template<class T1, class T2>
inline sc_uint_concat<sc_uint_subref,sc_uint_concat<T1,T2> > operator,( const sc_uint_subref& l, const sc_uint_concat<T1, T2>& r )
{
  return sc_uint_concat<sc_uint_subref,sc_uint_concat<T1,T2> >( const_cast<sc_uint_subref&>(l), const_cast<sc_uint_concat<T1,T2>&>(r) );
}

template<class T1, class T2>
inline sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base> operator,( sc_uint_concat<T1,T2>& l, sc_uint_base& r )
{
  return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base>( l, r );
}

template<class T1, class T2>
inline sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base> operator,( const sc_uint_concat<T1,T2>& l, const sc_uint_base& r )
{
  return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base>( const_cast<sc_uint_concat<T1,T2>&>(l), const_cast<sc_uint_base&>(r) );
}

template<class T1, class T2>
inline sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_bitref> operator,( sc_uint_concat<T1,T2>& l, sc_uint_bitref& r )
{
  return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_bitref>( l, r );
}

template<class T1, class T2>
inline sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_bitref>  operator,( const sc_uint_concat<T1,T2>& l, const sc_uint_bitref& r )
{
  return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_bitref>( const_cast<sc_uint_concat<T1,T2>&>(l), const_cast<sc_uint_bitref&>(r) );
}

template<class T1, class T2>
inline sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_subref> operator,( sc_uint_concat<T1,T2>& l, sc_uint_subref& r )
{
  return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_subref>( l, r );
}

template<class T1, class T2>
inline sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_subref> operator,( const sc_uint_concat<T1,T2>& l, const sc_uint_subref& r )
{
  return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_subref>( const_cast<sc_uint_concat<T1,T2>&>(l), const_cast<sc_uint_subref&>(r) );
}

template<class T1,class T2, class T3, class T4>
inline sc_uint_concat<sc_uint_concat<T1,T2>, sc_uint_concat<T3,T4> > operator,( sc_uint_concat<T1,T2>& l, sc_uint_concat<T3,T4>& r )
{
  return sc_uint_concat<sc_uint_concat<T1,T2>, sc_uint_concat<T3,T4> >( l, r );
}

template<class T1,class T2, class T3, class T4>
inline sc_uint_concat<sc_uint_concat<T1,T2>, sc_uint_concat<T3,T4> > operator,( const sc_uint_concat<T1,T2>& l, 
										const sc_uint_concat<T3,T4>& r )
{
  return sc_uint_concat<sc_uint_concat<T1,T2>, sc_uint_concat<T3,T4> >( const_cast<sc_uint_concat<T1,T2>&>(l), 
								        const_cast<sc_uint_concat<T3,T4>&>(r) );
}


#endif
#endif
