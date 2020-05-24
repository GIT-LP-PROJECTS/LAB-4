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

    sc_uint.h -- A sc_int is an unsigned integer whose width is less
    than the machine's native integer width. We provide two
    implementations (i) sc_uint with bitwidth between 1 - 64 (ii)
    sc_uint with bitwidth between 1 - 32 Implementation (i) is the
    default implementation, while implementation (ii) can be used only
    if compiled with -D_32BIT_.  Unlike AP, arithmetic and bitwise
    operations are performed using the native types (hence capped at
    32/64 bits). The sc_uint integer is useful when the user does not
    need AP and the performance is superior to sc_bigint/sc_biguint

    Original Author: Amit Rao. Synopsys, Inc. (arao@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_UINT_H
#define SC_UINT_H

#ifndef _32BIT_

#include "sc_nbdefs.h"

#define LLSIZE 64
#ifndef WIN32
#define MASK(W) ( (1ULL<<W)-1 )
#else
#define MASK(W) ( (1i64<<W)-1 )
#endif

// Forward declarations
class sc_uint_bitref;
class sc_uint_subref;
class sc_uint_base;
class sc_bool_vector;
class sc_logic_vector;
class sc_signed;
class sc_unsigned;

template<class T1, class T2> class sc_uint_concat;
template<int W> class sc_uint;
namespace sc_bv_ns
{
template<int W> class sc_bv;
template<int W> class sc_lv;
}
using sc_bv_ns::sc_bv;
using sc_bv_ns::sc_lv;

extern const uint64 mask64[LLWIDTH][LLWIDTH];

//Base class for sc_uint. 

class sc_uint_base {
  
  friend class sc_uint_bitref;
  friend class sc_uint_subref;
  
public:
  uint64 num;
  int width;
  

  /* Constructors */
  sc_uint_base() : num(0),width(LLWIDTH) { } 
  explicit sc_uint_base(int w) : num(0),width(w) { }
  sc_uint_base(uint64 value, int w) : width(w) { num = value & MASK(w); }
  sc_uint_base( const sc_uint_base& v ) { width = v.width; num = v.num; }
  sc_uint_base( const sc_uint_subref& v );
#if !defined(__SUNPRO_CC)
  template<class T1, class T2> sc_uint_base( const sc_uint_concat<T1,T2>& v )
  {
      width = v.bitwidth();
      num = (v.operator uint64()) & MASK(width);
  }
#endif
  sc_uint_base( const sc_bool_vector& v );
  sc_uint_base( const sc_logic_vector& v );
  sc_uint_base( const sc_signed& v );
  sc_uint_base( const sc_unsigned& v );

  // Destructor
  ~sc_uint_base() { }
  
  // Assignment operators
  sc_uint_base& operator=( uint64 v ) { num = v & MASK(width); return *this; }
  sc_uint_base& operator=( const sc_uint_subref& v); 
#if !defined(__SUNPRO_CC)
  template<class T1, class T2> sc_uint_base& operator=( const sc_uint_concat<T1,T2>& v)
  {
      width = v.bitwidth();
      num = (v.operator uint64()) & MASK(width);
      return *this;
  }
#endif
  sc_uint_base& operator=( const sc_bool_vector& v );
  sc_uint_base& operator=( const sc_logic_vector& v );
  sc_uint_base& operator=( const sc_signed& v );
  sc_uint_base& operator=( const sc_unsigned& v);
#ifdef SC_INCLUDE_FX
  sc_uint_base& operator = ( const sc_fxval& );
  sc_uint_base& operator = ( const sc_fxval_fast& );
  sc_uint_base& operator = ( const sc_fxnum& );
  sc_uint_base& operator = ( const sc_fxnum_fast& );
#endif

  operator uint64() const { return num; }
  
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
  sc_uint_base& operator+=( uint64 v) 
  {
    uint64 val = num;
    val += v;
    num = val & MASK(width);
    return *this;
  }

  sc_uint_base& operator-=(uint64 v) 
  {
    uint64 val = num;
    val -= v;
    num = val & MASK(width);
    return *this;
  }
    
  sc_uint_base& operator*=(uint64 v) 
  {
    uint64 val = num;
    val *= v;
    num = val & MASK(width);
    return *this;
  }

  sc_uint_base& operator/=(uint64 v) 
 {
    uint64 val = num;
    val /= v;
    num = val & MASK(width);
    return *this;
  }

  sc_uint_base& operator%=(uint64 v) 
  {
    uint64 val = num;
    val %= v;
    num = val & MASK(width);
    return *this;
  }
  
  //Bitwise operators
  sc_uint_base& operator&=(uint64 v) 
  {
    uint64 val = num;
    val &= v;
    num = val & MASK(width);
    return *this;
  }

  sc_uint_base& operator|=(uint64 v) 
  {
    uint64 val = num;
    val |= v;
    num = val & MASK(width);
    return *this;
  }

  sc_uint_base& operator^=(uint64 v) 
  {
    uint64 val = num;
    val ^= v;
    num = val & MASK(width);
    return *this;
  }

  sc_uint_base& operator<<=(uint64 v) 
  {
    
    uint64 val = num;
    val <<= v;
    num = val & MASK(width);
    return *this;
  }

  sc_uint_base& operator>>=(uint64 v) 
  {
    uint64 val = num;
    val >>= v;
    num = val & MASK(width);
    return *this;
  }
  
  //postfix and prefix increment & decrement operators
  sc_uint_base& operator++() // prefix
  {
    uint64 val = num;
    ++val;
    val &= MASK(width); 
    num = val;
    return *this;
  }

  const sc_uint_base operator++(int) // postfix
  {
    uint64 val = num;
    uint64 tmp = val;
    val++;
    val &= MASK(width); 
    num = val;
    return sc_uint_base(tmp, width);
  }

  sc_uint_base& operator--() //prefix
  {
    uint64 val = num;
    --val;
    val &= MASK(width);
    num = val;
    return *this;
  }
  
  const sc_uint_base operator--(int) //postfix 
  {
    uint64 val = num;
    uint64 tmp = val;
    val--;
    val &= MASK(width);
    num = val;
    return sc_uint_base(tmp, width);
  }


  // Bit-select and part-select operators
  
  sc_uint_bitref operator[] ( int i ); 
  bit operator[] (int i) const { return test(i); }
  sc_uint_subref range(int left, int right ); 
  uint64 range( int left, int right ) const;

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
    uint64 val = num;  
    val &= ~ (1ULL << i );
    val |= ((v & 1ULL) << i);
    num = val;
#else
    uint64 val = num;
    val &= ~ (1i64<< i );
    val |= ((v & 1i64) << i);
    num = val;
#endif
  }

  int bitwidth() const { return width; }

  friend bool operator == ( const sc_uint_base& a, const sc_uint_base& b )
    { return a.num == b.num; }
  friend bool operator != ( const sc_uint_base& a, const sc_uint_base& b )
    { return a.num != b.num; }
  friend bool operator < ( const sc_uint_base& a, const sc_uint_base& b )
    { return a.num < b.num; }
  friend bool operator <= ( const sc_uint_base& a, const sc_uint_base& b )
    { return a.num <= b.num; }
  friend bool operator > ( const sc_uint_base& a, const sc_uint_base& b )
    { return a.num > b.num; }
  friend bool operator >= ( const sc_uint_base& a, const sc_uint_base& b )
    { return a.num >= b.num; }

};


