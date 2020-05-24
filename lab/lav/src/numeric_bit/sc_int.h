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

    sc_int.h -- A sc_int is a signed integer whose width is less than
    the machine's native integer width. We provide two implementations
    (i) sc_int with bitwidth between 1 - 64 (ii) sc_int with bitwidth
    between 1 - 32 Implementation (i) is the default implementation,
    while implementation (ii) can be used only if the class library is
    compiled with -D_32BIT_.  Unlike AP, arithmetic and bitwise
    operations are performed using the native types (hence capped at
    32/64 bits). The sc_int integer is useful when the user does not
    need AP and the performance is superior to sc_bigint/sc_biguint

    Original Author: Amit Rao. Synopsys, Inc. (arao@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_INT_H
#define SC_INT_H

#ifndef _32BIT_

#include "sc_bool_vector.h"
#include "sc_logic_vector.h"
#include "sc_signed.h"
#include "sc_unsigned.h"
#include "sc_nbdefs.h"

// Forward declarations
class sc_int_bitref;
class sc_int_subref;
class sc_int_base;
template<class T1, class T2> class sc_int_concat;
template<int W> class sc_int;
namespace sc_bv_ns
{
template<int W> class sc_bv;
template<int W> class sc_lv;
}
using sc_bv_ns::sc_bv;
using sc_bv_ns::sc_lv;

extern const uint64 mask64[LLWIDTH][LLWIDTH];


//Base class for sc_int. 

class sc_int_base {
  
  friend class sc_int_bitref;
  friend class sc_int_subref;
  
public:
  int64 num;
  int width;
  

  /* Constructors */
  sc_int_base() : num(0),width(LLWIDTH) { } 
  explicit sc_int_base(int w) : num(0),width(w) { }
  sc_int_base(int64 value, int w) : width(w) { int S = (LLWIDTH - w); num = value << S >> S; }
  sc_int_base( const sc_int_base& v ) { width = v.width; num = v.num; }
  sc_int_base( const sc_int_subref& v );
#if !defined(__SUNPRO_CC)
  template <class T1, class T2> sc_int_base( const sc_int_concat<T1, T2>& v ) 
  { 
      width = v.bitwidth();
      int S = LLWIDTH - width;
      num = (v.operator int64()) << S >> S;
  }
#endif
  sc_int_base( const sc_bool_vector& v); 
  sc_int_base( const sc_logic_vector& v );  
  sc_int_base( const sc_signed& v);
  sc_int_base( const sc_unsigned& v);

  // Destructor
  ~sc_int_base() { }
  
  // Assignment operators
  sc_int_base& operator=(int64 v ) { int S = (LLWIDTH - width); num = v << S >> S; return *this; }
  sc_int_base& operator=( const sc_int_base& v ) { int S = (LLWIDTH - width); num = v.num << S >> S; return *this; }
  sc_int_base& operator=( const sc_int_subref& v); 
#if !defined(__SUNPRO_CC)
  template< class T1, class T2 > sc_int_base& operator=( const sc_int_concat<T1, T2>& v)
  {
      int S = LLWIDTH - width;
      num = (v.operator int64()) << S >> S;
      return *this;
  }
#endif
  sc_int_base& operator=( const sc_bool_vector& v );
  sc_int_base& operator=( const sc_logic_vector& v );
  sc_int_base& operator=( const sc_signed& v );
  sc_int_base& operator=( const sc_unsigned& v );
#ifdef SC_INCLUDE_FX
  sc_int_base& operator = ( const sc_fxval& );
  sc_int_base& operator = ( const sc_fxval_fast& );
  sc_int_base& operator = ( const sc_fxnum& );
  sc_int_base& operator = ( const sc_fxnum_fast& );
#endif
  
  operator int64() const { return num; }

  unsigned int to_uint() const {return (unsigned int)num; }
  int to_int() const {return (int)num; }
  long long_low() const { 
#ifndef WIN32
      return (long)(num & 0x00000000ffffffffULL);
#else
      return (long)(num & 0x00000000ffffffffi64);
#endif
  }

  long long_high() const {
#ifndef WIN32
      return (long)( (num >> 32)  & 0x00000000ffffffffULL );
#else
      return (long)( (num >> 32)  & 0x00000000ffffffffi64 );
#endif
  }

  //Arithmetic operators
  sc_int_base& operator+=(int64 v) 
  { 
      int64 val = num;
      int S = LLWIDTH - width; 
      val += v;
      num = val << S >> S;
      return *this; 
  }
  sc_int_base& operator-=(int64 v) 
  { 
      int64 val = num;
      int S = LLWIDTH - width; 
      val -= v;
      num = val << S >> S;
      return *this;
  }
  
  sc_int_base& operator*=(int64 v) 
  { 
    int64 val = num;
    int S = LLWIDTH - width; 
    val *= v;
    num = val << S >> S; 
    return *this;
  }
  
  sc_int_base& operator/=(int64 v) 
  { 
    int64 val = num;
    int S = LLWIDTH - width; 
    val /= v; 
    num = val << S >> S; 
    return *this; 
  }
  sc_int_base& operator%=(int64 v)
  { 
    int64 val = num;
    int S = LLWIDTH - width;
    val %= v; 
    num = val << S >> S;
    return *this; 
  }
  
  //Bitwise operators
  sc_int_base& operator&=(int64 v) 
  { 
    int64 val = num;
    int S = LLWIDTH - width; 
    val &= v;
    num = val << S >> S;
    return *this;
  }
  sc_int_base& operator|=(int64 v) 
  {
    int64 val = num;
    int S = LLWIDTH - width; 
    val |= v;
    num = val << S >> S;
    return *this;
  }

  sc_int_base& operator^=(int64 v) 
  {
    int64 val = num;
    int S = LLWIDTH - width; 
    val ^= v;
    num = val << S >> S;
    return *this;
  }
  sc_int_base& operator<<=(int64 v) 
  {
    int64 val = num;
    int S = LLWIDTH - width; 
    val <<= v;
    num = val << S >> S;
    return *this;
  }

  sc_int_base& operator>>=(int64 v) 
  {
    num >>= v;  //No sign extension needed.
    return *this;
  }
  
  //postfix and prefix increment & decrement operators
  sc_int_base& operator++() // prefix
  {
    int64 val = num;
    int S = LLWIDTH - width;
    ++val;
    num = val << S >> S;
    return *this;
  }

  const sc_int_base operator++(int) // postfix
  {
    int64 val = num;
    int S = LLWIDTH - width;
    int64 tmp = val;
    val++;
    num = val << S >> S;
    return sc_int_base(tmp, width);
  }

  sc_int_base& operator--() //prefix
  {
    int64 val = num;
    int S = LLWIDTH - width;
    --val;
    num = val << S >> S; 
    return *this;
  }
  
  const sc_int_base operator--(int) //postfix 
  {
    int64 val = num;  
    int S = LLWIDTH - width;
    int64 tmp = val;
    val--;
    num = val << S >> S;
    return sc_int_base(tmp, width);
  }


  // Bit-select and part-select operators
  
  sc_int_bitref operator[] ( int i ); 
  bit operator[] (int i) const { return test(i); }
  sc_int_subref range(int left, int right ); 
  int64 range( int left, int right ) const;

  bit test(int i) const 
  {
#ifndef WIN32
      return (  0 != ( num & (1ULL << i) ) ); 
#else
      return (  0 != ( num & (1i64 << i) ) );
#endif
  }  
  void set(int i) 
  {
#ifndef WIN32
      num |= (1ULL << i); 
#else
      num |= (1i64 << i);
#endif
  }

  void set(int i, bit v )
  {
#ifndef WIN32
    int64 val = num;  
    val &= ~ (1ULL << i );
    val |= ((v & 1ULL) << i);
    num = val;
#else
    int64 val = num;
    val &= ~ (1i64<< i );
    val |= ((v & 1i64) << i);
    num = val;
#endif
  }

  int bitwidth() const { return width; }

  friend bool operator == ( const sc_int_base& a, const sc_int_base& b )
    { return a.num == b.num; }
  friend bool operator != ( const sc_int_base& a, const sc_int_base& b )
    { return a.num != b.num; }
  friend bool operator < ( const sc_int_base& a, const sc_int_base& b )
    { return a.num < b.num; }
  friend bool operator <= ( const sc_int_base& a, const sc_int_base& b )
    { return a.num <= b.num; }
  friend bool operator > ( const sc_int_base& a, const sc_int_base& b )
    { return a.num > b.num; }
  friend bool operator >= ( const sc_int_base& a, const sc_int_base& b )
    { return a.num >= b.num; }

};


// Template class sc_int<W> is the interface that the user sees. 
// It is derived from sc_int_base and most of its methods are
// just wrappers that call the corresponding method in the 
// parent class. Note that the width of sc_int datatype is specified
// as a template parameter.

template< int W >
class sc_int : public sc_int_base {
 friend class sc_int_bitref;
 friend class sc_int_subref;

public:
  // Constructors
  sc_int() : sc_int_base(W) { }
  sc_int(int64 v) : sc_int_base(W) { num = v << (LLWIDTH - W) >> (LLWIDTH - W); }
  sc_int( const sc_int<W>& v) : sc_int_base(W) { num = v.num ; }
  sc_int( const sc_int_base& v ) : sc_int_base(W) { num = v.num << (LLWIDTH - W) >> (LLWIDTH - W); }
  sc_int( const sc_int_subref& v ) : sc_int_base( v ) { }
#if !defined(__SUNPRO_CC)
  template<class T1, class T2> sc_int( const sc_int_concat<T1,T2>& v) : sc_int_base(v) { }
#endif
  sc_int( const sc_bool_vector& v) : sc_int_base(v) { }
  sc_int( const sc_logic_vector& v ) : sc_int_base(v) { }
  sc_int( const sc_signed& v ) : sc_int_base(v) { }
  sc_int( const sc_unsigned& v) : sc_int_base(v) { }
#ifdef SC_INCLUDE_FX
  explicit sc_int( const sc_fxval&  );
  explicit sc_int( const sc_fxval_fast& );
  explicit sc_int( const sc_fxnum&  );
  explicit sc_int( const sc_fxnum_fast& );
#endif
  sc_int( const sc_bv<W>& v);
  sc_int( const sc_lv<W>& v );

  //Assignment operators
 sc_int& operator=(int64 v) { num = v << (LLWIDTH - W) >> (LLWIDTH - W); return *this; }
 sc_int& operator=( const sc_int_base& v ) { num = v.num << (LLWIDTH - W) >> (LLWIDTH - W); return *this; }
 sc_int& operator=( const sc_int_subref& v ) { sc_int_base::operator=(v); return *this; }
 sc_int& operator=( const sc_int<W>& v) { num = v.num; return *this; }
#if !defined(__SUNPRO_CC)
 template<class T1, class T2> sc_int& operator=( const sc_int_concat<T1, T2>& v) { sc_int_base::operator=(v); return *this; }
#endif
 sc_int& operator=( const sc_bool_vector& v) { sc_int_base::operator=(v); return *this; }
 sc_int& operator=( const sc_logic_vector& v) { sc_int_base::operator=(v); return *this; }
 sc_int& operator=( const sc_signed& v) { sc_int_base::operator=(v); return *this;}
 sc_int& operator=( const sc_unsigned& v) { sc_int_base::operator=(v); return *this; }
#ifdef SC_INCLUDE_FX
  sc_int& operator = ( const sc_fxval& );
  sc_int& operator = ( const sc_fxval_fast& );
  sc_int& operator = ( const sc_fxnum& );
  sc_int& operator = ( const sc_fxnum_fast& );
#endif
 sc_int& operator=( const sc_bv<W>& v);
 sc_int& operator=( const sc_lv<W>& v);

    // Use sc_int_base's operator int64() const;
    // Note that g++ does not compile this correctly; if
    // we define operator int64() here, g++ fails to make
    // it mask the one defined in the base class.
    // operator int64() const { return num; }
 
 //Arithmetic operators
  sc_int& operator+=(int64 v) 
  { 
    int64 val = num;
    val += v;
    num = val << (LLWIDTH - W) >> (LLWIDTH - W);
    return *this;
  }

  sc_int& operator-=(int64 v) 
  {
    int64 val = num;
    val -= v;
    num = val << (LLWIDTH - W) >> (LLWIDTH - W);
    return *this;
  }

  sc_int& operator*=(int64 v) 
  {
    int64 val = num;
    val *= v;
    num = val << (LLWIDTH - W) >> (LLWIDTH - W);
    return *this;
  }

  sc_int& operator/=(int64 v) 
  {
    int64 val = num;
    val /= v;
    num = val << (LLWIDTH - W) >> (LLWIDTH - W);
    return *this;
  }

  sc_int& operator%=(int64 v) 
  {
    int64 val = num;
    val %= v;
    num = val << (LLWIDTH - W) >> (LLWIDTH - W);
    return *this;
  }
  
  //Bitwise operators
  sc_int& operator&=(int64 v) 
  {
    int64 val = num;
    val &= v;
    num = val << (LLWIDTH - W) >> (LLWIDTH - W);
    return *this;
  }

  sc_int& operator|=(int64 v) 
  {
    int64 val = num;
    val |= v;
    num = val << (LLWIDTH - W) >> (LLWIDTH - W);
    return *this;
  }

  sc_int& operator^=(int64 v) 
  {
    int64 val = num;
    val ^= v;
    num = val << (LLWIDTH - W) >> (LLWIDTH - W);
    return *this;
  }

  sc_int& operator<<=(int64 v) 
  {
    int64 val = num;
    val <<= v;
    num = val << (LLWIDTH - W) >> (LLWIDTH - W);
    return *this;
  }

  sc_int& operator>>=(int64 v) { num >>= v; return *this; }
  
  //postfix and prefix increment & decrement operators
  sc_int& operator++() // prefix
  {
    int64 val = num;  
    ++val;
    num = val << (LLWIDTH - W) >> (LLWIDTH - W);
    return *this;
  }

  const sc_int operator++(int) // postfix
  {
    int64 val = num;
    int64 tmp = val;
    val++;
    num = val << (LLWIDTH - W) >> (LLWIDTH - W);
    return sc_int<W>(tmp);
  }

  sc_int& operator--() //prefix
  {
    int64 val = num;
    --val;
    num = val << (LLWIDTH - W) >> (LLWIDTH - W); 
    return *this;
  }
  
  const sc_int operator--(int) //postfix 
  {
    int64 val = num;
    int64 tmp = val;
    val--;
    num = val << (LLWIDTH - W) >> (LLWIDTH - W);
    return sc_int<W>(tmp);
  }

 // inherit bit-select and part-select operators from sc_int_base
    
 // concatenation operators are global functions that are friends of sc_int_base

}; 




// Class that implements bit-selection on sc_int

class sc_int_bitref {
  friend class sc_int_base;
public:

  sc_int_bitref& operator=( const sc_int_bitref& b)
  {
    int64 val;
    int len = obj->width;

    obj->set(index, (b.obj)->test(b.index) );
    if( index == len - 1 ){
	val = obj->num;
	val = val << (LLWIDTH - len) >> (LLWIDTH - len);
	obj->num = val;
    }
    return *this;
  }
   
  sc_int_bitref& operator=( bit b )
  {
    int64 val;
    int len = obj->width;

    obj->set(index, b );
    if( index == len - 1 ){
	val = obj->num;
	val = val << (LLWIDTH - len) >> (LLWIDTH - len);
	obj->num = val;
    }
    return *this;
  }
 
  
  sc_int_bitref& operator&=( bit b )
  {
    int64 val;
    int len = obj->width;

    obj->num &= ~ ( ( !b ) << index );
    if( index == len - 1 ){
	val = obj->num;
	val = val << (LLWIDTH - len) >> (LLWIDTH - len);
	obj->num = val;
    }
    return *this;
  }

  sc_int_bitref& operator|=( bit b )
  {
      int64 val;
      int len = obj->width;
#ifndef WIN32
    obj->num |= ( (b & 1ULL) << index );
#else
    obj->num |= ( (b & 1i64) << index );
#endif
    if( index == len - 1 ) {
	val = obj->num;
	val = val << (LLWIDTH - len) >> (LLWIDTH - len);
	obj->num = val;
    }
    return *this;
  }
  sc_int_bitref& operator^=( bit b )
  {
    int64 val;
    int len = obj->width;
#ifndef WIN32
    obj->num ^= ((b & 1ULL ) << index );
#else
    obj->num ^= ((b & 1i64 ) << index );
#endif
    if ( index == len - 1 ){
	val = obj->num;
	val = val << (LLWIDTH - len) >> (LLWIDTH - len);
	obj->num = val;
    }
    return *this;
  }
  
  operator bit() const {return obj->test(index); }
  bit operator!() const {return ! obj->test(index); }

  int bitwidth() const { return 1; }
 
/*  // gcc 2.95.2 bug - bool comparison mapped into int comparison
  // must provide explicit conversion, otherwise gcc is confused
  bool operator==(bool b)const {return (bool)(*this)==b;}
  // gcc 2.95.2 bug - all the other operators are affected by bool->int
  // conversion. Must provide implicit int conversion to avoid it
  operator int() const {return obj->test(index); }
*/
private:
  sc_int_base* obj;
  int index;
  
  sc_int_bitref( sc_int_base* u, int li ) { obj = u; index = li; }
};
/*
// gcc 2.95.2 bug - bool comparison mapped into int comparison
// must provide explicit conversion, otherwise gcc is confused
inline bool operator==(bool b,sc_int_bitref& t){return t==b;}
*/
// Class that implements part-selection on sc_int

class sc_int_subref {
  friend class sc_int_base;
  
public:
  
  sc_int_subref& operator=( const sc_int_base& v)
  { 
    int64 val = obj->num;
    int len = obj->width;

    // Mask the bits out that are part-selected
    val &= mask64[left][right];

    // set the part-selected bits
    val |= (v.num << right) & ~mask64[left][right];
    
    // normalize the value
    val = val << (LLWIDTH - len) >> (LLWIDTH - len);

    obj->num = val;
    return *this;
  }
    
  sc_int_subref& operator=( const sc_int_subref& v)
  {
    if( this == &v )
      return *this;
    return operator=( v.operator int64() );
  }
  
  sc_int_subref& operator=( int64 v )
  {
    int64 val = obj->num;
    int len = obj->width;
    
    // Mask the bits out that are part-selected
    val &= mask64[left][right];

    //Set the part-selected bits
    val |= (v << right) & ~mask64[left][right];

    // Normalize the value
    val = val << (LLWIDTH - len) >> (LLWIDTH - len);

    obj->num = val;
    return *this;
  }
  
  operator int64() const 
  { 
    int64 val = obj->num;
    if( left == obj->width - 1 )
      return val >> right;
    else
      return val << (LLWIDTH - (left + 1)) >> (LLWIDTH -( left + 1) + right); 
  }

  int bitwidth() const { return left - right + 1; }

private:
  sc_int_base* obj;
  int left;
  int right;

  sc_int_subref( sc_int_base* u, int l, int r) { obj = u; left = l; right = r; }
};

// Proxy class to implement concatenation

template<class T1, class T2>
class sc_int_concat {

public:

  sc_int_concat( T1& l, T2& r ) : left(l), right(r) 
  {
    len = l.bitwidth() + r.bitwidth();
  }

  sc_int_concat( const sc_int_concat<T1,T2>& v ) : left(v.left), right(v.right), len(v.len)
  {
      
  }

  // Sun's SC5.0 doesn't support member templates
#if !defined(__SUNPRO_CC)
  template<class T3, class T4>
  sc_int_concat<T1,T2>& operator=( const sc_int_concat<T3,T4>& v ) // lvalue use
  {
      return operator=(v.operator int64());
  }
#else
  //  operator const sc_int_base() {return sc_int_base(operator int64());}
#endif

  sc_int_concat& operator=(int64 v)
  {
#ifndef WIN32
    int64 mask = (1ULL << right.bitwidth() )-1;
#else
    int64 mask = (1i64 << right.bitwidth() )-1;
#endif
    right = v & mask;
    left = (v & ~mask) >> right.bitwidth();
    return *this;
  }
    

 sc_int_concat& operator=( const sc_int_base& v )
 {
   return operator=(v.operator int64() );
 }

  sc_int_concat& operator=( const sc_int_subref& v)
  {
    return operator=( v.operator int64() );
  }   

  operator int64() const 
  {   
#ifndef WIN32
    int64 mask = (1ULL << right.bitwidth() )-1;
#else
    int64 mask = (1i64 << right.bitwidth() )-1;
#endif
    int64 high = (left << right.bitwidth() ) & (~mask);
    int64 low  = right & mask;
    int64 tmp = high | low;
    return ( tmp << (LLWIDTH - len) >> (LLWIDTH - len) );
  }

  int bitwidth() const { return len; }

private:
  T1& left;
  T2& right;
  int len;


};

#include "sc_int_defs.h"

#else 

#include "sc_int32.h"

#endif // end of #ifndef _32BIT_ 

#if defined(WIN32) && !defined(__BCPLUSPLUS__)
extern ostream& operator<<(ostream& os, int64);
#endif

#endif
