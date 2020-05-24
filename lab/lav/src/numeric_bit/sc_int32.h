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


#ifndef SC_INT32_H
#define SC_INT32_H

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

extern const unsigned int  mask32[INTWIDTH][INTWIDTH];


//Base class for sc_int. 

class sc_int_base {
  
  friend class sc_int_bitref;
  friend class sc_int_subref;
  
public:
  int num;
  int width;
  

  /* Constructors */
  sc_int_base() : num(0),width(INTWIDTH) { } 
  explicit sc_int_base(int w) : num(0),width(w) { }
  sc_int_base(int value, int w) : width(w) { int S = (INTWIDTH - w); num = value << S >> S; }
  sc_int_base( const sc_int_base& v ) { width = v.width; num = v.num; }
  sc_int_base( const sc_int_subref& v );
#if !defined(__SUNPRO_CC)
  template <class T1, class T2> sc_int_base( const sc_int_concat<T1, T2>& v ) 
  { 
      width = v.bitwidth();
      int S = INTWIDTH - width;
      num = (v.operator int()) << S >> S;
  }
#endif
  sc_int_base( const sc_bool_vector& v); 
  sc_int_base( const sc_logic_vector& v );  
  sc_int_base( const sc_signed& v);
  sc_int_base( const sc_unsigned& v);

  // Destructor
  ~sc_int_base() { }
  
