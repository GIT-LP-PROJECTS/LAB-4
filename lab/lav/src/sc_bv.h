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
  sc_bv.cpp (.h) -- packed bit vector class

  Original Author: Gene Bushuyev. Synopsys, Inc. (gbush@synopsys.com)
             No animals were harmed during the creation of this code.

******************************************************************************/



/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:

******************************************************************************/
#ifndef sc_bvH
#define sc_bvH


#if !defined(_MSC_VER)
#include <iostream>
using std::ostream;
using std::istream;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
#else
#include <iostream.h>
#endif

#if defined(WIN32)
#include <limits>
const unsigned byte_size = std::numeric_limits<unsigned char>::digits;
#else
const unsigned byte_size = 8;
#endif
#ifdef __BCPLUSPLUS__
#pragma hdrstop
#endif
//#include "sc_ptr.h"

#include "numeric_bit/sc_signed.h"
#include "numeric_bit/sc_unsigned.h"
#include "numeric_bit/sc_int.h"
#include "numeric_bit/sc_uint.h"
#include "numeric_bit/sc_bigint.h"
#include "numeric_bit/sc_biguint.h"
#include "sc_signal.h"
#include "sc_string.h"
#include "sc_bit.h"
#include "sc_exception.h"

class sc_bool_vector;
class sc_logic_vector;

#ifdef _MSC_VER
#define for if(0);else for
#endif

namespace sc_bv_ns
{
	using namespace std;

	// conversion tables
	extern const long char_to_logic[128];
	extern const char logic_to_char[4];
	extern const long and_table[4][4];
	extern const long or_table[4][4];
	extern const long xor_table[4][4];
	extern const long not_table[4];
	// constants for logical values
	enum Log_enum { Log_0 = 0, Log_1 = 1, Log_Z = 2, Log_X = 3 };
	// bit-length of the underlying type
	const unsigned ul_size = byte_size*sizeof(unsigned long);
	// utility function
	char* long_to_char(unsigned long l);
	// conversion from typed string
	sc_string convert_to_bin(const sc_string& s);
	sc_string convert_to_logic(const sc_string& s);

#ifndef _MSC_VER
	// due to incompatibilities in different compilers
	// use home-brewed min/max templates
	template<class T>
	inline const T& min(const T& t1, const T& t2)
	{
		return t1>t2 ? t2 : t1;
	}

	template<class T>
	inline const T& max(const T& t1, const T& t2)
	{
		return t1>t2 ? t1 : t2;
	}
#else
#endif
#if !defined(__SUNPRO_CC)
	// use to create binary constants
	template<unsigned x> unsigned binary()
	{
		return x ? 2 * binary<x / 10>() + x % 10 : 0;
	}
	// e.g. int i = binary<1010101>()
#endif
	// forward declaration of the classes
	template<class X> class t_base; // common base class
	class sc_bv_base;
	class sc_lv_base;
	template<int w> class sc_lv;
	template<int w> class sc_bv;
	template<int w> class sc_signal_rv;
	template<class X> class sc_range;
	template<class X, class Y> class sc_concat;
	template<class X, class Y>  class sc_bitwise_and;
	template<class X, class Y>  class sc_bitwise_or;
	template<class X, class Y>  class sc_bitwise_xor;
	template<class X> class sc_not;
	template<class X> class sc_shift_left;
	template<class X> class sc_shift_right;
	template<class X> class sc_bit_proxy;
	struct sc_bv_error;
	// non-member assignment function
	template<class X, class Y> void assign_(t_base<X>& x, const t_base<Y>& y);
	// non-member function verifies compatibility for assignment
	template<class X, class Y>
	inline void check_bounds(const t_base<X>&, const t_base<Y>&);

	//---------------------------------------------------------------------------
	// sc_bit_proxy
	//-------------------------------------------------------------------------
	template<class X>
	class sc_bit_proxy : public t_base<sc_bit_proxy<X> >
	{
		X& rep;
		unsigned index;
		// the following constructors/assignments are prohibited
		sc_bit_proxy(char);
		sc_bit_proxy(int);
	public:
		sc_bit_proxy(X& x, unsigned n) :rep(x), index(n){}
		sc_bit_proxy(const sc_bit_proxy& r) :rep(r.rep), index(r.index){}
		sc_bit_proxy& operator=(const sc_bit_proxy& r);
		sc_bit_proxy& operator=(long r);
		sc_bit_proxy& operator=(int r){ return operator=((long)r); }
		sc_bit_proxy& operator=(bool r){ return operator=((long)r); }
		sc_bit_proxy& operator=(const sc_bit& r){ return operator=((bool)r); }
		operator long() const;
		operator sc_logic()const { return sc_logic(long(*this)); }
		sc_bit_proxy& operator&=(bool r);
		sc_bit_proxy& operator|=(bool r);
		sc_bit_proxy& operator^=(bool r);
		bool operator &(bool r) const;
		bool operator |(bool r) const;
		bool operator ^(bool r) const;
		bool operator ~()const;
		sc_bit_proxy& operator &=(const sc_bit_proxy& s);
		sc_bit_proxy& operator |=(const sc_bit_proxy& s);
		sc_bit_proxy& operator ^=(const sc_bit_proxy& s);
		bool operator &(const sc_bit_proxy& s)const;
		bool operator |(const sc_bit_proxy& s)const;
		bool operator ^(const sc_bit_proxy& s)const;
		// common interface
		long get_bit(unsigned bit_number) const;
		void set_bit(unsigned bit_number, long value);
		unsigned long get_word(unsigned i) const;
		void set_word(unsigned i, unsigned long w);
		unsigned long get_cword(unsigned i) const;
		void set_cword(unsigned i, unsigned long w);
		int length()const;
	private:
		friend class sc_bv_base;
		friend class sc_lv_base;
	};