// Template class sc_uint<W> is the interface that the user sees. 
// It is derived from sc_uint_base and most of its methods are
// just wrappers that call the corresponding method in the 
// parent class. Note that the width of sc_uint datatype is specified
// as a template parameter.

template< int W =LLSIZE>
class sc_uint : public sc_uint_base {
 friend class sc_uint_bitref;
 friend class sc_uint_subref;

public:
  // Constructors
  sc_uint() : sc_uint_base(W) { }
  sc_uint( uint64 v) : sc_uint_base(W) { num = v & MASK(W); }
  sc_uint( const sc_uint<W>& v) : sc_uint_base(W) { num = v.num ; }
  sc_uint( const sc_uint_base& v ) : sc_uint_base(W) { num = v.num & MASK(W); }
  sc_uint( const sc_uint_subref& v ) : sc_uint_base( v ) { }
#if !defined(__SUNPRO_CC)
  template<class T1, class T2> sc_uint( const sc_uint_concat<T1,T2>& v) : sc_uint_base(v) { }
#endif
  sc_uint( const sc_bool_vector& v) : sc_uint_base(v) { }
  sc_uint( const sc_logic_vector& v) : sc_uint_base(v) { }
  sc_uint( const sc_signed& v ) : sc_uint_base(v) { }
  sc_uint( const sc_unsigned& v ) : sc_uint_base(v) { }
#ifdef SC_INCLUDE_FX
  explicit sc_uint( const sc_fxval& );
  explicit sc_uint( const sc_fxval_fast& );
  explicit sc_uint( const sc_fxnum& );
  explicit sc_uint( const sc_fxnum_fast& );
#endif
  sc_uint( const sc_bv<W>& v);
  sc_uint( const sc_lv<W>& v );