  // Assignment operators
  sc_int_base& operator=(int v ) { int S = (INTWIDTH - width); num = v << S >> S; return *this; }
  sc_int_base& operator=( const sc_int_base& v ) { int S = (INTWIDTH - width); num = v.num << S >> S; return *this; }
  sc_int_base& operator=( const sc_int_subref& v); 
#if !defined(__SUNPRO_CC)
  template< class T1, class T2 > sc_int_base& operator=( const sc_int_concat<T1, T2>& v)
  {
      int S = INTWIDTH - width;
      num = (v.operator int()) << S >> S;
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
  
  operator int() const { return num; }

  unsigned int to_uint() const {return (unsigned int)num; }
  int to_int() const {return (int)num; }
  

  //Arithmetic operators
  sc_int_base& operator+=(int v) 
  { 
      int val = num;
      int S = INTWIDTH - width; 
      val += v;
      num = val << S >> S;
      return *this; 
  }
  sc_int_base& operator-=(int v) 
  { 
      int val = num;
      int S = INTWIDTH - width; 
      val -= v;
      num = val << S >> S;
      return *this;
  }
  
  sc_int_base& operator*=(int v) 
  { 
    int val = num;
    int S = INTWIDTH - width; 
    val *= v;
    num = val << S >> S; 
    return *this;
  }
  
  sc_int_base& operator/=(int v) 
  { 
    int val = num;
    int S = INTWIDTH - width; 
    val /= v; 
    num = val << S >> S; 
    return *this; 
  }
  sc_int_base& operator%=(int v)
  { 
    int val = num;
    int S = INTWIDTH - width;
    val %= v; 
    num = val << S >> S;
    return *this; 
  }
  
  //Bitwise operators
  sc_int_base& operator&=(int v) 
  { 
    int val = num;
    int S = INTWIDTH - width; 
    val &= v;
    num = val << S >> S;
    return *this;
  }
  sc_int_base& operator|=(int v) 
  {
    int val = num;
    int S = INTWIDTH - width; 
    val |= v;
    num = val << S >> S;
    return *this;
  }

  sc_int_base& operator^=(int v) 
  {
    int val = num;
    int S = INTWIDTH - width; 
    val ^= v;
    num = val << S >> S;
    return *this;
  }
  sc_int_base& operator<<=(int v) 
  {
    int val = num;
    int S = INTWIDTH - width; 
    val <<= v;
    num = val << S >> S;
    return *this;
  }

  sc_int_base& operator>>=(int v) 
  {
    num >>= v;  //No sign extension needed.
    return *this;
  }
  
  //postfix and prefix increment & decrement operators
  sc_int_base& operator++() // prefix
  {
    int val = num;
    int S = INTWIDTH - width;
    ++val;
    num = val << S >> S;
    return *this;
  }

  const sc_int_base operator++(int) // postfix
  {
    int val = num;
    int S = INTWIDTH - width;
    int tmp = val;
    val++;
    num = val << S >> S;
    return sc_int_base(tmp, width);
  }

  sc_int_base& operator--() //prefix
  {
    int val = num;
    int S = INTWIDTH - width;
    --val;
    num = val << S >> S; 
    return *this;
  }
  
  const sc_int_base operator--(int) //postfix 
  {
    int val = num;  
    int S = INTWIDTH - width;
    int tmp = val;
    val--;
    num = val << S >> S;
    return sc_int_base(tmp, width);
  }


  // Bit-select and part-select operators
  
  sc_int_bitref operator[] ( int i ); 
  bit operator[] (int i) const { return test(i); }
  sc_int_subref range(int left, int right ); 
  int range( int left, int right ) const;

  bit test(int i) const 
  {
      return (  0 != ( num & (1U << i) ) ); 
  }  
  void set(int i) 
  {
      num |= (1U << i); 
  }

  void set(int i, bit v )
  {
    int val = num;  
    val &= ~ (1U << i );
    val |= ((v & 1U) << i);
    num = val;
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
  sc_int(int v) : sc_int_base(W) { num = v << (INTWIDTH - W) >> (INTWIDTH - W); }
  sc_int( const sc_int<W>& v) : sc_int_base(W) { num = v.num ; }
  sc_int( const sc_int_base& v ) : sc_int_base(W) { num = v.num << (INTWIDTH - W) >> (INTWIDTH - W); }
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
 sc_int& operator=(int v) { num = v << (INTWIDTH - W) >> (INTWIDTH - W); return *this; }
 sc_int& operator=( const sc_int_base& v ) { num = v.num << (INTWIDTH - W) >> (INTWIDTH - W); return *this; }
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

    // Use sc_int_base's operator int() const;
    // Note that g++ does not compile this correctly; if
    // we define operator int() here, g++ fails to make
    // it mask the one defined in the base class.
    // operator int() const { return num; }
 
 //Arithmetic operators
  sc_int& operator+=(int v) 
  { 
    int val = num;
    val += v;
    num = val << (INTWIDTH - W) >> (INTWIDTH - W);
    return *this;
  }

  sc_int& operator-=(int v) 
  {
    int val = num;
    val -= v;
    num = val << (INTWIDTH - W) >> (INTWIDTH - W);
    return *this;
  }

  sc_int& operator*=(int v) 
  {
    int val = num;
    val *= v;
    num = val << (INTWIDTH - W) >> (INTWIDTH - W);
    return *this;
  }

  sc_int& operator/=(int v) 
  {
    int val = num;
    val /= v;
    num = val << (INTWIDTH - W) >> (INTWIDTH - W);
    return *this;
  }

  sc_int& operator%=(int v) 
  {
    int val = num;
    val %= v;
    num = val << (INTWIDTH - W) >> (INTWIDTH - W);
    return *this;
  }
  
  //Bitwise operators
  sc_int& operator&=(int v) 
  {
    int val = num;
    val &= v;
    num = val << (INTWIDTH - W) >> (INTWIDTH - W);
    return *this;
  }

  sc_int& operator|=(int v) 
  {
    int val = num;
    val |= v;
    num = val << (INTWIDTH - W) >> (INTWIDTH - W);
    return *this;
  }

  sc_int& operator^=(int v) 
  {
    int val = num;
    val ^= v;
    num = val << (INTWIDTH - W) >> (INTWIDTH - W);
    return *this;
  }

  sc_int& operator<<=(int v) 
  {
    int val = num;
    val <<= v;
    num = val << (INTWIDTH - W) >> (INTWIDTH - W);
    return *this;
  }

  sc_int& operator>>=(int v) { num >>= v; return *this; }
  
  //postfix and prefix increment & decrement operators
  sc_int& operator++() // prefix
  {
    int val = num;  
    ++val;
    num = val << (INTWIDTH - W) >> (INTWIDTH - W);
    return *this;
  }

  const sc_int operator++(int) // postfix
  {
    int val = num;
    int tmp = val;
    val++;
    num = val << (INTWIDTH - W) >> (INTWIDTH - W);
    return sc_int(tmp);
  }

  sc_int& operator--() //prefix
  {
    int val = num;
    --val;
    num = val << (INTWIDTH - W) >> (INTWIDTH - W); 
    return *this;
  }
  
  const sc_int operator--(int) //postfix 
  {
    int val = num;
    int tmp = val;
    val--;
    num = val << (INTWIDTH - W) >> (INTWIDTH - W);
    return sc_int(tmp);
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
    int val;
    int len = obj->width;

    obj->set(index, (b.obj)->test(b.index) );
    if( index == len - 1 ){
	val = obj->num;
	val = val << (INTWIDTH - len) >> (INTWIDTH - len);
	obj->num = val;
    }
    return *this;
  }
   
  sc_int_bitref& operator=( bit b )
  {
    int val;
    int len = obj->width;

    obj->set(index, b );
    if( index == len - 1 ){
	val = obj->num;
	val = val << (INTWIDTH - len) >> (INTWIDTH - len);
	obj->num = val;
    }
    return *this;
  }
 
  
  sc_int_bitref& operator&=( bit b )
  {
    int val;
    int len = obj->width;

    obj->num &= ~ ( ( !b ) << index );
    if( index == len - 1 ){
	val = obj->num;
	val = val << (INTWIDTH - len) >> (INTWIDTH - len);
	obj->num = val;
    }
    return *this;
  }

  sc_int_bitref& operator|=( bit b )
  {
      int val;
      int len = obj->width;
      obj->num |= ( (b & 1U) << index );
      if( index == len - 1 ) {
	val = obj->num;
        val = val << (INTWIDTH - len) >> (INTWIDTH - len);
	obj->num = val;
      }
    return *this;
  }
  sc_int_bitref& operator^=( bit b )
  {
    int val;
    int len = obj->width;

    obj->num ^= ((b & 1U ) << index );

    if ( index == len - 1 ){
	val = obj->num;
	val = val << (INTWIDTH - len) >> (INTWIDTH - len);
	obj->num = val;
    }
    return *this;
  }
  
  operator bit() const {return obj->test(index); }
  bit operator!() const {return ! obj->test(index); }

  int bitwidth() const { return 1; }
 /* TODO 5 -oGene -crelease preparation : remove gcc 2.95.2 ... portions of commented code before release */
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
    int val = obj->num;
    int len = obj->width;

    // Mask the bits out that are part-selected
    val &= mask32[left][right];

    // set the part-selected bits
    val |= (v.num << right) & ~mask32[left][right];
    
    // normalize the value
    val = val << (INTWIDTH - len) >> (INTWIDTH - len);

    obj->num = val;
    return *this;
  }
    
  sc_int_subref& operator=( const sc_int_subref& v)
  {
    if( this == &v )
      return *this;
    return operator=( v.operator int() );
  }
  
  sc_int_subref& operator=( int v )
  {
    int val = obj->num;
    int len = obj->width;
    
    // Mask the bits out that are part-selected
    val &= mask32[left][right];

    //Set the part-selected bits
    val |= (v << right) & ~mask32[left][right];

    // Normalize the value
    val = val << (INTWIDTH - len) >> (INTWIDTH - len);

    obj->num = val;
    return *this;
  }
  
  operator int() const 
  { 
    int val = obj->num;
    if( left == obj->width - 1 )
      return val >> right;
    else
      return val << (INTWIDTH - (left + 1)) >> (INTWIDTH -( left + 1) + right); 
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
      return operator=(v.operator int());
  }
#else
  //  operator const sc_int_base() {return sc_int_base(operator int());}
#endif

  sc_int_concat& operator=(int v)
  {
    int mask = (1U << right.bitwidth() )-1;

    right = v & mask;
    left = (v & ~mask) >> right.bitwidth();
    return *this;
  }
    

 sc_int_concat& operator=( const sc_int_base& v )
 {
   return operator=(v.operator int() );
 }

  sc_int_concat& operator=( const sc_int_subref& v)
  {
    return operator=( v.operator int() );
  }   

  operator int() const 
  {   

    int mask = (1U << right.bitwidth() )-1;
    int high = (left << right.bitwidth() ) & (~mask);
    int low  = right & mask;
    int tmp = high | low;
    return ( tmp << (INTWIDTH - len) >> (INTWIDTH - len) );
  }

  int bitwidth() const { return len; }

private:
  T1& left;
  T2& right;
  int len;


};

#include "sc_int32_defs.h"

#endif