	//-------------------------------------------------------------------------
	// t_base class: this class is created for several purposes
	// 1) hiding operators from global namespace
	// that would be otherwise found by Koenig lookup
	// 2) avoiding repeating the same operations in every class including proxies
	// that could also be achieved by common base class, but this method allows
	// 3) improve performance by using non-virtual functions
	//-------------------------------------------------------------------------
	template<class X> class t_base
	{
	public:
		// comparisons
		bool operator== (const char*)const;
		bool operator== (const bool*)const;
		bool operator== (const sc_unsigned&)const;
		bool operator== (const sc_signed&)const;
		bool operator== (int)const;
		bool operator== (unsigned int)const;
		bool operator== (long)const;
		bool operator== (unsigned long)const;

		X& operator&= (const sc_unsigned&);
		X& operator|= (const sc_unsigned&);
		X& operator^= (const sc_unsigned&);

		X& operator&= (const sc_signed&);
		X& operator|= (const sc_signed&);
		X& operator^= (const sc_signed&);

		X& operator&= (unsigned int);
		X& operator|= (unsigned int);
		X& operator^= (unsigned int);

		X& operator&= (int);
		X& operator|= (int);
		X& operator^= (int);

		X& operator&= (unsigned long);
		X& operator|= (unsigned long);
		X& operator^= (unsigned long);

		X& operator&= (long);
		X& operator|= (long);
		X& operator^= (long);

		// reduce operators
		long and_reduce()  const;
		long nand_reduce() const { return not_table[and_reduce() & 3]; }
		long or_reduce()   const;
		long nor_reduce()  const { return not_table[or_reduce() & 3]; }
		long xor_reduce()  const;
		long xnor_reduce() const { return not_table[xor_reduce() & 3]; }
		// range selection
		sc_range<X> range(unsigned hi_bit, unsigned lo_bit);
		// the opposite of range for VSIA compatibility
		sc_range<X> operator()(unsigned hi_bit, unsigned lo_bit)
		{
			return range(lo_bit, hi_bit);
		}
		const sc_range<X> operator()(unsigned hi_bit, unsigned lo_bit) const
		{
			return sc_range<X>(cast_back(), lo_bit, hi_bit);
		}
		// bit selection
		sc_bit_proxy<X> operator [](unsigned n)
		{
			return sc_bit_proxy<X>(back_cast(), n);
		}
		long operator [](unsigned n) const
		{
			return back_cast().get_bit(n);
		}
		// negation
		const sc_not<X> operator~ ()const;
		// rvalue concatenation operators ???
		const sc_lv_base operator,(const char* s) const;
		sc_string to_string()const;
		// for compatibility purposes
		// no check is performed, may throw
		void to_string(char* buffer) const;
		void print(ostream& os) const { os << to_string(); }
		void dump(ostream& os) const
		{
			os << "sc_bv Length=" << back_cast().length() << "\n""" << to_string() << """\n";
		}

		// to_signed() returns the signed-integer interpretation of the
		// sc_bv; the MSB (left) is treated as the sign-bit.  If
		// the length of the sc_bv is bigger than 32 for whatever
		// value of 32 your C++ compiler has, then the result will be
		// truncated.  The sign bit will however be retained.
		int to_signed() const { return to_anything(); }
		unsigned to_unsigned() const { return to_anything(); }
		unsigned to_long() const { return to_anything(); }
		unsigned to_ulong() const { return to_anything(); }
		// common assignments
#if !defined(__SUNPRO_CC)
		template<class Y> X& assign_(const t_base<Y>& y)
		{
			sc_bv_ns::assign_(*this, y); return back_cast();
		}
#else
		// provide conversion to sc_lv_base temporary for Sun's SC5
		// warning: performance hit
		operator sc_lv_base() const;
#endif

		X& assign_(const char* b);
		X& assign_(const bool* b);

		X& assign_(const sc_unsigned& nv){ return assign_(nv.to_ulong()); }
		X& assign_(const sc_signed& nv){ return assign_(nv.to_ulong()); }
		X& assign_(long nv){ return assign_((unsigned long)nv); }
		X& assign_(unsigned long nv);
		X& assign_(int nv) { return assign_((unsigned long)nv); }
		X& assign_(unsigned nv){ return assign_((unsigned long)nv); }
		X& assign_(int64 v);
		X& assign_(uint64 v);
		X& assign_(const sc_bit& b){ return assign_((bool)b); }
		X& assign_(char c){ return assign_(char_to_logic[c]); }
		X& assign_(const sc_bool_vector&);

		X& assign_(const sc_logic_vector&);

	protected:
		void check_bounds(const sc_bool_vector& v);
		void check_bounds(const sc_logic_vector& v);
		void check_bounds(unsigned n); // check if bit n accessible
		void check_wbounds(unsigned n);// check if word n accessible
	public:
		// casts: these functions should be private
		// they are made public because Sun's 5.0 compiler doesn't
		// support member templates and thus they all were moved
		// outside of the class
		X& back_cast() { return static_cast<X&>(*this); }
		const X& back_cast()const { return static_cast<const X&>(*this); }
		const X& back_const_cast(){ return static_cast<const X&>(*this); }
	private:
		unsigned long to_anything() const;
		// additional functions required by VSIA
		X& operator <<= (int); // left shift with assignment
		X& operator >>= (int); // right shift with assignment

		const sc_string to_string(sc_numrep) const;
	};

	//--------------------------------------------------------------

	template<class X, class Y> void assign_(t_base<X>& x, const t_base<Y>& y);

	//--------------------------------------------------------------
	class sc_lv_base : public t_base<sc_lv_base>
	{
	protected:
		unsigned long* data; // data array
		unsigned long* control;
		unsigned N; // array length
		typedef sc_logic elemtype;

		sc_lv_base(char c);
		void init(unsigned long);
	public:
		// ctor/dtor
		sc_lv_base() { init(1); }
		virtual ~sc_lv_base() { delete[] data; delete[] control; }
		explicit sc_lv_base(unsigned long length) { init(length); }
		explicit sc_lv_base(long length) { init(length); }
		explicit sc_lv_base(unsigned length) { init(length); }
		explicit sc_lv_base(int length) { init(length); }
		sc_lv_base(unsigned length, long init_value);
		sc_lv_base(const sc_lv_base& y) { init(y.length()); sc_bv_ns::assign_(*this, y); }
		sc_lv_base(const char* y) { init(strlen(y)); assign_(y); }
#if !defined(__SUNPRO_CC)
		//sc_lv_base(const sc_bv_base& t){convert(t);}
#else
		sc_lv_base(const sc_bv_base& t){ sc_bv_ns::assign_(*this, t); }
#endif
		sc_lv_base(const sc_bool_vector& y) { init(y.length()); assign_(y); }
		sc_lv_base(const sc_logic_vector& y) { init(y.length()); assign_(y); }
#if !defined(__SUNPRO_CC)
		template<class Y> sc_lv_base(const t_base<Y>& y)
		{
			init(y.back_cast().length()); assign_(y);
		}
		template<class Y> sc_lv_base& operator=(const t_base<Y>& y)
		{
			sc_bv_ns::assign_(*this, y); return *this;
		}
#endif
		sc_lv_base& operator=(const sc_lv_base& nv) { sc_bv_ns::assign_(*this, nv); return *this; }
		sc_lv_base& operator=(const char* nv) { assign_(nv); return *this; }
		sc_lv_base& operator=(const bool* nv) { assign_(nv); return *this; }

		sc_lv_base& operator=(const sc_unsigned& nv) { assign_(nv); return *this; }
		sc_lv_base& operator=(const sc_signed& nv) { assign_(nv); return *this; }
		sc_lv_base& operator=(long nv) { assign_(nv); return *this; }
		sc_lv_base& operator=(unsigned long nv) { assign_(nv); return *this; }
		sc_lv_base& operator=(int nv) { assign_(nv); return *this; }
		sc_lv_base& operator=(unsigned nv) { assign_(nv); return *this; }
		sc_lv_base& operator=(int64 nv) { assign_(nv); return *this; }
		sc_lv_base& operator=(uint64 nv) { assign_(nv); return *this; }
		sc_lv_base& operator=(char nv) { assign_(nv); return *this; }
		sc_lv_base& operator=(const sc_bit& nv) { assign_(nv); return *this; }
		sc_lv_base& operator=(const sc_bool_vector& nv) { assign_(nv); return *this; }
		sc_lv_base& operator=(const sc_logic_vector& nv) { assign_(nv); return *this; }

		// common interface
		long get_bit(unsigned bit_number) const;
		void set_bit(unsigned bit_number, long value);
		unsigned long get_word(unsigned i) const { return data[i]; }
		void set_word(unsigned i, unsigned long w) { data[i] = w; }
		unsigned long get_cword(unsigned i) const { return control[i]; }
		void set_cword(unsigned i, unsigned long w) { control[i] = w; }
		// capacity functions
		int length()const { return N; }

		// member functions
		void set(unsigned long v); // fill with v
		virtual void resize(unsigned long new_size);

	public: // make private later
		void get_packed_rep(unsigned long* ptr) const
		{
			if (data && N) memmove(ptr, data, ((N - 1) / ul_size + 1)*sizeof(unsigned long));
		}
		void set_packed_rep(const unsigned long* new_buffer);
		void get_control_rep(unsigned long* ptr) const
		{
			if (data && N) memmove(ptr, control, ((N - 1) / ul_size + 1)*sizeof(unsigned long));
		}
		void set_control_rep(const unsigned long* new_buffer);

		void clean_tail();
	};
	//--------------------------------------------------------------

//-------------------------------------------------------------------------
// main t_base operators and functions (don't have to be friends)
//-------------------------------------------------------------------------
template<class X,class Y> inline
bool operator== (const t_base<X>& x,const t_base<Y>& y);
#if !defined(__SUNPRO_CC)
template<class X,int W> inline
bool operator== (const t_base<X>&,const sc_int<W>&);
template<class X,int W> inline
bool operator== (const t_base<X>&,const sc_uint<W>&);
template<class X,int W> inline
bool operator== (const sc_int<W>& i,const t_base<X>& v) {return v==i;}
template<class X,int W> inline
bool operator== (const sc_uint<W>& i,const t_base<X>& v){return v==i;}
#endif
template<class X> inline
bool operator== (const char* x,const t_base<X>& b)  {return b==x;}
template<class X> inline
bool operator== (const bool* x,const t_base<X>& b)  {return b==x;}
template<class X> inline
bool operator== (const sc_unsigned& x,const t_base<X>& b) {return b==x;}
template<class X> inline
bool operator== (const sc_signed& x,const t_base<X>& b) {return b==x;}
template<class X> inline
bool operator== (int x,const t_base<X>& b) {return b==x;}
template<class X> inline
bool operator== (unsigned int x,const t_base<X>& b) {return b==x;}
template<class X> inline
bool operator== (long x,const t_base<X>& b) {return b==x;}
template<class X> inline
bool operator== (unsigned long x,const t_base<X>& b) {return b==x;}

template<class X,class Y> inline
bool operator!= (const t_base<X>& x,const t_base<Y>& y) {return !(x==y);}
#if !defined(__SUNPRO_CC)
template<class X,int W> inline
bool operator!= (const t_base<X>& x,const sc_int<W>& y)
{ return !(x==y);}
template<class X,int W> inline
bool operator!= (const t_base<X>& x,const sc_uint<W>& y)
{ return !(x==y);}
template<class X,int W> inline
bool operator!= (const sc_int<W>& x,const t_base<X>& y)
{ return !(x==y);}
template<class X,int W> inline
bool operator!= (const sc_uint<W>& x,const t_base<X>& y)
{ return !(x==y);}
#endif
template<class X> inline
bool operator!= (const t_base<X>& b, const char* x)  {return !(b==x);}
template<class X> inline
bool operator!= (const char* x,const t_base<X>& b)  {return !(b==x);}
template<class X> inline
bool operator!= (const t_base<X>& b, const bool* x)  {return !(b==x);}
template<class X> inline
bool operator!= (const bool* x,const t_base<X>& b)  {return !(b==x);}
template<class X> inline
bool operator!= (const t_base<X>& b, const sc_unsigned& x) {return !(b==x);}
template<class X> inline
bool operator!= (const sc_unsigned& x,const t_base<X>& b) {return !(b==x);}
template<class X> inline
bool operator!= (const t_base<X>& b, const sc_signed& x) {return !(b==x);}
template<class X> inline
bool operator!= (const sc_signed& x,const t_base<X>& b) {return !(b==x);}
template<class X> inline
bool operator!= (const t_base<X>& b, int x) {return !(b==x);}
template<class X> inline
bool operator!= (int x,const t_base<X>& b) {return !(b==x);}
template<class X> inline
bool operator!= (const t_base<X>& b, unsigned int x) {return !(b==x);}
template<class X> inline
bool operator!= (unsigned int x,const t_base<X>& b) {return !(b==x);}
template<class X> inline
bool operator!= (const t_base<X>& b, long x) {return !(b==x);}
template<class X> inline
bool operator!= (long x,const t_base<X>& b) {return !(b==x);}
template<class X> inline
bool operator!= (const t_base<X>& b, unsigned long x) {return !(b==x);}
template<class X> inline
bool operator!= (unsigned long x,const t_base<X>& b) {return !(b==x);}

// bitwise assignments returning proxy
// t_base operators
template<class X,class Y> inline
X& operator&= (t_base<X>& x,const t_base<Y>& y);
template<class X,class Y> inline
X& operator|= (t_base<X>& x,const t_base<Y>& y);
template<class X,class Y> inline
X& operator^= (t_base<X>& x,const t_base<Y>& y);
// sc_int operators
#if !defined(__SUNPRO_CC)
template<class X,int W>  inline
X& operator&= (t_base<X>&,const sc_int<W>&);
template<class X,int W>  inline
X& operator|= (t_base<X>&,const sc_int<W>&);
template<class X,int W>  inline
X& operator^= (t_base<X>&,const sc_int<W>&);
// sc_uint operators
template<class X,int W>  inline
X& operator&= (t_base<X>&,const sc_uint<W>&);
template<class X,int W> inline
X& operator|= (t_base<X>&,const sc_uint<W>&);
template<class X,int W> inline
X& operator^= (t_base<X>&,const sc_uint<W>&);
#endif
// bitwise non-assignment returning proxy
template<class X,class Y> inline
const sc_bitwise_and<X,Y> operator& (const t_base<X>& x,const t_base<Y>& y);
template<class X,class Y> inline
const sc_bitwise_or<X,Y> operator| (const t_base<X>& x,const t_base<Y>& y);
template<class X,class Y> inline
const sc_bitwise_xor<X,Y> operator^ (const t_base<X>& x,const t_base<Y>& y);

// l-value concatenation
template<class X,class Y> inline
sc_concat<X,Y> operator,(t_base<X>& x,t_base<Y>& y);
// l-value shift operators
template<class X> inline
const sc_shift_left<X>  operator<<(const t_base<X>& x,unsigned n);
template<class X>  inline
const sc_shift_right<X> operator>>(const t_base<X>& x,unsigned n);

// rotations (cannot be a proxy type)
template<class X>
const X lrotate(const t_base<X>& x, int n)
{
  if(n<0)
    throw sc_edata(1000);
  n %= x.back_cast().length(); // no need to rotate more than a full cirle
  if(n)
    return x.back_cast()<<n | x.back_cast()>>(x.back_cast().length()-n);
  else
    return x;
}
template<class X>
const X rrotate(const t_base<X>& x, int n)
{
  if(n<0)
    throw sc_edata(1000);
  n %= x.back_cast().length(); // no need to rotate more than a full cirle
  if(n)
    return x.back_cast()>>n | x.back_cast()<<(x.back_cast().length()-n);
  else
    return x;
}

// reverse function
template<class X>
const sc_range<X> reverse(const t_base<X>& x)
{
  return sc_range<X>(x,0,x.back_cast().length()-1);
}

// shift array of type T: T t<<n
template<class T>
unsigned long array_get_word(const T& p, unsigned length, unsigned index);
template<class T>
void array_set_word(T& p, const T t, unsigned length, unsigned index);



//-------------------------------------------------------------------------
// proxies for bool operands
//-------------------------------------------------------------------------
template<class X>
sc_bit_proxy<X> operator &(bool r, const sc_bit_proxy<X>& s){return s&r;}
template<class X>
sc_bit_proxy<X> operator |(bool r, const sc_bit_proxy<X>& s){return s|r;}
template<class X>
sc_bit_proxy<X> operator ^(bool r, const sc_bit_proxy<X>& s){return s^r;}


//-------------------------------------------------------------------------
// base class - packed bit representation
//-------------------------------------------------------------------------
class sc_bv_base: public t_base<sc_bv_base>
{
  typedef t_base<sc_bv_base> inherited;
  protected:
  unsigned long* data; // data array
  unsigned N; // array length
  friend class sc_lv_base;

  explicit sc_bv_base(char);
  void init(unsigned long);
  //sc_bv_base():data(NULL),N(0){}
  public:
  // ctor/dtor
#if !defined(__SUNPRO_CC)
  template<class T> sc_bv_base(const t_base<T>& y)
	{init(y.back_cast().length());assign_(y);}
#else
  // provide at least construction from sc_lv_base
  sc_bv_base(const sc_lv_base& v);
#endif

  sc_bv_base(){init(1);} // VSIA requirements, don't ever use it; to be amended
  sc_bv_base(const sc_bv_base&);
  virtual ~sc_bv_base(){if(N) delete[] data;}
  explicit sc_bv_base(unsigned long length){init(length);}
  explicit sc_bv_base(long length){init(length);}
  explicit sc_bv_base(unsigned length){init(length);}
  explicit sc_bv_base(int length){init(length);}
  sc_bv_base(unsigned length, bool init_value);
  sc_bv_base(const sc_bool_vector& v);
  sc_bv_base(const char*);

  // Assignments may be made to a sc_bv from various types of
  // objects. Only common bits are changed.
#if !defined(__SUNPRO_CC)
  template<class T> sc_bv_base& operator=(const t_base<T>& t){return assign_(t);}
  sc_bv_base& operator=(const sc_bv_base& t){return assign_(t);}
#else
  sc_bv_base& operator=(const sc_bv_base& t)
    { sc_bv_ns::assign_((*this),t); return *this;}
  sc_bv_base& operator=(const sc_lv_base& t);
#endif
  sc_bv_base& operator=( const char* t){return assign_(t);}
  sc_bv_base& operator=( const bool* t){return assign_(t);}
  sc_bv_base& operator=( const sc_unsigned& t ){return assign_(t);}
  sc_bv_base& operator=( const sc_signed& t ){return assign_(t);}
  sc_bv_base& operator=( long t ){return assign_(t);}
  sc_bv_base& operator=( unsigned long t ) {return assign_(t);}
  sc_bv_base& operator=( int t ) {return assign_(t);}
  sc_bv_base& operator=( unsigned t ){return assign_(t);}
  sc_bv_base& operator=( int64 t ){return assign_(t);}
  sc_bv_base& operator=( uint64 t ){return assign_(t);}
  sc_bv_base& operator=( char t ){return assign_(t);}
  sc_bv_base& operator=( const sc_bit& t ){return assign_(t);}
  sc_bv_base& operator=(const sc_bool_vector& t){return assign_(t);}

  // common functions thru which t_base implements all operators
  inline long get_bit(unsigned n) const;
  inline void set_bit(unsigned bit_number, long value);
  unsigned long get_word(unsigned i) const {return data[i];}
  void set_word(unsigned i, unsigned long w) {data[i]=w;}
  unsigned long get_cword(unsigned i) const {return 0;}
  void set_cword(unsigned i, unsigned long w);
  int length()const { return N;}
  // operators specific to sc_bv_base
#if !defined(__GNUC__)
  // gcc 2.95.2 bug makes it impossible to use operators with char*
  using t_base<sc_bv_base>::operator &=;
  using t_base<sc_bv_base>::operator |=;
  using t_base<sc_bv_base>::operator ^=;
  sc_bv_base& operator &= (const char* s){return (*this)&=sc_bv_base(s);}
  sc_bv_base& operator |= (const char* s){return (*this)|=sc_bv_base(s);}
  sc_bv_base& operator ^= (const char* s){return (*this)^=sc_bv_base(s);}
  sc_bv_base operator & (const char* s)const
    { sc_bv_base b(*this); return b&=s;}
  sc_bv_base operator | (const char* s)const
    { sc_bv_base b(*this); return b|=s;}
  sc_bv_base operator ^ (const char* s)const
    { sc_bv_base b(*this); return b^=s;}
  friend sc_bv_base operator &(const char* s, const sc_bv_base& b){ return b&s;}
  friend sc_bv_base operator |(const char* s, const sc_bv_base& b){ return b|s;}
  friend sc_bv_base operator ^(const char* s, const sc_bv_base& b){ return b^s;}
#endif
  // member functions
  void set(unsigned long v=0); // fill with v
  virtual void resize(unsigned long new_size);

  public: // make it private later
  // copy the internal buffer into ptr
  void get_packed_rep(unsigned long* ptr) const
    { if(data && N) memmove(ptr,data,((N-1)/ul_size+1)*sizeof(unsigned long) );}
  void set_packed_rep(const unsigned long* new_buffer);
  void clean_tail();
  // additional functions required by VSIA
  sc_bv_base (const char*, int);  // With length argument.
};
//-------------------------------------------------------------------------
// sc_bv<> bit-vector class
//---------------------------------------------------------------------------
template<int W>
class sc_bv : public sc_bv_base
{
  public:
  // ctor/dtor
  sc_bv():sc_bv_base(W){} // uninitialized
  explicit sc_bv(bool init_value):sc_bv_base(W,init_value){}
#if !defined(__SUNPRO_CC)
  template<class X> sc_bv(const t_base<X>& t):sc_bv_base(t)
  {
    if(W!=t.back_cast().length())
     throw sc_edata(1002,"different lengths");
  }
#endif
  sc_bv(const sc_bv& y ):sc_bv_base(y){}
  sc_bv(const char*);
  sc_bv(char c):sc_bv_base(W,sc_bv_base::char_to_logic[c]){}
  sc_bv(int v):sc_bv_base(W){(*this)=v;}
  sc_bv(const sc_bool_vector& v):sc_bv_base(v){}
  sc_bv(const sc_logic_vector& v);
  virtual sc_bv_base* clone(){return new sc_bv<W>(*this);}
  // Assignments may be made to a sc_bv from various types of
  // objects. Only common bits are changed.
  // this template assignment is for proxy classes and sc_lv
#if !defined(__SUNPRO_CC)
  template<class T> sc_bv& operator=(const t_base<T>& t)
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
#endif
  sc_bv& operator=(const sc_bv& t)
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( const char* t)
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( const bool* t)
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( const sc_unsigned& t )
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( const sc_signed& t )
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( long t )
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( unsigned long t )
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( int t )
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( unsigned t )
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( char t )
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( const sc_bit& t )
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=(int64 t)
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=(uint64 t)
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( const sc_int<W>& t )
   { return operator=((int64)t);}
  sc_bv& operator=( const sc_uint<W>& t )
   { return operator=((uint64)t);}
  sc_bv& operator=(const sc_bool_vector& v)
   { return static_cast<sc_bv&>(sc_bv_base::operator=(v));}
  sc_bv& operator=(const sc_logic_vector& v);

  virtual void resize(unsigned long new_size)
   { throw sc_edata(2006," - sc_bv"); }
};

//---------------------------------------------------------------------------
//  shift proxies (r-value only)
//---------------------------------------------------------------------------
template<class X>
class sc_shift_left : public t_base<sc_shift_left<X> >
{
  const X& b;
  int n;
  public:
  sc_shift_left(const X& b, int n): b(b),n(n){};
  sc_shift_left(const sc_shift_left& bn):b(bn.b),n(bn.n){};
  // bit selection
  long get_bit(unsigned i) const;
  // member functions
  int length()const {return b.length();}
  unsigned long get_word(unsigned i)const;
  unsigned long get_cword(unsigned i) const;
};

template<class X>
class sc_shift_right : public t_base<sc_shift_right<X> >
{
  const X& b;
  int n;
  public:
  sc_shift_right(const X& b, int n): b(b),n(n){};
  sc_shift_right(const sc_shift_right& bn):b(bn.b),n(bn.n){};
  // bit selection
  long get_bit(unsigned i) const;
  // member functions
  int length()const {return b.length();}
  unsigned long get_word(unsigned i)const;
  unsigned long get_cword(unsigned i) const;
};
//---------------------------------------------------------------------------
//  range proxies (l-value)
//---------------------------------------------------------------------------
template<class X>
class sc_range : public t_base<sc_range<X> >
{
  X& bv;
  unsigned hi_bit,lo_bit;
  public:
  sc_range(X& bv, unsigned hi_bit, unsigned lo_bit):
    bv(bv),hi_bit(hi_bit),lo_bit(lo_bit)
    { if((int)hi_bit>=bv.length()||(int)lo_bit>=bv.length()) throw sc_edata(1000); }
  sc_range(const sc_range& bn): bv(bn.bv),hi_bit(bn.hi_bit),lo_bit(bn.lo_bit){}
  virtual sc_range* clone(){return new sc_range(*this);}
  // assignment operators
#if !defined(__SUNPRO_CC)
  template<class T>
  sc_range& operator = (const t_base<T>& b){assign_(b); return *this;}
#endif
  sc_range& operator = (const sc_range& b);
  sc_range& operator = (const char* b);
  sc_range& operator = (const bool* b);
  long get_bit(unsigned i) const;
  inline void set_bit(unsigned n, long value);
  // member functions
  int length()const
    { return (hi_bit>lo_bit?int(hi_bit - lo_bit):int(lo_bit - hi_bit)) +1;}
  bool reversed()const { return lo_bit>hi_bit; }
  unsigned long get_word(unsigned i)const;
  void set_word(unsigned i, unsigned long w);
  unsigned long get_cword(unsigned i) const;
  void set_cword(unsigned i, unsigned long w);
  void clean_tail(){bv.clean_tail();}
};

//---------------------------------------------------------------------------
// bitwise proxies
//---------------------------------------------------------------------------
// sc_bitwise_and
//---------------------------------------------------------------------------
template<class X, class Y>
class sc_bitwise_and : public t_base<sc_bitwise_and<X,Y> >
{
  const X& b1;
  const Y& b2;
  public:
  sc_bitwise_and(const X& b1, const Y& b2):b1(b1),b2(b2)
    {if(b1.length()!=b2.length()) throw sc_edata(1002);}
  sc_bitwise_and(const sc_bitwise_and& b):b1(b.b1),b2(b.b2){}
  // no assignments available
  // bit selection
  long get_bit(unsigned i) const;
  // member functions
  int length()const { return b1.length();}
  // get the whole word of the underlying type for fast operations
  // no range check
  unsigned long get_word(unsigned i)const
    {return b1.get_word(i)&b2.get_word(i) | get_cword(i);}
  unsigned long get_cword(unsigned i) const;
};
//---------------------------------------------------------------------------
// sc_bitwise_or
//---------------------------------------------------------------------------
template<class X, class Y>
class sc_bitwise_or : public t_base<sc_bitwise_or<X,Y> >
{
  const X& b1;
  const Y& b2;
  public:
  sc_bitwise_or(const X& b1, const Y& b2):b1(b1),b2(b2)
    {if(b1.length()!=b2.length()) throw sc_edata(1002);}
  sc_bitwise_or(const sc_bitwise_or& b):b1(b.b1),b2(b.b2){}
  // no assignments available
  // bit selection
  long get_bit(unsigned n) const {return b1.get_bit(n)||b2.get_bit(n);}
  // member functions
  int length()const { return b1.length();}
  // get the whole word of the underlying type for fast operations
  // no range check
  unsigned long get_word(unsigned i)const;
  unsigned long get_cword(unsigned i) const;
};
//---------------------------------------------------------------------------
// sc_bitwise_xor
//---------------------------------------------------------------------------
template<class X, class Y>
class sc_bitwise_xor : public t_base<sc_bitwise_xor<X,Y> >
{
  const X& b1;
  const Y& b2;
  public:
  sc_bitwise_xor(const X& b1, const Y& b2):b1(b1),b2(b2)
    {if(b1.length()!=b2.length()) throw sc_edata(1002);}
  sc_bitwise_xor(const sc_bitwise_xor& b):b1(b.b1),b2(b.b2){}
  // no assignments available
  // bit selection
  long get_bit(unsigned n) const {return b1.get_bit(n)^b2.get_bit(n);}
  // member functions
  int length()const { return b1.length();}
  // get the whole word of the underlying type for fast operations
  // no range check
  unsigned long get_word(unsigned i)const;
  unsigned long get_cword(unsigned i) const;
};
//---------------------------------------------------------------------------
// sc_not
//---------------------------------------------------------------------------
template<class X>
class sc_not : public t_base<sc_not<X> >
{
  const X& b;
  public:
  sc_not(const X& b):b(b){};
  sc_not(const sc_not<X>& bn):b(bn.b){};
  // no assignments available
  long get_bit(unsigned n) const;
  // member functions
  int length()const{return b.length();};
  // get the whole word of the underlying type for fast operations
  // no range check
  unsigned long get_word(unsigned i)const {return ~b.get_word(i) | get_cword(i);}
  unsigned long get_cword(unsigned i) const  {return b.get_cword(i); }
};
//---------------------------------------------------------------------------
// sc_concat
//---------------------------------------------------------------------------
template<class X, class Y>
class sc_concat : public t_base<sc_concat<X,Y> >
{
  X& bv1; //hi
  Y& bv2; //lo
  public:
  sc_concat(X&, const char*);
  // the ordering of words is  (bv1[N1-1]...bv1[0] bv2[N2-1]...bv2[0])
  sc_concat(X& bv1, Y& bv2):bv1(bv1),bv2(bv2){};
  sc_concat(const sc_concat& bn):bv1(bn.bv1),bv2(bn.bv2){};
  virtual sc_concat* clone(){return new sc_concat(*this);}
  // assignment operators
#if !defined(__SUNPRO_CC)
  template<class T>
  sc_concat& operator = (const t_base<T>& b){return assign_(b);}
#endif
  sc_concat& operator = (const sc_concat& b){return assign_(b);}
  sc_concat& operator = (const char* b){return assign_(b);}
  sc_concat& operator = (const bool* b){return assign_(b);}
  // bit selection
  long get_bit(unsigned n) const;
  void set_bit(unsigned bit_number, long value);
  int length()const { return bv1.length()+bv2.length();}
  unsigned long get_word(unsigned i)const;
  void set_word(unsigned i, unsigned long w);
  unsigned long get_cword(unsigned i) const;
  void set_cword(unsigned i, unsigned long w);
  void clean_tail(){bv1.clean_tail();bv2.clean_tail();}
};
// ------- end of interface ---------------------------------------------------

// ----------------------------------------------------------------------------
// implementation
// ----------------------------------------------------------------------------

// general templates (t_base template)
#if defined(__SUNPRO_CC)
// warning: performance hit
template<class X>
t_base<X>::operator sc_lv_base() const
{
  sc_lv_base v(back_cast().length());
  sc_bv_ns::assign_(v,back_cast());
  return v;
}
#endif

template<class X,class Y>
bool operator== (const t_base<X>& x,const t_base<Y>& y)
{
  bool equal = x.back_cast().length()==y.back_cast().length();
  unsigned len = (x.back_cast().length()-1)/ul_size+1;
  unsigned i=0;
  while(equal && i<len)
  {
    equal = x.back_cast().get_word(i)==y.back_cast().get_word(i) &&
            x.back_cast().get_cword(i)==y.back_cast().get_cword(i);
    i++;
  }
  return equal;
}
template<class X> bool t_base<X>::operator== (const char* y)const
{
  bool equal = back_cast().length()==(int)strlen(y);
  unsigned len = (back_cast().length()-1)/ul_size+1;
  unsigned i=0;
  while(equal && i<len)
  {
    equal = back_cast()[i]==char_to_logic[y[len-i-1]];
    i++;
  }
  return equal;
}
template<class X> bool t_base<X>::operator== (const bool* y)const
{
  bool equal = true;
  unsigned len = (back_cast().length()-1)/ul_size+1;
  unsigned i=0;
  while(equal && i<len)
  {
    equal = static_cast<bool>( back_cast()[i] ) == y[i];
    i++;
  }
  return equal;
}
template<class X> bool t_base<X>::operator== (const sc_unsigned& y)const
{
  bool equal = static_cast<int>( back_cast().length() ) == y.length();
  int i = 0;
  while(equal && i<back_cast().length())
  {
    equal = static_cast<bool>( back_cast()[i] ) == y[i];
    i++;
  }
  return equal;
}
template<class X> bool t_base<X>::operator== (const sc_signed& y)const
{
  bool equal = static_cast<int>( back_cast().length() ) == y.length();
  int i = 0;
  while(equal && i<back_cast().length())
  {
    equal = static_cast<bool>( back_cast()[i] ) == y[i];
    i++;
  }
  return equal;
}

#if !defined(__SUNPRO_CC)
template<class X,int W>
bool operator== (const t_base<X> x, const sc_int<W>& y)
{
  bool equal = x.back_cast().length()==W;
  if(equal)
    equal = ((int64)y & ~0ul) == x.back_cast().get_word(0) &&
      x.back_cast().get_cword(0)==0;
    if(equal && W>ul_size)
      equal = ((int64)y>>ul_size & ~0ul) == x.back_cast().get_word(1) &&
        x.back_cast().get_cword(1)==0;
  return equal;
}

template<class X,int W>
bool operator== (const t_base<X> x, const sc_uint<W>& y)
{
  bool equal = x.back_cast().length()==W;
  if(equal)
    equal = ((uint64)y & ~0ul) == x.back_cast().get_word(0) &&
      x.back_cast().get_cword(0)==0;
    if(equal && W>ul_size)
      equal = ((uint64)y>>ul_size & ~0ul) == x.back_cast().get_word(1) &&
        x.back_cast().get_cword(1)==0;
  return equal;
}
#endif

template<class X> bool t_base<X>::operator== (int y)const
{
  return back_cast().length()<=(int)ul_size &&
         back_cast().get_word(0)==(unsigned long)y &&
         back_cast().get_cword(0)==0;
}
template<class X> bool t_base<X>::operator== (unsigned int y)const
{
  return back_cast().length()<=(int)ul_size &&
         back_cast().get_word(0)==(unsigned long)y &&
         back_cast().get_cword(0)==0;
}
template<class X> bool t_base<X>::operator== (long y)const
{
  return back_cast().length()<=(int)ul_size &&
         back_cast().get_word(0)==(unsigned long)y &&
         back_cast().get_cword(0)==0;
}
template<class X> bool t_base<X>::operator== (unsigned long y)const
{
  return back_cast().length()<=(int)ul_size &&
         back_cast().get_word(0)==y &&
         back_cast().get_cword(0)==0;
}

template<class X,class Y> X& operator&= (t_base<X>& x,const t_base<Y>& y)
{
  unsigned len = (x.back_cast().length()-1)/ul_size+1;
  if(x.back_cast().length()!=y.back_cast().length())
    throw sc_edata(1002);
  for(unsigned i=0; i<len; i++)
  {
    unsigned long control = x.back_cast().get_cword(i)&y.back_cast().get_word(i) |
      x.back_cast().get_cword(i) & y.back_cast().get_cword(i) | x.back_cast().get_word(i)&
      y.back_cast().get_cword(i);
    x.back_cast().set_cword(i,control);
    x.back_cast().set_word(i, x.back_cast().get_word(i) & y.back_cast().get_word(i) |
      control);
  }
  return x.back_cast();
}
template<class X,class Y> X& operator|= (t_base<X>& x,const t_base<Y>& y)
{
  unsigned len = (x.back_cast().length()-1)/ul_size+1;
  if(x.back_cast().length()!=y.back_cast().length())
    throw sc_edata(1002);
  for(unsigned i=0; i<len; i++)
  {
    unsigned long control = x.back_cast().get_cword(i)&~y.back_cast().get_word(i) |
      x.back_cast().get_cword(i)&y.back_cast().get_cword(i) |
      ~x.back_cast().get_word(i)&y.back_cast().get_cword(i);
    x.back_cast().set_cword(i,control);
    x.back_cast().set_word(i, x.back_cast().get_word(i) | y.back_cast().get_word(i) |
       control);
  }
  return x.back_cast();
}
template<class X,class Y> X& operator^= (t_base<X>& x,const t_base<Y>& y)
{
  unsigned len = (x.back_cast().length()-1)/ul_size+1;
  if(x.back_cast().length()!=y.back_cast().length())
    throw sc_edata(1002);
  for(unsigned i=0; i<len; i++)
  {
    unsigned long control = x.back_cast().get_cword(i)|y.back_cast().get_cword(i);
    x.back_cast().set_cword(i,control);
    x.back_cast().set_word(i, x.back_cast().get_word(i) ^ y.back_cast().get_word(i) |
      control);
  }
  return x.back_cast();
}

template<class X> X& t_base<X>::operator&= (const sc_unsigned& y)
{
  const unsigned len = sc_bv_ns::min(back_cast().length(),y.length());
  for(unsigned i=0; i<len; i++)
  {
    back_cast()[i]=and_table[back_cast()[i]][y[i]];
  }
  return back_cast();
}
template<class X> X& t_base<X>::operator|= (const sc_unsigned& y)
{
  const unsigned len = sc_bv_ns::min(back_cast().length(),y.length());
  for(unsigned i=0; i<len; i++)
  {
    back_cast()[i]=or_table[back_cast()[i]][y[i]];
  }
  return back_cast();
}
template<class X> X& t_base<X>::operator^= (const sc_unsigned& y)
{
  const unsigned len = sc_bv_ns::min(back_cast().length(),y.length());
  for(unsigned i=0; i<len; i++)
  {
    back_cast()[i]=xor_table[back_cast()[i]][y[i]];
  }
  return back_cast();
}

template<class X> X& t_base<X>::operator&= (const sc_signed& y)
{
  const unsigned len = sc_bv_ns::min(back_cast().length(),(unsigned)(y.length()));
  for(unsigned i=0; i<len; i++)
  {
    back_cast()[i]=and_table[back_cast()[i]][y[i]];
  }
  return back_cast();
}
template<class X> X& t_base<X>::operator|= (const sc_signed& y)
{
  const unsigned len = sc_bv_ns::min(back_cast().length(),(unsigned)(y.length()));
  for(unsigned i=0; i<len; i++)
  {
    back_cast()[i]=or_table[back_cast()[i]][y[i]];
  }
  return back_cast();
}
template<class X> X& t_base<X>::operator^= (const sc_signed& y)
{
  const unsigned len = sc_bv_ns::min(back_cast().length(),(unsigned)(y.length()));
  for(unsigned i=0; i<len; i++)
  {
    back_cast()[i]=xor_table[back_cast()[i]][y[i]];
  }
  return back_cast();
}

#if !defined(__SUNPRO_CC)
template<class X,int W>
X& operator&= (t_base<X>& x,const sc_int<W>& y)
{
  const unsigned len = sc_bv_ns::min(x.back_cast().length(),(unsigned)W);
  for(unsigned i=0; i<len; i++)
    x.back_cast()[i]=and_table[x.back_cast()[i]][y[i]];
  return x.back_cast();
}
template<class X,int W>
X& operator|= (t_base<X>& x,const sc_int<W>& y)
{
  const unsigned len = sc_bv_ns::min(x.back_cast().length(),(unsigned)W);
  for(unsigned i=0; i<len; i++)
    x.back_cast()[i]=or_table[x.back_cast()[i]][y[i]];
  return x.back_cast();
}
template<class X,int W>
X& operator^= (t_base<X>& x,const sc_int<W>& y)
{
  const unsigned len = sc_bv_ns::min(x.back_cast().length(),(unsigned)W);
  for(unsigned i=0; i<len; i++)
    x.back_cast()[i]=xor_table[x.back_cast()[i]][y[i]];
  return x.back_cast();
}

template<class X,int W>
X& operator&= (t_base<X>& x,const sc_uint<W>& y)
{
  const unsigned len = sc_bv_ns::min(x.back_cast().length(),(unsigned)W);
  for(unsigned i=0; i<len; i++)
    x.back_cast()[i]=and_table[x.back_cast()[i]][y[i]];
  return x.back_cast();
}
template<class X,int W>
X& operator|= (t_base<X>& x,const sc_uint<W>& y)
{
  const unsigned len = sc_bv_ns::min(x.back_cast().length(),(unsigned)W);
  for(unsigned i=0; i<len; i++)
    x.back_cast()[i]=or_table[x.back_cast()[i]][y[i]];
  return x.back_cast();
}
template<class X,int W>
X& operator^= (t_base<X>& x,const sc_uint<W>& y)
{
  const unsigned len = sc_bv_ns::min(x.back_cast().length(),(unsigned)W);
  for(unsigned i=0; i<len; i++)
    x.back_cast()[i]=xor_table[x.back_cast()[i]][y[i]];
  return x.back_cast();
}
#endif

template<class X> X& t_base<X>::operator^= (unsigned int y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)^y|control);
  return back_cast();
}
template<class X> X& t_base<X>::operator&= (unsigned int y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)&y|control);
  return back_cast();
}
template<class X> X& t_base<X>::operator|= (unsigned int y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)|y|control);
  return back_cast();
}

template<class X> X& t_base<X>::operator^= (int y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)^y|control);
  return back_cast();
}
template<class X> X& t_base<X>::operator&= (int y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)&y|control);
  return back_cast();
}
template<class X> X& t_base<X>::operator|= (int y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)|y|control);
  return back_cast();
}

template<class X> X& t_base<X>::operator^= (unsigned long y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)^y|control);
  return back_cast();
}
template<class X> X& t_base<X>::operator&= (unsigned long y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)&y|control);
  return back_cast();
}
template<class X> X& t_base<X>::operator|= (unsigned long y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)|y|control);
  return back_cast();
}

template<class X> X& t_base<X>::operator^= (long y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)^y|control);
  return back_cast();
}
template<class X> X& t_base<X>::operator&= (long y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)&y|control);
  return back_cast();
}
template<class X> X& t_base<X>::operator|= (long y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)|y|control);
  return back_cast();
}

template<class X>
unsigned long t_base<X>::to_anything() const
{
  if(back_cast().length()>1)
    throw sc_edata(2002);
  if(back_cast().length()==0)
    return 0; // assume empty vector is equivalent to 0
  if(back_cast().get_cword(0))
    throw sc_edata(2003);
  return back_cast().get_word(0);
}
template<class T>
sc_string t_base<T>::to_string()const
{
   unsigned N = back_cast().length();
   sc_string buff(N+1);
   for(unsigned i=0; i<N; i++)
     buff.set(i, logic_to_char[back_cast()[N-i-1]&3]);
   buff.set(N,0);
   return buff;
}
template<class T>
const sc_string t_base<T>::to_string(sc_numrep base) const
{ // slow
#if defined(FORMATTED_STRING_CONVERSION)
  sc_unsigned us(to_string());
  return us(base);
#else
  throw sc_exception(1);
#endif
}

template<class T>
void t_base<T>::to_string( char* buffer ) const
{
  unsigned N = back_cast().length();
  for(unsigned i=0; i<N; i++)
    buffer[i] = logic_to_char[back_cast().get_bit(N-i-1)&3];
  buffer[N]=0;
}

template<class X>
ostream& operator<<(ostream& os, const t_base<X>& f)
{
  return os<<f.back_cast().to_string();
}

// range select
template<class T>
sc_range<T> t_base<T>::range(unsigned hi_bit, unsigned lo_bit)
{
  return sc_range<T>(back_cast(),hi_bit,lo_bit);
}

// operators returning bitwise proxies
template<class X,class Y>
const sc_bitwise_and<X,Y> operator& (const t_base<X>& x,const t_base<Y>& y)
{ return sc_bitwise_and<X,Y>(x.back_cast(),y.back_cast()); }
template<class X,class Y>
const sc_bitwise_or<X,Y> operator| (const t_base<X>& x,const t_base<Y>& y)
{ return sc_bitwise_or<X,Y>(x.back_cast(),y.back_cast()); }
template<class X,class Y>
const sc_bitwise_xor<X,Y> operator^ (const t_base<X>& x,const t_base<Y>& y)
{ return sc_bitwise_xor<X,Y>(x.back_cast(),y.back_cast());}
// operators returning negation proxies
template<class X> const sc_not<X> t_base<X>::operator~ () const
{ return sc_not<X>(back_cast()); }
// operators returning concat functions  proxies
template<class X,class Y>
inline sc_concat<X,Y> operator,(t_base<X>& x, t_base<Y>& y)
{ return sc_concat<X,Y>(x.back_cast(),y.back_cast());}

//---------------------------------------------------------------------------
#ifndef sc_lvH
#define sc_lvH

//---------------------------------------------------------------------------
template<int W>
class sc_lv : public sc_lv_base
{
	void verify_length() { if (length() != W) throw sc_edata(2000); }
public:
	sc_lv() :sc_lv_base(W){} // uninitialized
	explicit sc_lv(long init_value) :sc_lv_base(W, init_value){}
	explicit sc_lv(bool init_value) :sc_lv_base(W, (long)init_value){}
	// Copy constructor
	sc_lv(const sc_lv& y) :sc_lv_base(y){}
	sc_lv(const char* s) :sc_lv_base(W){ assign_(s); }
	sc_lv(int v) :sc_lv_base(W){ (*this) = v; }
	sc_lv(const sc_bool_vector& v) :sc_lv_base(v){ verify_length(); }
	sc_lv(const sc_logic_vector& v) :sc_lv_base(v){ verify_length(); }
	explicit sc_lv(char c) :sc_lv_base(W, char_to_logic[c]){}
#if !defined(__SUNPRO_CC)
	template<class X, class Y> sc_lv(const t_base<Y>& y)
	{
		init(y.cast_back().length()); assign_(y);
	}
	template<class T> sc_lv& operator=(const t_base<T>& t)
	{
		sc_bv_ns::assign_(*this, t); return *this;
	}
#else
	sc_lv(const sc_bv<W>& t){ convert(t); verify_length(); }
	sc_lv(const sc_lv_base& y) :sc_lv_base(y){ verify_length(); }
	sc_lv(const sc_bv_base& t){ convert(t); verify_length(); }
#endif

	virtual sc_lv_base* clone(){ return new sc_lv<W>(*this); }
	// Assignments may be made to a sc_lv from various types of
	// objects.
	sc_lv& operator=(const sc_lv& t)
	{
		return static_cast<sc_lv&>(sc_lv_base::operator=(t));
	}
	sc_lv& operator=(const char* t)
	{
		return static_cast<sc_lv&>(sc_lv_base::operator=(t));
	}
	sc_lv& operator=(const bool* t)
	{
		return static_cast<sc_lv&>(sc_lv_base::operator=(t));
	}

	sc_lv& operator=(const sc_unsigned& t)
	{
		return static_cast<sc_lv&>(sc_lv_base::operator=(t));
	}
	sc_lv& operator=(const sc_signed& t)
	{
		return static_cast<sc_lv&>(sc_lv_base::operator=(t));
	}
	sc_lv& operator=(long t)
	{
		return static_cast<sc_lv&>(sc_lv_base::operator=(t));
	}
	sc_lv& operator=(unsigned long t)
	{
		return static_cast<sc_lv&>(sc_lv_base::operator=(t));
	}
	sc_lv& operator=(int t)
	{
		return static_cast<sc_lv&>(sc_lv_base::operator=(t));
	}
	sc_lv& operator=(unsigned t)
	{
		return static_cast<sc_lv&>(sc_lv_base::operator=(t));
	}
	sc_lv& operator=(char t)
	{
		return static_cast<sc_lv&>(sc_lv_base::operator=(t));
	}
	sc_lv& operator=(const sc_bit& t)
	{
		return static_cast<sc_lv&>(sc_lv_base::operator=(t));
	}
	sc_lv& operator=(int64 t)
	{
		return static_cast<sc_lv&>(sc_lv_base::operator=(t));
	}
	sc_lv& operator=(uint64 t)
	{
		return static_cast<sc_lv&>(sc_lv_base::operator=(t));
	}
	sc_lv& operator=(const sc_int<W>& t){ return operator=((int64)t); }
	sc_lv& operator=(const sc_uint<W>& t){ return operator=((uint64)t); }
	sc_lv& operator=(const sc_bool_vector& v)
	{
		return static_cast<sc_lv&>(sc_lv_base::operator=(v));
	}
	sc_lv& operator=(const sc_logic_vector& v)
	{
		return static_cast<sc_lv&>(sc_lv_base::operator=(v));
	}

	virtual void resize(unsigned long new_size)
	{
		throw sc_edata(2006, " - sc_lv");
	}
};

// ----------------------------------------------------------------------------
// inline and template functions
// ----------------------------------------------------------------------------
// inline functions
inline long sc_lv_base::get_bit(unsigned n) const
{
	return data[n / ul_size] >> n%ul_size & 1 |
		control[n / ul_size] >> n%ul_size << 1 & 2;
}

inline void sc_lv_base::set_bit(unsigned n, long value)
{
	data[n / ul_size] |= 1 << n%ul_size; // set 1
	control[n / ul_size] |= 1 << n%ul_size; // set 1
	data[n / ul_size] &= value << n%ul_size | ~(1ul << n%ul_size);
	control[n / ul_size] &= value >> 1 << n%ul_size | ~(1ul << n%ul_size);
}
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------

template<class X>
inline const sc_lv_base t_base<X>::operator,( const char* s) const
{
  // some compilers violate 13.3.3p1 (the best viable function)
  // and use non-const back_cast() function if used below
  // have to explicitly call const version
	sc_lv_base result = (back_const_cast(), sc_lv_base(s));
  return result;
}
// many compilers do not perform RVO, so we have to repeat the function
template<class X>
inline const sc_lv_base operator,( const char* s, const t_base<X>& x)
{
	sc_lv_base result = (sc_lv_base(s), x.back_const_cast());
  return result;
}

template<class X, class Y>
inline void check_bounds(const t_base<X>& x,const t_base<Y>& y)
{ // verify compatibility for assignment
  if(x.back_cast().length()!=y.back_cast().length())
    throw sc_edata(1001);
}
template<class X>
inline void t_base<X>::check_bounds(const sc_bool_vector& v)
{
  if(v.length()!=back_cast().length())
    throw sc_edata(1001);
}
template<class X>
inline void t_base<X>::check_bounds(const sc_logic_vector& v)
{
  if(v.length()!=back_cast().length())
    throw sc_edata(1001);
}
template<class X>
inline void t_base<X>::check_bounds(unsigned n) // check if bit n accessible
{
  if((int)n>=back_cast().length()) throw sc_edata(1000);
}
template<class X>
inline void t_base<X>::check_wbounds(unsigned n)// check if word n accessible
{
   if(n>(back_cast().length()-1)/ul_size) throw sc_edata(1000);
}

// non-member template for common assignments
template<class X, class Y>
void assign_(t_base<X>& x, const t_base<Y>& y)
{
  check_bounds(x,y);
  const unsigned reserved = (x.back_cast().length()-1)/ul_size +1;
  for(register unsigned i =0; i<reserved; i++)
  {
     x.back_cast().set_word(i,y.back_cast().get_word(i));
     x.back_cast().set_cword(i,y.back_cast().get_cword(i));
  }
  x.back_cast().clean_tail();
}
template<class X> X& t_base<X>::assign_(const char* bb)
{ // ATTN: only common bits are assigned
  const sc_string b = convert_to_logic(bb);
#ifndef _MSC_VER
  int len = sc_bv_ns::min(back_cast().length(),b.length());
#else
  int len = min(back_cast().length(),b.length());
#endif
  for(register int i =0; i<len; i++)
     back_cast().set_bit(i,char_to_logic[b[len-i-1]]);
  for(register int i =len; i<back_cast().length(); i++)
     back_cast().set_bit(i,0);
  back_cast().clean_tail();
  return back_cast();
}
template<class X> X& t_base<X>::assign_(const bool* b)
{
  // verification of the length is impossible
  // may throw
  for(register int i =0; i<back_cast().length(); i++)
     back_cast().set_bit(i,b[i]);
  back_cast().clean_tail();
  return back_cast();
}
template<class X> X& t_base<X>::assign_( unsigned long nv )
{
  back_cast().set_word(0,nv);
  back_cast().set_cword(0,0);
  int words = (back_cast().length()-1)/ul_size+1;
  // must fill the rest with 0
  for(int i=1; i<words; i++)
  {
    back_cast().set_word(i,0);
    back_cast().set_cword(i,0);
  }
  return back_cast();
}

template<class X> X& t_base<X>::assign_( int64 v )
{
  back_cast().set_word(0,(uint64)v & ~0ul);
  back_cast().set_cword(0,0);
  if(back_cast().length()>1)
  {
    back_cast().set_word(1,(uint64)v>>ul_size & ~0ul);
    back_cast().set_cword(1,0);
    int words = (back_cast().length()-1)/ul_size+1;
    // must fill the rest with 0
    for(int i=2; i<words; i++)
    {
      back_cast().set_word(i,0);
      back_cast().set_cword(i,0);
    }
  }
  return back_cast();
}

template<class X> X& t_base<X>::assign_( uint64 v )
{
  back_cast().set_word(0,v & ~0ul);
  back_cast().set_cword(0,0);
  if(back_cast().length()>1)
  {
    back_cast().set_word(1,v>>ul_size & ~0ul);
    back_cast().set_cword(1,0);
    int words = (back_cast().length()-1)/ul_size+1;
    // must fill the rest with 0
    for(int i=2; i<words; i++)
    {
      back_cast().set_word(i,0);
      back_cast().set_cword(i,0);
    }
  }
  return back_cast();
}

template<class X> X& t_base<X>::assign_(const sc_bool_vector& v)
{ // slow
  check_bounds(v);
  for(int i=0; i<back_cast().length(); i++)
    back_cast().set_bit(i,v[i]);
  back_cast().clean_tail();
  return back_cast();
}

template<class X> X& t_base<X>::assign_(const sc_logic_vector& v)
{ // slow
  check_bounds(v);
  for(int i=0; i<back_cast().length(); i++)
  // must convert sc_logic values to longs used by sc_lv
    back_cast().set_bit(i,sc_logic::to_long_table[v[i]]);
  back_cast().clean_tail();
  return back_cast();
}

// reduction operators
template<class X>
long t_base<X>::and_reduce()  const
{
  const unsigned len = back_cast().length();
  const unsigned full_words = len/ul_size;
  register bool finished = false;
  register bool zero=false;
  register bool undefined=false;
  register unsigned i = 0;
  // no need to go beyond the first 0
  while(!finished && i<full_words)
  {
    zero =  back_cast().get_word(i)!=~0ul;
    undefined =  back_cast().get_cword(i)!=0;
    finished = zero || undefined;
    i++;
  }
  i=full_words*ul_size;
  while(!finished && i<len)
  {
    register long value = back_cast().get_bit(i);
    zero =  !(value & 1);
    undefined =  value & 2;
    finished = zero || undefined;
    i++;
  }
  if(undefined) return 3;
  else
    return !zero;
}

template<class X>
long t_base<X>::or_reduce()   const
{
  const unsigned len = back_cast().length();
  const unsigned full_words = len/ul_size;
  register bool finished = false;
  register bool zero=false;
  register bool undefined=false;
  register unsigned i = 0;
  // no need to go beyond the first 1
  while(!finished && i<full_words)
  {
    zero =  back_cast().get_word(i)==0;
    undefined =  back_cast().get_cword(i)!=0;
    finished = !zero || undefined;
    i++;
  }
  i=full_words*ul_size;
  while(!finished && i<len)
  {
    register long value = back_cast().get_bit(i);
    zero =  !(value & 1);
    undefined =  value & 2;
    finished = !zero || undefined;
    i++;
  }
  if(undefined) return 3;
  else
    return !zero;
}
template<class X>
long t_base<X>::xor_reduce()  const
{
  const unsigned len = back_cast().length();
  const unsigned n_words = (len-1)/ul_size+1;
  register unsigned long result = 0;
  register unsigned i = 0;
  register bool undefined = false;
  while(!undefined && i<n_words)
  {
    result ^= back_cast().get_word(i);
    undefined = back_cast().get_cword(i)!=0;
    i++;
  }
  i = 1;
  while(!undefined && i<ul_size)
  {
    result ^=(result>>i)&1;
    i++;
  }
  result = result &1;
  if(undefined)
    return 3;
  else
    return result;
}
// l-value shift operators
template<class X> inline
const sc_shift_left<X>  operator<<(const t_base<X>& x,unsigned n)
{
   return sc_shift_left<X>(x.back_cast(),n);
}
template<class X>  inline
const sc_shift_right<X> operator>>(const t_base<X>& x,unsigned n)
{
   return sc_shift_right<X>(x.back_cast(),n);
}

// additional functions required by VSIA
template<class X>
X& t_base<X>::operator <<= (int i) // left shift with assignment
{
  X temp((*this)<<i);
  (*this) = temp;
  return back_cast();
}
template<class X>
X& t_base<X>::operator >>= (int i) // right shift with assignment
{
  X temp((*this)>>i);
  (*this) = temp;
  return back_cast();
}

// -------------------- end of t_base -----------------------------------------

//---------------------------------------------------------------------------
// sc_bit_proxy
//-------------------------------------------------------------------------
template<class X>
inline sc_bit_proxy<X>& sc_bit_proxy<X>::operator=(const sc_bit_proxy<X>& r)
{ // set the value
  rep.set_bit(index,r);
  return *this;
}
template<class X>
inline sc_bit_proxy<X>& sc_bit_proxy<X>::operator=(long r)
{
  rep.set_bit(index,r);
  return *this;
}
template<class X>
inline sc_bit_proxy<X>::operator long() const
{
  return const_cast<const X&>(rep)[index];
}
template<class X>
inline sc_bit_proxy<X>& sc_bit_proxy<X>::operator&=(bool r)
{
  rep.set_bit(index,r && const_cast<const X&>(rep)[index]);
  return *this;
}
template<class X>
inline sc_bit_proxy<X>& sc_bit_proxy<X>::operator|=(bool r)
{
  rep.set_bit(index,r || const_cast<const X&>(rep)[index]);
  return *this;
}
template<class X>
inline sc_bit_proxy<X>& sc_bit_proxy<X>::operator^=(bool r)
{
  rep.set_bit(index,r ^ const_cast<const X&>(rep)[index]);
  return *this;
}
template<class X>
inline bool sc_bit_proxy<X>::operator ~()const
{
  !const_cast<const X&>(rep)[index];
}
template<class X>
inline sc_bit_proxy<X>& sc_bit_proxy<X>::operator &=(const sc_bit_proxy<X>& s)
{
  return operator&=(const_cast<const X&>(rep)[index]);
}
template<class X>
inline sc_bit_proxy<X>& sc_bit_proxy<X>::operator |=(const sc_bit_proxy<X>& s)
{
  return operator|=(const_cast<const X&>(rep)[index]);
}
template<class X>
inline sc_bit_proxy<X>& sc_bit_proxy<X>::operator ^=(const sc_bit_proxy<X>& s)
{
  return operator^=(const_cast<const X&>(rep)[index]);
}
template<class X>
inline bool sc_bit_proxy<X>::operator &(const sc_bit_proxy<X>& s)const
{
  return operator&(const_cast<const X&>(rep)[index]);
}
template<class X>
inline bool sc_bit_proxy<X>::operator |(const sc_bit_proxy<X>& s)const
{
  return operator|(const_cast<const X&>(rep)[index]);
}
template<class X>
inline bool sc_bit_proxy<X>::operator ^(const sc_bit_proxy<X>& s)const
{
  return operator^(const_cast<const X&>(rep)[index]);
}
// common interface
template<class X>
inline long sc_bit_proxy<X>::get_bit(unsigned bit_number) const
{
  if(bit_number==0)
    return rep.get_bit(index);
  else
    throw sc_edata(1000);
}
template<class X>
inline void sc_bit_proxy<X>::set_bit(unsigned bit_number, long value)
{
  if(bit_number==0)
    rep.set_bit(index,value);
  else
    throw sc_edata(1000);
}
template<class X>
inline unsigned long sc_bit_proxy<X>::get_word(unsigned i) const
{
  return get_bit(i)&1;
}
template<class X>
inline void sc_bit_proxy<X>::set_word(unsigned i, unsigned long w)
{
  if(i==0)
    rep.set_word(index,w);
  else
    throw sc_edata(1000);
}
template<class X>
inline unsigned long sc_bit_proxy<X>::get_cword(unsigned i) const
{
  if(i==0)
    return get_bit(i)&2;
  else
    throw sc_edata(1000);
}
template<class X>
inline void sc_bit_proxy<X>::set_cword(unsigned i, unsigned long w)
{
  if(i==0)
    rep.set_cword(index,w);
  else
    throw sc_edata(1000);
}
template<class X>
inline int sc_bit_proxy<X>::length()const
{
  return 1;
}
// ----------------------------------------------------------------------------
// sc_shift_right
// ----------------------------------------------------------------------------
template<class X>
inline long sc_shift_right<X>::get_bit(unsigned i) const
{
  int ind = i+n;
  if(ind < b.length())
    return b.get_bit(ind);
  else
    return 0;
}
template<class X>
inline unsigned long sc_shift_right<X>::get_word(unsigned i)const
{
  unsigned hi = b.get_word(i)<<n; // right end filled with 0
  unsigned lo = i>0?b.get_word(i-1)>>(ul_size-n) : 0;
  return lo | hi; // glue them together
}
template<class X>
inline unsigned long sc_shift_right<X>::get_cword(unsigned i)const
{
  unsigned hi = b.get_cword(i)<<n; // right end filled with 0
  unsigned lo = i>0?b.get_cword(i-1)>>(ul_size-n) : 0;
  return lo | hi; // glue them together
}

// ----------------------------------------------------------------------------
// sc_shift_left
// ----------------------------------------------------------------------------
template<class X>
inline long sc_shift_left<X>::get_bit(unsigned i) const
{
  int ind = i-n;
  if(ind < b.length())
    return b.get_bit(ind);
  else
    return 0;
}
template<class X>
inline unsigned long sc_shift_left<X>::get_word(unsigned i)const
{
  unsigned lo = b.get_word(i)>>n; // left end filled with 0
  unsigned hi = i<b.length()-1?b.get_word(i+1)<<(ul_size-n) : 0;
  return lo | hi; // glue them together
}

template<class X>
inline unsigned long sc_shift_left<X>::get_cword(unsigned i)const
{
  unsigned lo = b.get_cword(i)>>n; // left end filled with 0
  unsigned hi = i<b.length()-1?b.get_cword(i+1)<<(ul_size-n) : 0;
  return lo | hi; // glue them together
}
//---------------------------------------------------------------------------
// sc_bitwise_and
//---------------------------------------------------------------------------
template<class X, class Y>
long sc_bitwise_and<X,Y>::get_bit(unsigned i) const
{
  const unsigned long b1_ = b1.get_bit(i);
  const unsigned long b2_ = b2.get_bit(i);
  const unsigned control = (b1_>>1 & b2_ | (b1_&b2_)>>1 | b1_ & b2_>>1)&1;
  return (b1_&b2_|control)&1 | control<<1;
}
template<class X, class Y>
unsigned long sc_bitwise_and<X,Y>::get_cword(unsigned i) const
{
  const unsigned long c1 = b1.get_cword(i);
  const unsigned long c2 = b2.get_cword(i);
  const unsigned long b1_ = b1.get_word(i);
  const unsigned long b2_ = b2.get_word(i);
  return c1&b2_ | c1&c2 | b1_&c2;
}
//---------------------------------------------------------------------------
// sc_bitwise_or
//---------------------------------------------------------------------------
template<class X, class Y>
unsigned long sc_bitwise_or<X,Y>::get_word(unsigned i)const
{
  return b1.get_word(i)|b2.get_word(i) | get_cword(i);
}
template<class X, class Y>
unsigned long sc_bitwise_or<X,Y>::get_cword(unsigned i) const
{
  return b1.get_cword(i)&~b2.get_word(i) | b1.get_cword(i)&b2.get_cword(i) |
         ~b1.get_word(i)&b2.get_cword(i);
}
//---------------------------------------------------------------------------
// sc_bitwise_xor
//---------------------------------------------------------------------------
template<class X, class Y>
unsigned long sc_bitwise_xor<X,Y>::get_word(unsigned i)const
{
  return b1.get_word(i)^b2.get_word(i) | get_cword(i);
}
template<class X, class Y>
unsigned long sc_bitwise_xor<X,Y>::get_cword(unsigned i) const
{
  return b1.get_cword(i) | b2.get_cword(i);
}
//---------------------------------------------------------------------------
// sc_not
//---------------------------------------------------------------------------
template<class X>
long sc_not<X>::get_bit(unsigned n) const
{
  long x=b.get_bit(n);
  return x<2 ? !x : Log_X /* X-value */;
}
// ----------------------------------------------------------------------------
// sc_concat
// ----------------------------------------------------------------------------
template<class X, class Y>
inline void sc_concat<X,Y>::set_bit(unsigned i, long w)
{
  const unsigned len = bv2.length();
  if(i<len)
    bv2.set_bit(i,w);
  else
    if(i<len+bv1.length())
      bv1.set_bit(i-len,w);
    else
      throw sc_edata(1000);
}
template<class X, class Y>
inline unsigned long sc_concat<X,Y>::get_word(unsigned i)const
{
  const unsigned shift = bv2.length()%ul_size;
  const unsigned border = bv2.length()/ul_size;
  if(shift==0)
  {
    if(i<border)
      return bv2.get_word(i);
    else
      return bv1.get_word(i-border);
  }
  else
    if(i<border)
      return bv2.get_word(i);
    else
    {// stiches are necessary
      if(i==border) //the last word of b1
        return bv2.get_word(i) & ~0ul>>(ul_size-shift) | bv1.get_word(0)<<shift;
      else // from the second word
      if(i<(bv2.length()+bv1.length()-1)/ul_size+1)
        return bv1.get_word(i-border-1)>>(ul_size-shift) |
               bv1.get_word(i-border)<<shift;
      else
        throw sc_edata(1000);
    }
}

template<class X, class Y>
inline void sc_concat<X,Y>::set_word(unsigned i, unsigned long w)
{
  unsigned shift = bv2.length()%ul_size;
  unsigned border = bv2.length()/ul_size; // number of full words
  if(shift==0)
  {
    if(i<border)
      bv2.set_word(i,w);
    else
      bv1.set_word(i-border,w);
  }
  else
    if(i<border)
      bv2.set_word(i,w);
    else
    if(i<(bv1.length()+bv1.length()-1)/ul_size+1)
    {// stiches are necessary
      if(i==border) //the last word of b1
      {
        bv2.set_word(i,w & ~0ul>>(ul_size-shift));
        bv1.set_word(0,bv1.get_word(0)& ~0ul<<(ul_size-shift) | w>>shift );
      }
      else // from the second word
      {
        bv1.set_word(i-border-1,bv1.get_word(i-border-1)& ~0ul>>shift |
            w<<(ul_size-shift));
        bv1.set_word(i-border,bv1.get_word(i-border)&~0ul<<(ul_size-shift)|
            w>>shift);
      }
    }
    else
      throw sc_edata(1000);
}
template<class X, class Y>
inline unsigned long sc_concat<X,Y>::get_cword(unsigned i)const
{
  const unsigned shift = bv2.length()%ul_size;
  const unsigned border = bv2.length()/ul_size;
  if(shift==0)
  {
    if(i<border)
      return bv2.get_cword(i);
    else
      return bv1.get_cword(i-border);
  }
  else
    if(i<border)
      return bv2.get_cword(i);
    else
    {// stiches are necessary
      if(i==border) //the last word of b1
        return bv2.get_cword(i) & ~0ul>>(ul_size-shift) | bv1.get_cword(0)<<shift;
      else // from the second word
      if(i<(bv2.length()+bv1.length()-1)/ul_size+1)
        return bv1.get_cword(i-border-1)>>(ul_size-shift) |
               bv1.get_cword(i-border)<<shift;
      else
        throw sc_edata(1000);
    }
}

template<class X, class Y>
inline void sc_concat<X,Y>::set_cword(unsigned i, unsigned long w)
{
  unsigned shift = bv2.length()%ul_size;
  unsigned border = bv2.length()/ul_size; // number of full words
  if(shift==0)
  {
    if(i<border)
      bv2.set_cword(i,w);
    else
      bv1.set_cword(i-border,w);
  }
  else
    if(i<border)
      bv2.set_cword(i,w);
    else
    if(i<(bv2.length()+bv1.length()-1)/ul_size+1)
    {// stiches are necessary
      if(i==border) //the last word of b1
      {
        bv2.set_cword(i,w & ~0ul>>(ul_size-shift));
        bv1.set_cword(0,bv1.get_cword(0)& ~0ul<<(ul_size-shift) | w>>shift );
      }
      else // from the second word
      {
        bv1.set_cword(i-border-1,bv1.get_cword(i-border-1)& ~0ul>>shift |
            w<<(ul_size-shift));
        bv1.set_cword(i-border,bv1.get_cword(i-border)&~0ul<<(ul_size-shift)|
            w>>shift);
      }
    }
    else
      throw sc_edata(1000);
}

template<class X, class Y>
inline long sc_concat<X,Y>::get_bit(unsigned n) const
{
  const unsigned len = bv2.length();
  if(n<len)
    return bv2.get_bit(n);
  else
    return bv1.get_bit(n-len);
}
// ----------------------------------------------------------------------------
// sc_range
// ----------------------------------------------------------------------------
#if !defined(__SUNPRO_CC) && !defined(_MSC_VER) && 0
template<class X> template<class T>
inline sc_range<X>& sc_range<X>::operator = (const t_base<T>& b)
{
  unsigned len = sc_bv_ns::min(length(),b.back_cast().length());
  for(unsigned i=0; i<len; i++)
    if(!reversed())
      bv.set_bit(lo_bit+i, b.back_cast()[i]);
    else
      bv.set_bit(lo_bit-i, b.back_cast()[i]);
  return *this;
}
#endif
template<class X>
inline sc_range<X>& sc_range<X>::operator = (const sc_range<X>& b)
{
  unsigned len = sc_bv_ns::min(length(),b.length());
  for(unsigned i=0; i<len; i++)
    if(!reversed())
      bv.set_bit(lo_bit+i, b[i]);
    else
      bv.set_bit(lo_bit-i, b[i]);
  return *this;
}
template<class X>
sc_range<X>& sc_range<X>::operator = (const char* b)
{
  unsigned len = sc_bv_ns::min(length(),strlen(b));
  for(unsigned i=0; i<len; i++)
    if(!reversed())
      bv.set_bit(lo_bit+i, char_to_logic[b[len-i-1]]);
    else
      bv.set_bit(lo_bit-i, char_to_logic[b[len-i-1]]);
  return *this;
}
template<class X>
sc_range<X>& sc_range<X>::operator = (const bool* b)
{
  unsigned len = length();
  for(unsigned i=0; i<len; i++)
    if(!reversed())
      bv.set_bit(lo_bit+i, b[i]);
    else
      bv.set_bit(lo_bit-i, b[i]);
  return *this;
}

template<class X>
inline long sc_range<X>::get_bit(unsigned n) const
{
  if(reversed())
    return bv.get_bit(lo_bit-n);
  else
    return bv.get_bit(lo_bit+n);
}

template<class X>
inline void sc_range<X>::set_bit(unsigned n, long value)
{
  if(reversed())
    bv.set_bit(lo_bit-n, value);
  else
    bv.set_bit(lo_bit+n, value);
}

template<class X>
unsigned long sc_range<X>::get_word(unsigned i)const
{
  int n1=0;
  int n2=0;
  unsigned long result = 0;
  unsigned k=0;
  if(reversed())
  {
    n1 = sc_bv_ns::max((int)hi_bit,(int)(lo_bit - i*ul_size));
    n2 = sc_bv_ns::max((int)hi_bit-1,(int)(n1-ul_size));
    for(int n = n1; n>n2; n--)
      result |= (const_cast<const X&>(bv)[n]&1ul)<<k++;
  }
  else
  {
    n1 = sc_bv_ns::min(lo_bit+i*ul_size,hi_bit);
    n2 = sc_bv_ns::min(n1 + ul_size,hi_bit+1);
    for(int n = n1; n<n2; n++)
      result |= (const_cast<const X&>(bv)[n]&1ul)<<k++;
  }
  return result;
}

template<class X>
void sc_range<X>::set_word(unsigned i, unsigned long w)
{
  int n1=0;
  int n2=0;
  unsigned long result = 0;
  unsigned k=0;
  if(reversed())
  {
    n1 = sc_bv_ns::max((int)hi_bit,(int)(lo_bit - i*ul_size));
    n2 = sc_bv_ns::max((int)hi_bit-1,(int)(n1-ul_size));
    for(int n = n1; n>n2; n--)
      bv.set_bit(n,(w>>k++)&1ul | const_cast<const X&>(bv)[n]&2ul);
  }
  else
  {
    n1 = sc_bv_ns::min(lo_bit+i*ul_size,hi_bit);
    n2 = sc_bv_ns::min(n1 + ul_size,hi_bit+1);
    for(int n = n1; n<n2; n++)
      bv.set_bit(n,(w>>k++)&1ul | const_cast<const X&>(bv)[n]&2ul);
  }
}

template<class X>
unsigned long sc_range<X>::get_cword(unsigned i)const
{
  int n1=0;
  int n2=0;
  unsigned long result = 0;
  unsigned k=0;
  if(reversed())
  {
    n1 = sc_bv_ns::max((int)hi_bit,(int)(lo_bit - i*ul_size));
    n2 = sc_bv_ns::max((int)hi_bit-1,(int)(n1-ul_size));
    for(int n = n1; n>n2; n--)
      result |= (const_cast<const X&>(bv)[n]&2ul)<<k++;
  }
  else
  {
    n1 = sc_bv_ns::min(lo_bit+i*ul_size,hi_bit);
    n2 = sc_bv_ns::min(n1 + ul_size,hi_bit+1);
    for(int n = n1; n<n2; n++)
      result |= (const_cast<const X&>(bv)[n]&2ul)<<k++;
  }
  return result;
}
template<class X>
void sc_range<X>::set_cword(unsigned i, unsigned long w)
{
  int n1=0;
  int n2=0;
  unsigned long result = 0;
  unsigned k=0;
  if(reversed())
  {
    n1 = sc_bv_ns::max((int)hi_bit,(int)(lo_bit - i*ul_size));
    n2 = sc_bv_ns::max((int)hi_bit-1,(int)(n1-ul_size));
    for(int n = n1; n>n2; n--)
      bv.set_bit(n,((w>>k++)&1ul)<<1 | const_cast<const X&>(bv)[n]&1ul);
  }
  else
  {
    n1 = sc_bv_ns::min(lo_bit+i*ul_size,hi_bit);
    n2 = sc_bv_ns::min(n1 + ul_size,hi_bit+1);
    for(int n = n1; n<n2; n++)
      bv.set_bit(n,((w>>k++)&1ul)<<1 | const_cast<const X&>(bv)[n]&1ul);
  }
}


// ----------------------------------------------------------------------------
// sc_bv_base templates
// ----------------------------------------------------------------------------
#if defined(__BCPLUSPLUS__)
#pragma warn -ccc
#pragma warn -rch
#endif
inline long sc_bv_base::get_bit(unsigned n) const
{ // gcc compiles idiv into a function call
  // instead of calling native processor command
  // ignore compiler warnings that conditions are
  // always true
  if(ul_size==32)
    return (data[n>>5] & (1<< (n & 0x1F)))!=0;
  else
    return (data[n/ul_size]&(1<<n%ul_size)) !=0;
}

inline void sc_bv_base::set_bit(unsigned n, long value)
{
  if(value<0 || value>1)
    throw sc_edata(2003,"can''t assign to sc_bv");
  if(ul_size==32)
  {
    if(value)
      data[n>>5] |= 1<< (n & 0x1F);
    else
      data[n>>5] &= ~(1<< (n & 0x1F));
  }
  else
  {
    if(value)
      data[n/ul_size] |= 1<< n%ul_size;
    else
      data[n/ul_size] &= ~(1<< n%ul_size);
  }
}
#if defined(__BCPLUSPLUS__)
#pragma warn +ccc
#pragma warn +rch
#endif

inline void sc_bv_base::set_cword(unsigned i, unsigned long w)
{
  check_wbounds(i);
  if(w) throw sc_edata(2004);
}

// ----------------------------------------------------------------------------
// sc_bv<> templates
// ----------------------------------------------------------------------------
template<int W>
sc_bv<W>::sc_bv(const char* ss)
{ // slow
   const sc_string s = convert_to_bin(ss);
   const unsigned s_length = s.length();
   if(s_length==0)
     throw sc_edata(2001);
   data = new unsigned long[(W-1)/ul_size +1]; // can throw
   N=W;
   assign_(s);
   clean_tail();
}
template<int W>
sc_bv<W>::sc_bv(const sc_logic_vector& v)
{
   if(W!=(unsigned)v.length())
    throw sc_edata(2000);
   const unsigned reserved = (W-1)/ul_size +1;
   data = new unsigned long[reserved]; // can throw
   N=W;
   assign_(v);
   clean_tail();
}
template<int W>
sc_bv<W>& sc_bv<W>::operator=(const sc_logic_vector& v)
{
  return static_cast<sc_bv<W> >(assign_(v));
}

// get word from array of type T with shift
template<class T>
unsigned long array_get_word(const T& p, unsigned index)
{
  const unsigned shift = index%ul_size;
  const unsigned word = index / ul_size;
  unsigned long result;
  if(index>=p.length())
    throw sc_edata(1000);
  if(shift == 0)
    result = p.get_word(word);
  else
    if((word+1)*ul_size>p.length()) // incomplete word
      result = p.get_word(word)>>shift;
    else
      result = p.get_word(word)>>shift | p.get_word(word+1)<<(ul_size - shift);
  // zero the end bits
  if(index+ul_size > length)
    return result & ~0ul>>(shift-length%ul_size);
  else
    return result;
}
// set word to array of type T with shift
template<class T>
void array_set_word(T& p, const T t, unsigned length, unsigned index)
{
  const unsigned shift = index%ul_size;
  const unsigned word = index / ul_size;
  const unsigned len_shift = length % T_zise;
  T result;
  if(index>=length)
    throw sc_edata(1000);
  if(index+ul_size > length)
  {
    if(shift == 0)
      p.set_word(word, p.get_word(word)>>length%ul_size<< length%ul_size
        | t<<(ul_size-length%ul_size)>>(ul_size-length%ul_size));
    else
      if((word+1)*ul_size>length) // incomplete word
      {
        T mask = ~0ul>>shift<<(shift+ul_size-len_shift)<< (ul_size-len_shift);
        p.set_word(word, p.get_word(word) & ~mask |
          t<<(shift+ul_size-len_shift)>>(ul_size-len_shift));
      }
      else
      {
        p.set_word(word, p.get_word(word)<<(ul_size-shift)>>(ul_size-shift)
          | t<<shift);
        p.set_word(word+1, p.get_word(word+1)>>len_shift<<len_shift |
          t<<(shift-len_shift)>>(shift-len_shift) >>(ul_size-shift));
      }
  }
  else
    if(shift == 0)
      p.set_word(word)=t;
    else
      if((word+1)*ul_size>length) // incomplete word
        p.set_word(word, p.get_word(word)<<(ul_size-shift)>>(ul_size-shift)
         | t<<shift);
      else
      {
        p.set_word(word, p.get_word(word)<<(ul_size-shift)>>(ul_size-shift)
          | t<<shift);
        p.set_word(word+1, p.get_word(word+1)>>shift<<shift | t>>(ul_size-shift));
      }
}
}
//---------------------------------------------------------------------------
#endif
