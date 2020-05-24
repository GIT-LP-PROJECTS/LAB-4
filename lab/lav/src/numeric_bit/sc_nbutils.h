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

    sc_nbutils.h -- External and friend functions for both sc_signed and
    sc_unsigned classes.

    Original Author: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)
 
******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_UTILS_H
#define SC_UTILS_H

#if !defined(__BCPLUSPLUS__) && !defined(_MSC_VER) && !defined(i386)
#include <ieeefp.h>
#endif

#include "sc_nbdefs.h"

// Printint out warnings.
#define warn(EX, MSG) \
(void)((EX) || (warn_help(#EX, MSG, __FILE__, __LINE__), 0))

extern 
void 
warn_help(const char *ex, const char *msg, 
          const char *fname, int lnum);

#if defined(__BCPLUSPLUS__)
#pragma warn -8027
#endif
inline
void
is_valid_base(sc_numrep base)
{
  switch (base) {
  case SC_NOBASE: case SC_BIN: case SC_OCT: case SC_DEC: case SC_HEX: 
    break;
  case SC_BIN_US: case SC_BIN_SM: 
  case SC_OCT_US: case SC_OCT_SM:
  case SC_HEX_US: case SC_HEX_SM:
    printf("SystemC error: The _US and _SM of bases are not supported yet.\n");
    abort();
  default:
    printf("SystemC error: The base arg is invalid.\n");
    abort();
  }
}

// One transition of the FSM to find base and sign of a number.
extern
small_type 
fsm_move(char c, small_type &b, small_type &s, small_type &state);

// Find the base and sign of a number in v.
extern 
const char *
get_base_and_sign(const char *v, small_type &base, small_type &sign);

// Create a number out of v in base.
extern 
small_type 
vec_from_str(length_type unb, length_type und, digit_type *u, 
             const char *v, sc_numrep base = SC_NOBASE) ;

///////////////////////////////////////////////////////////////////////////
// Naming convention for the vec_ functions below:
//   vec_OP(u, v, w)  : computes w = u OP v.
//   vec_OP_on(u, v)  : computes u = u OP v if u has more digits than v.
//   vec_OP_on2(u, v) : computes u = u OP v if u has fewer digits than v.
//   _large           : parameters are vectors.
//   _small           : one of the parameters is a single digit.
//   Xlen             : the number of digits in X.
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// Functions for vector addition: w = u + v or u += v.
///////////////////////////////////////////////////////////////////////////

extern 
void 
vec_add(length_type ulen, const digit_type *u, 
        length_type vlen, const digit_type *v, digit_type *w);

extern 
void 
vec_add_on(length_type ulen, digit_type *u, 
           length_type vlen, const digit_type *v);

extern 
void 
vec_add_on2(length_type ulen, digit_type *u, 
            length_type vlen, const digit_type *v);

extern 
void 
vec_add_small(length_type ulen, const digit_type *u,
              digit_type v, digit_type *w);

extern 
void 
vec_add_small_on(length_type ulen, digit_type *u, digit_type v);

///////////////////////////////////////////////////////////////////////////
// Functions for vector subtraction: w = u - v, u -= v, or u = v - u.
///////////////////////////////////////////////////////////////////////////

extern 
void 
vec_sub(length_type ulen, const digit_type *u, 
        length_type vlen, const digit_type *v, digit_type *w);

extern 
void 
vec_sub_on(length_type ulen, digit_type *u, 
           length_type vlen, const digit_type *v);

extern 
void 
vec_sub_on2(length_type ulen, digit_type *u,
            length_type vlen, const digit_type *v);

extern 
void 
vec_sub_small(length_type ulen, const digit_type *u,
              digit_type v, digit_type *w);

extern 
void 
vec_sub_small_on(length_type ulen, digit_type *u, digit_type v);

///////////////////////////////////////////////////////////////////////////
// Functions for vector multiplication: w = u * v or u *= v.
///////////////////////////////////////////////////////////////////////////

extern 
void 
vec_mul(length_type ulen, const digit_type *u, 
        length_type vlen, const digit_type *v, digit_type *w);

extern 
void 
vec_mul_small(length_type ulen, const digit_type *u,
              digit_type v, digit_type *w);

extern 
void 
vec_mul_small_on(length_type ulen, digit_type *u, digit_type v);

///////////////////////////////////////////////////////////////////////////
// Functions for vector division: w = u / v.
///////////////////////////////////////////////////////////////////////////

extern 
void 
vec_div_large(length_type ulen, const digit_type *u, 
              length_type vlen, const digit_type *v, digit_type *w);

extern 
void 
vec_div_small(length_type ulen, const digit_type *u, 
              digit_type v, digit_type *w);

///////////////////////////////////////////////////////////////////////////
// Functions for vector remainder: w = u % v or u %= v.
///////////////////////////////////////////////////////////////////////////

extern 
void 
vec_rem_large(length_type ulen, const digit_type *u, 
              length_type vlen, const digit_type *v, digit_type *w);

extern 
digit_type 
vec_rem_small(length_type ulen, const digit_type *u, digit_type v);

extern 
digit_type 
vec_rem_on_small(length_type ulen, digit_type *u, digit_type v);

///////////////////////////////////////////////////////////////////////////
// Functions to convert between vectors of char and digit_type.
///////////////////////////////////////////////////////////////////////////

extern 
length_type 
vec_to_char(length_type ulen, const digit_type *u, 
            length_type vlen, uchar *v);

extern 
void 
vec_from_char(length_type ulen, const uchar *u,
              length_type vlen, digit_type *v);

///////////////////////////////////////////////////////////////////////////
// Functions to shift left or right, or to create a mirror image of vectors.
///////////////////////////////////////////////////////////////////////////

extern 
void 
vec_shift_left(length_type ulen, digit_type *u, int nsl);

extern 
void 
vec_shift_right(length_type vlen, digit_type *u, int nsr, digit_type fill = 0);

extern
void 
vec_reverse(length_type unb, length_type und, digit_type *ud, 
            length_type l, length_type r = 0);

///////////////////////////////////////////////////////////////////////////
// Various utility functions. 
///////////////////////////////////////////////////////////////////////////

// Return the low half part of d.
inline 
digit_type 
low_half(digit_type d) 
{
  return (d & HALF_DIGIT_MASK);
}

// Return the high half part of d. The high part of the digit may have
// more bits than BITS_PER_HALF_DIGIT due to, e.g., overflow in the
// multiplication. Hence, in other functions that use high_half(),
// make sure that the result contains BITS_PER_HALF_DIGIT if
// necessary. This is done by high_half_masked().
inline 
digit_type 
high_half(digit_type d) 
{
  return (d >> BITS_PER_HALF_DIGIT);
}

inline
digit_type
high_half_masked(digit_type d)
{
  return (high_half(d) & HALF_DIGIT_MASK);
}

// Concatenate the high part h and low part l. Assumes that h and l
// are less than or equal to HALF_DIGIT_MASK;
inline 
digit_type 
concat(digit_type h, digit_type l) 
{
  return ((h << BITS_PER_HALF_DIGIT) | l);
}

// Create a number with n 1's.
inline
digit_type
one_and_ones(length_type n)
{
  return (((digit_type) 1 << n) - 1);
}

// Create a number with one 1 and n 0's.
inline
digit_type
one_and_zeros(length_type n)
{
  return ((digit_type) 1 << n);
}

///////////////////////////////////////////////////////////////////////////

// Find the digit that bit i is in.
inline
length_type 
digit_ord(length_type i)
{
  return (i / BITS_PER_DIGIT);
}

// Find the bit in digit_ord(i) that bit i corressponds to.
inline
length_type 
bit_ord(length_type i)
{
  return (i % BITS_PER_DIGIT);
}

///////////////////////////////////////////////////////////////////////////
// MAX, MIN, and ABS.
///////////////////////////////////////////////////////////////////////////

template< class Type >
inline 
Type
MAXT(Type x, Type y)
{
  return (x > y ? x : y);
}

template< class Type >
inline 
Type
MINT(Type x, Type y)
{
  return (x < y ? x : y);
}

template< class Type >
inline 
Type
ABST(Type x)
{
  return (x > 0 ? x : -x);
}

///////////////////////////////////////////////////////////////////////////
// Functions to compare, zero, complement vector(s).
///////////////////////////////////////////////////////////////////////////

// Compare u and v and return r
//  r = 0 if u == v
//  r < 0 if u < v
//  r > 0 if u > v
// - Assume that all the leading zero digits are already skipped.
// - ulen and/or vlen can be zero.
// - Every digit is less than or equal to DIGIT_MASK;
inline 
length_type
vec_cmp(length_type ulen, const digit_type *u, 
        length_type vlen, const digit_type *v)
{

#ifdef DEBUG_SYSTEMC
  // assert((ulen <= 0) || (u != NULL));
  // assert((vlen <= 0) || (v != NULL));

  // ulen and vlen can be equal to 0 because vec_cmp can be called
  // after vec_skip_leading_zeros.
  assert((ulen >= 0) && (u != NULL));
  assert((vlen >= 0) && (v != NULL));
  // If ulen > 0, then the leading digit of u must be non-zero.
  assert((ulen <= 0) || (u[ulen - 1] != 0));
  assert((vlen <= 0) || (v[vlen - 1] != 0));
#endif

  if (ulen != vlen)
    return (ulen - vlen);

  // ulen == vlen >= 1
  while ((--ulen >= 0) && (u[ulen] == v[ulen]))
    ;

  if (ulen < 0)
    return 0;

#ifdef DEBUG_SYSTEMC
  // Test to see if the result is wrong due to the presence of
  // overflow bits.
  assert((u[ulen] & DIGIT_MASK) != (v[ulen] & DIGIT_MASK));
#endif

  return (length_type) (u[ulen] - v[ulen]);

}

// Find the index of the first non-zero digit.
// - ulen (before) = the number of digits in u.
// - the returned value = the index of the first non-zero digit. 
// A negative value of -1 indicates that every digit in u is zero.
inline
length_type
vec_find_first_nonzero(length_type ulen, const digit_type *u)
{

#ifdef DEBUG_SYSTEMC
  // assert((ulen <= 0) || (u != NULL));
  assert((ulen > 0) && (u != NULL));
#endif

  while ((--ulen >= 0) && (! u[ulen]))
    ;

  return ulen;
  
}

// Skip all the leading zero digits.  
// - ulen (before) = the number of digits in u.
// - the returned value = the number of non-zero digits in u.
// - the returned value is non-negative.
inline 
length_type
vec_skip_leading_zeros(length_type ulen, const digit_type *u)
{

#ifdef DEBUG_SYSTEMC
  // assert((ulen <= 0) || (u != NULL));
  assert((ulen > 0) && (u != NULL));
#endif

  return (1 + vec_find_first_nonzero(ulen, u));

}

// Compare u and v and return r
//  r = 0 if u == v
//  r < 0 if u < v
//  r > 0 if u > v
inline 
length_type 
vec_skip_and_cmp(length_type ulen, const digit_type *u, 
                 length_type vlen, const digit_type *v)
{

#ifdef DEBUG_SYSTEMC
  assert((ulen > 0) && (u != NULL));
  assert((vlen > 0) && (v != NULL));
#endif

  ulen = vec_skip_leading_zeros(ulen, u);
  vlen = vec_skip_leading_zeros(vlen, v);
  // ulen and/or vlen can be equal to zero here.
  return vec_cmp(ulen, u, vlen, v);

}

// Set u[i] = 0 where i = from ... (ulen - 1).
inline
void
vec_zero(length_type from, length_type ulen, digit_type *u)
{

#ifdef DEBUG_SYSTEMC
  assert((ulen > 0) && (u != NULL));
#endif

  for(int i = from; i < ulen; i++)
    u[i] = 0;

}

// Set u[i] = 0 where i = 0 .. (ulen - 1).
inline
void
vec_zero(length_type ulen, digit_type *u)
{
  vec_zero(0, ulen, u);
}

// Copy n digits from v to u.
inline
void
vec_copy(length_type n, digit_type *u, const digit_type *v)
{

#ifdef DEBUG_SYSTEMC
  assert((n > 0) && (u != NULL) && (v != NULL));
#endif

  for (register length_type i = 0; i < n; ++i)
    u[i] = v[i];
}

// Copy v to u, where ulen >= vlen, and zero the rest of the digits in u.
inline
void
vec_copy_and_zero(length_type ulen, digit_type *u, 
                  length_type vlen, const digit_type *v)
{

#ifdef DEBUG_SYSTEMC
  assert((ulen > 0) && (u != NULL));
  assert((vlen > 0) && (v != NULL));
  assert(ulen >= vlen);
#endif

  vec_copy(vlen, u, v);
  vec_zero(vlen, ulen, u);

}

// 2's-complement the digits in u.
inline
void
vec_complement(length_type ulen, digit_type *u)
{

#ifdef DEBUG_SYSTEMC
  assert((ulen > 0) && (u != NULL));
#endif

  register digit_type carry = 1;

  for (register length_type i = 0; i < ulen; ++i) {
    carry += (~u[i] & DIGIT_MASK);
    u[i] = carry & DIGIT_MASK;
    carry >>= BITS_PER_DIGIT;
  }
  
}

///////////////////////////////////////////////////////////////////////////
// Functions to handle built-in types or signs.
///////////////////////////////////////////////////////////////////////////

// u = v
// - v is an unsigned long or uint64, and positive integer.
template< class Type >
inline
void
from_uint(length_type ulen, digit_type *u, Type v)
{

#ifdef DEBUG_SYSTEMC
  // assert((ulen <= 0) || (u != NULL));
  assert((ulen > 0) && (u != NULL));
  assert(v >= 0);
#endif

  register length_type i = 0;

  while (v && (i < ulen)) {
#ifndef WIN32
    u[i++] = v & DIGIT_MASK;
#else
    u[i++] = ((digit_type) v) & DIGIT_MASK;
#endif
    v >>= BITS_PER_DIGIT;
  }

  vec_zero(i, ulen, u);

}
#if defined(__BCPLUSPLUS__)
#pragma warn .8027
#endif

#if defined(__BCPLUSPLUS__)
#pragma warn -ngu
#endif

// Get u's sign and return its absolute value.
// u can be long, unsigned long, int64, or uint64.
template< class Type >
inline
small_type
get_sign(Type &u) 
{
  if (u > 0)
    return SC_POS;

  else if (u == 0)
    return SC_ZERO;

  else {
    u = -u;
    return SC_NEG;
  }
}
#if defined(__BCPLUSPLUS__)
#pragma warn +ngu
#endif

// Return us * vs:
// - Return SC_ZERO if either sign is SC_ZERO.
// - Return SC_POS if us == vs
// - Return SC_NEG if us != vs.
inline
small_type
mul_signs(small_type us, small_type vs)
{
  if ((us == SC_ZERO) || (vs == SC_ZERO))
    return SC_ZERO;

  if (us == vs)
    return SC_POS;

  return SC_NEG;
}

///////////////////////////////////////////////////////////////////////////
// Functions to test for errors and print out error messages.
///////////////////////////////////////////////////////////////////////////

#ifdef MAX_NBITS

inline
void
test_bound(length_type nb)
{
  if (nb > MAX_NBITS) {
    printf("SystemC error: Cannot store all the bits in MAX_NBITS bits.\n");
    abort();
  }
}

#endif

template< class Type >
inline
void
div_by_zero(Type s)
{
  if (s == 0) {
    printf("SystemC error: 0 / 0 or num / 0.\n");
    abort();
  }
}

///////////////////////////////////////////////////////////////////////////
// Functions to check if a given vector is zero or make one.
///////////////////////////////////////////////////////////////////////////

// If u[i] is zero for every i = 0,..., ulen - 1, return SC_ZERO,
// else return s.
inline
small_type 
check_for_zero(small_type s, length_type ulen, const digit_type *u)
{

#ifdef DEBUG_SYSTEMC
  // assert(ulen >= 0);
  assert((ulen > 0) && (u != NULL));
#endif

  if (vec_find_first_nonzero(ulen, u) < 0)
    return SC_ZERO;  

  return s;

}

// If u[i] is zero for every i = 0,..., ulen - 1, return true,
// else return false.
inline
bool
check_for_zero(length_type ulen, const digit_type *u)
{

#ifdef DEBUG_SYSTEMC
  // assert(ulen >= 0);
  assert((ulen > 0) && (u != NULL));
#endif

  if (vec_find_first_nonzero(ulen, u) < 0)
    return true;

  return false;

}

inline
small_type
make_zero(length_type nd, digit_type *d)
{
  vec_zero(nd, d);
  return SC_ZERO;
}

///////////////////////////////////////////////////////////////////////////
// Functions for both signed and unsigned numbers to convert sign-magnitude
// (SM) and 2's complement (2C) representations.
// added = 1 => for signed.
// added = 0 => for unsigned.
// IF_SC_SIGNED can be used as 'added'.
///////////////////////////////////////////////////////////////////////////

// Trim the extra leading bits of a signed or unsigned number.
inline
void
trim(small_type added, length_type nb, length_type nd, digit_type *d)
{
#ifdef DEBUG_SYSTEMC
  assert((nb > 0) && (nd > 0) && (d != NULL));
#endif

  d[nd - 1] &= one_and_ones(bit_ord(nb - 1) + added);    
}

// Convert an (un)signed number from sign-magnitude representation to
// 2's complement representation and trim the extra bits.
inline
void
convert_SM_to_2C_trimmed(small_type added, 
                         small_type s, length_type nb, length_type nd, digit_type *d)
{
  if (s == SC_NEG) {
    vec_complement(nd, d);
    trim(added, nb, nd, d);
  }
}

// Convert an (un)signed number from sign-magnitude representation to
// 2's complement representation but do not trim the extra bits.
inline
void
convert_SM_to_2C(small_type s, length_type nd, digit_type *d)
{
  if (s == SC_NEG)
    vec_complement(nd, d);
}

///////////////////////////////////////////////////////////////////////////
// Functions to convert between sign-magnitude (SM) and 2's complement
// (2C) representations of signed numbers.
///////////////////////////////////////////////////////////////////////////

// Trim the extra leading bits off a signed number.
inline
void
trim_signed(length_type nb, length_type nd, digit_type *d)
{
#ifdef DEBUG_SYSTEMC
  assert((nb > 0) && (nd > 0) && (d != NULL));
#endif

  d[nd - 1] &= one_and_ones(bit_ord(nb - 1) + 1);
}

// Convert a signed number from 2's complement representation to
// sign-magnitude representation, and return its sign. nd is d's
// actual size, without zeros eliminated.
inline
small_type
convert_signed_2C_to_SM(length_type nb, length_type nd, digit_type *d)
{

#ifdef DEBUG_SYSTEMC
  assert((nb > 0) && (nd > 0) && (d != NULL));
#endif

  small_type s;

  length_type xnb = bit_ord(nb - 1) + 1;

  // Test the sign bit.  
  if (d[nd - 1] & one_and_zeros(xnb - 1)) {
    s = SC_NEG;
    vec_complement(nd, d);
  }
  else
    s = SC_POS;

  // Trim the last digit.
  d[nd - 1] &= one_and_ones(xnb);

  // Check if the new number is zero.
  if (s == SC_POS)
    return check_for_zero(s, nd, d);

  return s;

}

// Convert a signed number from sign-magnitude representation to 2's
// complement representation, get its sign, convert back to
// sign-magnitude representation, and return its sign. nd is d's
// actual size, without zeros eliminated.
inline
small_type 
convert_signed_SM_to_2C_to_SM(small_type s, length_type nb, length_type nd, digit_type *d)
{
  convert_SM_to_2C(s, nd, d);
  return convert_signed_2C_to_SM(nb, nd, d);
}

// Convert a signed number from sign-magnitude representation to 2's
// complement representation and trim the extra bits.
inline
void
convert_signed_SM_to_2C_trimmed(small_type s, length_type nb, length_type nd, digit_type *d)
{
  convert_SM_to_2C_trimmed(1, s, nb, nd, d);
}

// Convert a signed number from sign-magnitude representation to 2's
// complement representation but do not trim the extra bits.
inline
void
convert_signed_SM_to_2C(small_type s, length_type nd, digit_type *d)
{
  convert_SM_to_2C(s, nd, d);
}

///////////////////////////////////////////////////////////////////////////
// Functions to convert between sign-magnitude (SM) and 2's complement
// (2C) representations of unsigned numbers.
///////////////////////////////////////////////////////////////////////////

// Trim the extra leading bits off an unsigned number.
inline
void
trim_unsigned(length_type nb, length_type nd, digit_type *d)
{
#ifdef DEBUG_SYSTEMC
  assert((nb > 0) && (nd > 0) && (d != NULL));
#endif

  d[nd - 1] &= one_and_ones(bit_ord(nb - 1));    
}

// Convert an unsigned number from 2's complement representation to
// sign-magnitude representation, and return its sign. nd is d's
// actual size, without zeros eliminated.
inline
small_type
convert_unsigned_2C_to_SM(length_type nb, length_type nd, digit_type *d)
{
  trim_unsigned(nb, nd, d);
  return check_for_zero(SC_POS, nd, d);
}

// Convert an unsigned number from sign-magnitude representation to
// 2's complement representation, get its sign, convert back to
// sign-magnitude representation, and return its sign. nd is d's
// actual size, without zeros eliminated.
inline
small_type
convert_unsigned_SM_to_2C_to_SM(small_type s, length_type nb, length_type nd, digit_type *d)
{
  convert_SM_to_2C(s, nd, d);
  return convert_unsigned_2C_to_SM(nb, nd, d);
}

// Convert an unsigned number from sign-magnitude representation to
// 2's complement representation and trim the extra bits.
inline
void
convert_unsigned_SM_to_2C_trimmed(small_type s, length_type nb, length_type nd, digit_type *d)
{
  convert_SM_to_2C_trimmed(0, s, nb, nd, d);
}

// Convert an unsigned number from sign-magnitude representation to
// 2's complement representation but do not trim the extra bits.
inline
void
convert_unsigned_SM_to_2C(small_type s, length_type nd, digit_type *d)
{
  convert_SM_to_2C(s, nd, d);
}

///////////////////////////////////////////////////////////////////////////
// Functions to copy one (un)signed number to another.
///////////////////////////////////////////////////////////////////////////

// Copy v to u.
inline
void
copy_digits_signed(small_type &us, 
                   length_type unb, length_type und, digit_type *ud,
                   length_type vnb, length_type vnd, const digit_type *vd)
{

  if (und <= vnd) {

    vec_copy(und, ud, vd);

    if (unb <= vnb)
      us = convert_signed_SM_to_2C_to_SM(us, unb, und, ud);

  }
  else // und > vnd
    vec_copy_and_zero(und, ud, vnd, vd);

}

// Copy v to u.
inline
void
copy_digits_unsigned(small_type &us, 
                     length_type unb, length_type und, digit_type *ud,
                     length_type vnb, length_type vnd, const digit_type *vd)
{

  if (und <= vnd)
    vec_copy(und, ud, vd);

  else // und > vnd
    vec_copy_and_zero(und, ud, vnd, vd);

  us = convert_unsigned_SM_to_2C_to_SM(us, unb, und, ud);

}

///////////////////////////////////////////////////////////////////////////
// Faster set(i, v), without bound checking.
///////////////////////////////////////////////////////////////////////////

// A version of set(i, v) without bound checking.
inline
void
safe_set(length_type i, bit v, digit_type *d)
{

#ifdef DEBUG_SYSTEMC
  assert((i >= 0) && (d != NULL));
#endif

  length_type bit_num = bit_ord(i);
  length_type digit_num = digit_ord(i);    
  
  if (v)
    d[digit_num] |= one_and_zeros(bit_num);      
  else
    d[digit_num] &= ~(one_and_zeros(bit_num));
  
}

///////////////////////////////////////////////////////////////////////////
// Function to check if a double number is bad (NaN or infinite).
///////////////////////////////////////////////////////////////////////////

inline
void
is_bad_double(double v)
{
// Windows throws exception.
#if !defined(WIN32) && !defined(i386)
  if (! finite(v)) {
    printf("SystemC error: Double argument is not finite - NaN or Inf.\n");
    abort();
  }
#endif
}

///////////////////////////////////////////////////////////////////////////

#endif
