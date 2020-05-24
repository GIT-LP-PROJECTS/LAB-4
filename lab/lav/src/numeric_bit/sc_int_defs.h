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

    sc_int_defs.h : defines some of the sc_int_base methods as well as
    the concatenation operator ,() for sc_ints.

    Original Author: Amit Rao. Synopsys, Inc. (arao@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifdef SC_INT_H
#ifndef SC_INT_DEFS_H
#define SC_INT_DEFS_H

template<int W>  sc_int<W>::sc_int( const sc_bv<W>& v)
{
  width = W;
  int S = LLWIDTH - width;
  for( int i = width-1; i >= 0; i-- ){
    set( i, v[i]);
  }
  num = num << S >> S;
}

template<int W>  sc_int<W>::sc_int( const sc_lv<W>& v )
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

inline 
sc_int_base::sc_int_base(const sc_int_subref& v)
{
  width = v.left - v.right + 1; num = v.operator int64();
}

#if defined(__BCPLUSPLUS__)
#pragma warn -8027
#endif
inline sc_int_base::sc_int_base( const sc_bool_vector& v )
{
  width = v.length();
  int S = LLWIDTH - width;
  for( int i = width-1; i >= 0; i-- ){
    set( i, *(v.get_data_ptr(i)) );
  }
  num = num << S >> S;

}

inline sc_int_base::sc_int_base( const sc_logic_vector& v )
{
  width = v.length();
  int S = LLWIDTH - width;
  for( int i = width-1; i >= 0; i-- ){
    set( i, v.get_data_ptr(i)->to_bool() );
  }
  num = num << S >> S;
}

inline sc_int_base::sc_int_base( const sc_unsigned& v )
{
  width = v.length();

  int S = LLWIDTH - width;

  for( int i = width-1; i >= 0; i-- ){
    set(i, v.test(i));
  }
  num = num << S >> S;
}


inline sc_int_base::sc_int_base( const sc_signed& v )
{
  width = v.length();
  int S = LLWIDTH - width;
    
  for( int i = width-1; i >= 0; i-- ){
    set(i, v.test(i));
  }
  
  num = num << S >> S;
}
  


inline sc_int_base& sc_int_base::operator=( const sc_int_subref& v )
{
  num = v.operator int64(); 
  return *this; 
}

inline sc_int_base& sc_int_base::operator=( const sc_bool_vector& v )
{
  int S = LLWIDTH - width;
  int vlen = v.length();
  int len = MIN( width, vlen );
  
  for( int i = len-1 ; i >= 0; i-- ){
    set( i, *(v.get_data_ptr(i)) );
  }
  num = num << S >> S;
  return *this;
}
  
inline sc_int_base& sc_int_base::operator=( const sc_logic_vector& v )
{
  int S = LLWIDTH - width;
  int vlen = v.length();
  int len = MIN( width, vlen );
  

  for( int i = len-1; i >= 0; i-- ){
    set( i, v.get_data_ptr(i)->to_bool() );
  }
  
  num = num << S >> S;
  return *this;
}

template<int W>
inline sc_int<W>& sc_int<W>::operator=( const sc_bv<W>& v )
{
  int S = LLWIDTH - width;
  for( int i = W-1 ; i >= 0; i-- ){
    set( i, v[i] );
  }
  num = num << S >> S;
  return *this;
}

template<int W>
inline sc_int<W>& sc_int<W>::operator=( const sc_lv<W>& v )
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

inline sc_int_base& sc_int_base::operator=( const sc_unsigned& v )
{
  int S = LLWIDTH - width;
  int vlen = v.length();
  int minlen = MIN(width, vlen);

  int i=0;
  for( ; i < minlen; ++i)
    set(i, v.test(i));
  for( ; i < width; ++i )
    set(i, 0); /* zero-extend */
 
  num = num << S >> S;
  return *this;
}

inline sc_int_base& sc_int_base::operator=( const sc_signed& v )
{
  int S = LLWIDTH - width;
  int vlen = v.length();
  int minlen = MIN(width, vlen);

  int i = 0;
  for( ; i < minlen; ++i )
    set( i, v.test(i) );
  bit sgn = v.sign();
  for( ; i < width; ++i ) 
    set( i, sgn );     /* sign-extend */

  num = num << S >> S;
  
  return *this;
}

#if defined(__BCPLUSPLUS__)
#pragma warn .8027
#endif

inline sc_int_bitref sc_int_base::operator[] ( int i )
{
  return sc_int_bitref( this, i ); 
}

inline sc_int_subref sc_int_base::range( int left, int right )
{
  return sc_int_subref( this, left, right );
}

inline int64 sc_int_base::range( int left, int right ) const
{
  return num << (LLWIDTH - (left + 1)) >> ( LLWIDTH -( left + 1) + right );  
}

// here we define concatenation operators taking args of all possible combinations.
// we define non-const and const versions. Note that the const versions still
// return non-const proxies as u can have (a.range(3,0), b.range(5,4) ) = ...
// In the above code, a.range() returns a temporary which gets bound to a const 
// sc_int_subref&. So, the compiler call operator,(const sc_int_subref&, const sc_int_subref& )
// Since this is on the l-value we return a non-const sc_int_concat

inline sc_int_concat<sc_int_base,sc_int_base> operator ,( sc_int_base& l, sc_int_base& r)
{
  return sc_int_concat<sc_int_base,sc_int_base>( l , r );
}

inline sc_int_concat<sc_int_base,sc_int_base> operator ,( const sc_int_base& l, const sc_int_base& r ) 
{
  return sc_int_concat<sc_int_base,sc_int_base>( const_cast<sc_int_base&>(l), const_cast<sc_int_base&>(r) );
}

inline sc_int_concat<sc_int_base,sc_int_bitref> operator ,( sc_int_base& l, sc_int_bitref& r)
{
  return sc_int_concat<sc_int_base,sc_int_bitref>( l, r );
}

inline sc_int_concat<sc_int_base,sc_int_bitref> operator ,( const sc_int_base& l, const sc_int_bitref& r )
{
  return sc_int_concat<sc_int_base,sc_int_bitref>( const_cast<sc_int_base&>(l), const_cast<sc_int_bitref&>(r) );
}

inline sc_int_concat<sc_int_base,sc_int_subref> operator ,( sc_int_base& l, sc_int_subref& r)
{
  return sc_int_concat<sc_int_base,sc_int_subref>( l, r );
}

inline sc_int_concat<sc_int_base,sc_int_subref> operator ,( const sc_int_base& l, const sc_int_subref& r)
{
  return sc_int_concat<sc_int_base,sc_int_subref>( const_cast<sc_int_base&>(l), const_cast<sc_int_subref&>(r) );
}

template<class T1, class T2>
inline sc_int_concat<sc_int_base,sc_int_concat<T1,T2> > operator ,( sc_int_base& l, sc_int_concat<T1,T2>& r )
{
  return sc_int_concat<sc_int_base,sc_int_concat<T1,T2> >( l, r );
}

template<class T1, class T2>
inline sc_int_concat<sc_int_base,sc_int_concat<T1,T2> > operator ,( const sc_int_base& l, const sc_int_concat<T1, T2>& r )
{
  return sc_int_concat<sc_int_base,sc_int_concat<T1,T2> >( const_cast<sc_int_base&>(l), const_cast<sc_int_concat<T1,T2>&>(r) );
}

inline sc_int_concat<sc_int_bitref,sc_int_base> operator ,( sc_int_bitref& l, sc_int_base& r )
{
  return sc_int_concat<sc_int_bitref,sc_int_base>( l, r );
}

inline sc_int_concat<sc_int_bitref,sc_int_base> operator ,( const sc_int_bitref& l, const sc_int_base& r ) 
{
  return sc_int_concat<sc_int_bitref,sc_int_base>( const_cast<sc_int_bitref&>(l), const_cast<sc_int_base&>(r) );
}

inline sc_int_concat<sc_int_bitref,sc_int_bitref> operator,( sc_int_bitref& l, sc_int_bitref& r )
{
  return sc_int_concat<sc_int_bitref,sc_int_bitref>(l, r);
}

inline sc_int_concat<sc_int_bitref,sc_int_bitref> operator,( const sc_int_bitref& l, const sc_int_bitref& r )
{
  return sc_int_concat<sc_int_bitref,sc_int_bitref>( const_cast<sc_int_bitref&>(l), const_cast<sc_int_bitref&>(r) );
}

inline sc_int_concat<sc_int_bitref,sc_int_subref> operator,( sc_int_bitref& l, sc_int_subref& r )
{
  return sc_int_concat<sc_int_bitref,sc_int_subref>( l, r );
}

inline sc_int_concat<sc_int_bitref,sc_int_subref> operator,( const sc_int_bitref& l, const sc_int_subref& r )
{
  return sc_int_concat<sc_int_bitref,sc_int_subref>( const_cast<sc_int_bitref&>(l), const_cast<sc_int_subref&>(r) );
}

template<class T1, class T2>
inline sc_int_concat<sc_int_bitref,sc_int_concat<T1,T2> > operator,( sc_int_bitref& l, sc_int_concat<T1,T2>& r )
{
  return sc_int_concat<sc_int_bitref,sc_int_concat<T1,T2> >( l, r );
}

template<class T1, class T2 >
inline sc_int_concat<sc_int_bitref,sc_int_concat<T1,T2> > operator,( const sc_int_bitref& l, const sc_int_concat<T1,T2>& r )
{
  return sc_int_concat<sc_int_bitref,sc_int_concat<T1,T2> >( const_cast<sc_int_bitref&>(l), const_cast<sc_int_concat<T1,T2>&>(r) );
}

inline sc_int_concat<sc_int_subref,sc_int_base> operator,( sc_int_subref& l, sc_int_base& r )
{
  return sc_int_concat<sc_int_subref,sc_int_base>( l, r );
}

inline sc_int_concat<sc_int_subref,sc_int_base> operator,( const sc_int_subref& l, const sc_int_base& r )
{
  return sc_int_concat<sc_int_subref,sc_int_base>( const_cast<sc_int_subref&>(l), const_cast<sc_int_base&>(r) );
}

inline sc_int_concat<sc_int_subref,sc_int_bitref> operator,( sc_int_subref& l, sc_int_bitref& r )
{
  return sc_int_concat<sc_int_subref,sc_int_bitref>( l, r );
}

inline sc_int_concat<sc_int_subref,sc_int_bitref> operator,( const sc_int_subref& l, const sc_int_bitref& r )
{
  return sc_int_concat<sc_int_subref,sc_int_bitref>( const_cast<sc_int_subref&>(l), const_cast<sc_int_bitref&>(r) );
}

inline sc_int_concat<sc_int_subref,sc_int_subref> operator,( sc_int_subref& l, sc_int_subref& r )
{
  return sc_int_concat<sc_int_subref,sc_int_subref>( l, r );
}

inline sc_int_concat<sc_int_subref,sc_int_subref> operator,( const sc_int_subref& l, const sc_int_subref& r )
{
  return sc_int_concat<sc_int_subref,sc_int_subref>( const_cast<sc_int_subref&>(l), const_cast<sc_int_subref&>(r) );
}

template<class T1, class T2>
inline sc_int_concat<sc_int_subref,sc_int_concat<T1,T2> > operator,( sc_int_subref& l, sc_int_concat<T1,T2>& r )
{
  return sc_int_concat<sc_int_subref,sc_int_concat<T1,T2> >( l, r );
}

template<class T1, class T2>
inline sc_int_concat<sc_int_subref,sc_int_concat<T1,T2> > operator,( const sc_int_subref& l, const sc_int_concat<T1, T2>& r )
{
  return sc_int_concat<sc_int_subref,sc_int_concat<T1,T2> >( const_cast<sc_int_subref&>(l), const_cast<sc_int_concat<T1,T2>&>(r) );
}

template<class T1, class T2>
inline sc_int_concat<sc_int_concat<T1,T2>,sc_int_base> operator,( sc_int_concat<T1,T2>& l, sc_int_base& r )
{
  return sc_int_concat<sc_int_concat<T1,T2>,sc_int_base>( l, r );
}

template<class T1, class T2>
inline sc_int_concat<sc_int_concat<T1,T2>,sc_int_base> operator,( const sc_int_concat<T1,T2>& l, const sc_int_base& r )
{
  return sc_int_concat<sc_int_concat<T1,T2>,sc_int_base>( const_cast<sc_int_concat<T1,T2>&>(l), const_cast<sc_int_base&>(r) );
}

template<class T1, class T2>
inline sc_int_concat<sc_int_concat<T1,T2>,sc_int_bitref> operator,( sc_int_concat<T1,T2>& l, sc_int_bitref& r )
{
  return sc_int_concat<sc_int_concat<T1,T2>,sc_int_bitref>( l, r );
}

template<class T1, class T2>
inline sc_int_concat<sc_int_concat<T1,T2>,sc_int_bitref>  operator,( const sc_int_concat<T1,T2>& l, const sc_int_bitref& r )
{
  return sc_int_concat<sc_int_concat<T1,T2>,sc_int_bitref>( const_cast<sc_int_concat<T1,T2>&>(l), const_cast<sc_int_bitref&>(r) );
}

template<class T1, class T2>
inline sc_int_concat<sc_int_concat<T1,T2>,sc_int_subref> operator,( sc_int_concat<T1,T2>& l, sc_int_subref& r )
{
  return sc_int_concat<sc_int_concat<T1,T2>,sc_int_subref>( l, r );
}

template<class T1, class T2>
inline sc_int_concat<sc_int_concat<T1,T2>,sc_int_subref> operator,( const sc_int_concat<T1,T2>& l, const sc_int_subref& r )
{
  return sc_int_concat<sc_int_concat<T1,T2>,sc_int_subref>( const_cast<sc_int_concat<T1,T2>&>(l), const_cast<sc_int_subref&>(r) );
}

template<class T1,class T2, class T3, class T4>
inline sc_int_concat<sc_int_concat<T1,T2>, sc_int_concat<T3,T4> > operator,( sc_int_concat<T1,T2>& l, sc_int_concat<T3,T4>& r )
{
  return sc_int_concat<sc_int_concat<T1,T2>, sc_int_concat<T3,T4> >( l, r );
}

template<class T1,class T2, class T3, class T4>
inline sc_int_concat<sc_int_concat<T1,T2>, sc_int_concat<T3,T4> > operator,( const sc_int_concat<T1,T2>& l, 
										   const sc_int_concat<T3,T4>& r )
{
  return sc_int_concat<sc_int_concat<T1,T2>, sc_int_concat<T3,T4> >( const_cast<sc_int_concat<T1,T2>&>(l), 
								     const_cast<sc_int_concat<T3,T4>&>(r) );
}

#endif
#endif