 //Assignment operators
 sc_uint& operator=(uint64 v) { num = v & MASK(W); return *this; }
 sc_uint& operator=( const sc_uint_base& v ) { num = v.num & MASK(W); return *this; }
 sc_uint& operator=( const sc_uint_subref& v ) { sc_uint_base::operator=(v); return *this; }
 sc_uint& operator=( const sc_uint<W>& v) { num = v.num; return *this; }
#if !defined(__SUNPRO_CC)
 template<class T1, class T2> sc_uint& operator=( const sc_uint_concat<T1,T2>& v) 
   { sc_uint_base::operator=(v); return *this; }
#endif
 sc_uint& operator=( const sc_bool_vector& v ) { sc_uint_base::operator=(v); return *this; }
 sc_uint& operator=( const sc_logic_vector& v ) { sc_uint_base::operator=(v); return *this; }
 sc_uint& operator=( const sc_signed& v ) { sc_uint_base::operator=(v); return *this; }
 sc_uint& operator=( const sc_unsigned& v ) { sc_uint_base::operator=(v); return *this;}
#ifdef SC_INCLUDE_FX
  sc_uint& operator = ( const sc_fxval& );
  sc_uint& operator = ( const sc_fxval_fast& );
  sc_uint& operator = ( const sc_fxnum& );
  sc_uint& operator = ( const sc_fxnum_fast& );
#endif
 sc_uint& operator=( const sc_bv<W>& v);
 sc_uint& operator=( const sc_lv<W>& v);

    // operator uint64() const { return num; }
 
 //Arithmetic operators
  sc_uint& operator+=( uint64 v) 
  {
    uint64 val = num;
    val += v;
    num = val & MASK(W);
    return *this;
  }

  sc_uint& operator-=(uint64 v) 
  {
    uint64 val = num;
    val -= v;
    num = val & MASK(W);
    return *this;
  }

  sc_uint& operator*=(uint64 v) 
  {
    uint64 val = num;
    val *= v;
    num = val & MASK(W);
    return *this;
  }

  sc_uint& operator/=(uint64 v) 
  {
    uint64 val = num;
    val /= v;
    num = val & MASK(W);
    return *this;
  }

  sc_uint& operator%=(uint64 v) 
  {
    uint64 val = num;
    val %= v;
    num = val & MASK(W);
    return *this;
  }
  
  //Bitwise operators
  sc_uint& operator&=(uint64 v) 
  {
    uint64 val = num;
    val &= v;
    num = val & MASK(W);
    return *this;
  }

  sc_uint& operator|=(uint64 v) 
  {
    uint64 val = num;
    val |= v;
    num = val & MASK(W);
    return *this;
  }

  sc_uint& operator^=(uint64 v) 
  {
    uint64 val = num;
    val ^= v;
    num = val & MASK(W);
    return *this;
  }

  sc_uint& operator<<=(uint64 v) 
  {
    uint64 val = num;
    val <<= v;
    num = val & MASK(W);
    return *this;
  }

  sc_uint& operator>>=(uint64 v) { num >>= v; return *this; }
  
  //postfix and prefix increment & decrement operators
  sc_uint& operator++() // prefix
  {
    uint64 val = num;
    ++val;
    val &= MASK(W); 
    num = val;
    return *this;
  }

  const sc_uint operator++(int) // postfix
  {
    uint64 val = num;   
    uint64 tmp = val;
    val++;
    val &= MASK(W); 
    num = val;
    return sc_uint<W>(tmp);
  }

  sc_uint& operator--() //prefix
  {
    uint64 val = num;  
    --val;
    val &= MASK(W);
    num = val;
    return *this;
  }
  
  const sc_uint operator--(int) //postfix 
  {
    uint64 val = num;
    uint64 tmp = val;
    val--;
    val &= MASK(W);
    num = val;
    return sc_uint<W>(tmp);
  }     

 // inherit bit-select and part-select operators from sc_uint_base
    
 // conatenation operators are global functions that are friends of sc_uint_base

}; 

// Specialized class for sc_uints of bitwidth 64. We have to specialize this class
// because no masking is needed. 

template<>
class sc_uint<LLSIZE> : public sc_uint_base {
 friend class sc_uint_bitref;
 friend class sc_uint_subref;

public:
  // Constructors
  sc_uint() : sc_uint_base(LLSIZE) { }
  sc_uint( uint64 v) : sc_uint_base(v, LLSIZE) { }
  sc_uint( const sc_uint<LLSIZE>& v) { num = v.num ; }
  sc_uint( const sc_uint_base& v ) : sc_uint_base(LLSIZE) { num = v.num; }
  sc_uint( const sc_uint_subref& v ) : sc_uint_base( v ) { }
#if !defined(__SUNPRO_CC)
  template<class T1,class T2> sc_uint( const sc_uint_concat<T1, T2>& v) : sc_uint_base(v){}
#endif
  sc_uint( const sc_bool_vector& v) : sc_uint_base(v) { }
  sc_uint( const sc_logic_vector& v) : sc_uint_base(v) { }
  sc_uint( const sc_signed& v ) : sc_uint_base(v) { }
  sc_uint( const sc_unsigned& v ) : sc_uint_base(v) { }

 //Assignment operators
 sc_uint& operator=(uint64 v) { num = v; return *this; }
 sc_uint& operator=( const sc_uint_base& v ) { num = v.num; return *this; }
 sc_uint& operator=( const sc_uint_subref& v ) { sc_uint_base::operator=(v); return *this;}
 sc_uint& operator=( const sc_uint<LLSIZE>& v) { num = v.num; return *this; }
#if !defined(__SUNPRO_CC)
 template<class T1, class T2> sc_uint& operator=(const sc_uint_concat<T1,T2>& v) 
   { sc_uint_base::operator=(v); return *this; }
#endif
 sc_uint& operator=( const sc_bool_vector& v ) { sc_uint_base::operator=(v); return *this; }
 sc_uint& operator=( const sc_logic_vector& v ) { sc_uint_base::operator=(v); return *this; }
 sc_uint& operator=( const sc_signed& v ) { sc_uint_base::operator=(v); return *this; }
 sc_uint& operator=( const sc_unsigned& v ) { sc_uint_base::operator=(v); return *this; }

    // operator uint64() const { return num; }
 
 //Arithmetic operators
  sc_uint& operator+=( uint64 v) 
  {
    num += v;
    return *this;
  }

  sc_uint& operator-=(uint64 v) 
  {
    num -= v;
    return *this;
  }

  sc_uint& operator*=(uint64 v) 
  {
    num *= v;
    return *this;
  }

  sc_uint& operator/=(uint64 v) 
  {
    num /= v;
    return *this;
  }

  sc_uint& operator%=(uint64 v) 
  {
    num %= v;
    return *this;
  }
  
  //Bitwise operators
  sc_uint& operator&=(uint64 v) 
  {
    num &= v;
    return *this;
  }

  sc_uint& operator|=(uint64 v) 
  {
    num |= v;
    return *this;
  }

  sc_uint& operator^=(uint64 v) 
  {
    num ^= v;
    return *this;
  }

  sc_uint& operator<<=(uint64 v) 
  {
    num <<= v;
    return *this;
  }

  sc_uint& operator>>=(uint64 v) { num >>= v; return *this; }
  
  //postfix and prefix increment & decrement operators
  sc_uint& operator++() // prefix
  {
    ++num;
    return *this;
  }

  const sc_uint operator++(int) // postfix
  {
    uint64 tmp = num;
    num++;
    return sc_uint<LLSIZE>(tmp);
  }

  sc_uint& operator--() //prefix
  {
    --num;
    return *this;
  }
  
  const sc_uint operator--(int) //postfix 
  {
    uint64 tmp = num;
    num--;
    return sc_uint<LLSIZE>(tmp);
  }     

 // inherit bit-select and part-select operators from sc_uint_base
    
 // concatenation operators are global functions that are friends of sc_uint_base

}; 


// Class that implements bit-selection on sc_uint

class sc_uint_bitref {
  friend class sc_uint_base;
public:

  sc_uint_bitref& operator=( const sc_uint_bitref& b)
  {
    obj->set(index, (b.obj)->test(b.index) );
    return *this;
  }
   
  sc_uint_bitref& operator=( bit b )
  {
    obj->set(index, b );
    return *this;
  }
 
  
  sc_uint_bitref& operator&=( bit b )
  {
    obj->num &= ~ ( ( !b ) << index );
    return *this;
  }
  sc_uint_bitref& operator|=( bit b )
  {
#ifndef WIN32
    obj->num |= ( (b & 1ULL) << index );
#else
    obj->num |= ( (b & 1i64) << index );
#endif 
    return *this;
  }
  sc_uint_bitref& operator^=( bit b )
  {
#ifndef WIN32
    obj->num ^= ((b & 1ULL ) << index );
#else
    obj->num ^= ((b & 1i64 ) << index );
#endif
    return *this;
  }
  
  operator bit() const {return obj->test(index); }
  bit operator!() const {return ! obj->test(index); }

  int bitwidth() const { return 1; }
/*
  // gcc 2.95.2 bug - bool comparison mapped into int comparison
  // must provide explicit conversion, otherwise gcc is confused
  bool operator==(bool b)const {return (bool)(*this)==b;}
  // gcc 2.95.2 bug - all the other operators are affected by bool->int
  // conversion. Must provide implicit int conversion to avoid it
  operator int() const {return obj->test(index); }
*/
  private:
  sc_uint_base* obj;
  int index;

  sc_uint_bitref( sc_uint_base* u, int li ) { obj = u; index = li; }
};
// gcc 2.95.2 bug - bool comparison mapped into int comparison
// must provide explicit conversion, otherwise gcc is confused
//inline bool operator==(bool b,sc_uint_bitref& t){return t==b;}

// Class that implements part-selection on sc_uint

class sc_uint_subref {
  friend class sc_uint_base;
  
public:
  
  sc_uint_subref& operator=( const sc_uint_base& v)
  { 
    uint64 val = obj->num;
    // Mask the bits out that are part-selected

    val &= mask64[left][right];

    // set the part-selected bits
    val |= (v.num << right) & ~mask64[left][right]; 

    // Normalize the value 
    val &= MASK(obj->width);

    obj->num = val;
    
    return *this;
  }
    
  sc_uint_subref& operator=( const sc_uint_subref& v)
  {
    if( this == &v )
      return *this;
    return operator=(v.operator uint64() );
  }
  
  sc_uint_subref& operator=( uint64 v )
  {
   uint64 val = obj->num;
   
   // Mask the bits out that are part-selected
     val &= mask64[left][right];
   
    // set the part-selected bits
      val |= (v << right) & ~mask64[left][right]; 
    
   // Normalize the value
    val &= MASK(obj->width);

    obj->num = val;
    return *this;
  }
  
  operator uint64() const 
  { 
    uint64 val = obj->num;  
    if( left == obj->width - 1 )
      return val >> right;
    else
#ifndef WIN32
     return ( ( val & ( (1ULL << (left + 1)) - 1 ) ) >> right ); 
#else
     return ( ( val & ( (1i64 << (left + 1)) - 1 ) ) >> right );
#endif
  }
  
  int bitwidth() const { return (left - right + 1); }
  
private:
  sc_uint_base* obj;
  int left;
  int right;
  

  sc_uint_subref( sc_uint_base* u, int l, int r) { obj = u; left = l; right = r; }
};

// Proxy class to implement concatenation

template<class T1, class T2>
class sc_uint_concat {

public:

  sc_uint_concat( T1& l, T2& r ) : left(l), right(r) 
  {
    len = l.bitwidth() + r.bitwidth();
  }

  sc_uint_concat<T1,T2>( const sc_uint_concat<T1,T2>& v ) : left(v.left), right(v.right), len(v.len)
  {
      
  }

#if !defined(__SUNPRO_CC)
  template<class T3, class T4>
  sc_uint_concat<T1,T2>& operator=( const sc_uint_concat<T3,T4>& v ) // lvalue use
  {
      return operator=(v.operator uint64());
  }
#endif
  sc_uint_concat& operator=(uint64 v)
  {
#ifndef WIN32
    uint64 mask = (1ULL << right.bitwidth() )-1;
#else
    uint64 mask = (1i64 << right.bitwidth() )-1;
#endif
    right = v & mask;
    left = (v & ~mask) >> right.bitwidth();
    return *this;
  }
    

 sc_uint_concat& operator=( const sc_uint_base& v )
 {
   return operator=(v.operator uint64() );
 }

  sc_uint_concat& operator=( const sc_uint_subref& v)
  {
    return operator=( v.operator uint64() );
  }   

  operator uint64() const 
  {   
#ifndef WIN32
    uint64 mask = (1ULL << right.bitwidth() )-1;
#else
    uint64 mask = (1i64 << right.bitwidth() )-1;
#endif
    uint64 high = (left << right.bitwidth() ) & (~mask);
    uint64 low  = right & mask;
    uint64 tmp = high | low;
    return ( tmp << (LLWIDTH - len) >> (LLWIDTH - len) );
  }

  int bitwidth() const { return len; }

private:
  T1& left;
  T2& right;
  int len;


};

#include "sc_uint_defs.h"

#else 

#include "sc_uint32.h"

#endif // end of #ifndef _32BIT_ 

#if defined(WIN32) && !defined(__BCPLUSPLUS__)
extern ostream& operator<<(ostream& os, uint64);
#endif

#endif

