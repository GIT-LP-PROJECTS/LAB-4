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

    sc_nbexterns.cpp -- External functions for both sc_signed and
    sc_unsigned classes. These functions work on two parameters u and
    v, and copy the result to the first parameter u. This is also the
    reason that they are suffixed with _on_help.
 
    Original Author: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include "sc_nbexterns.h"

/////////////////////////////////////////////////////////////////////////////
// SECTION: External functions for PLUS operators.
/////////////////////////////////////////////////////////////////////////////


// Handles the cases 3 and 4 and returns the result in u.
void
add_on_help(small_type &us, length_type unb, length_type und,
            digit_type *ud, 
            small_type vs, length_type vnb, length_type vnd,
            const digit_type *vd)
{

  vnd = vec_skip_leading_zeros(vnd, vd);

  if (us == vs) {  // case 3

    if (und >= vnd)
      vec_add_on(und, ud, vnd, vd);
    else
      vec_add_on2(und, ud, vnd, vd);

  }
  else {  // case 4

    // vec_cmp expects that und is the number of non-zero digits in ud.
    length_type new_und = vec_skip_leading_zeros(und, ud); 
    length_type cmp_res = vec_cmp(new_und, ud, vnd, vd);

    if (cmp_res == 0)  { // u == v
      us = SC_ZERO;
      vec_zero(und, ud);
      return;
    }

    if (cmp_res > 0) // u > v
      vec_sub_on(und, ud, vnd, vd);

    else { // u < v
      us = -us;
      vec_sub_on2(und, ud, vnd, vd);
    }

  }
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


/* 

mul_on_help_signed and mul_on_help_unsigned have the same body except
that CONVERT_SM_to_2C_to_SM and COPY_DIGITS are defined for signed and
unsigned, respectively.  This comment also applies to the
signed/unsigned versions of div_on_help and mod_on_help. It is
possible to take COPY_DIGITS out of these functions and create a
single version of each of these helper functions; however, this will
impose an onverhead on performance. In the versions below, any change
in the signed version of a helper function must be carried to a
corresponding change in the unsigned verion of the same function or
vice versa.

*/

/////////////////////////////////////////////////////////////////////////////
// SECTION: External functions of MULTIPLICATION operators.
/////////////////////////////////////////////////////////////////////////////

/*

// Handles the case 4 and returns the result in u.
void
mul_on_help(small_type &us, 
            length_type unb, length_type und, 
            digit_type *ud, 
            length_type vnb, length_type vnd,
            const digit_type *vd)
{

  length_type old_und = und;

  und = vec_skip_leading_zeros(und, ud);
  vnd = vec_skip_leading_zeros(vnd, vd);

  digit_type ud0 = (*ud);
  digit_type vd0 = (*vd);

  if ((vnd == 1) && (vd0 == 1)) {
    us = CONVERT_SM_to_2C_to_SM(us, unb, old_und, ud);
    return;
  }
    
  if ((und == 1) && (ud0 == 1)) {
    COPY_DIGITS(us, unb, old_und, ud, vnb, vnd, vd);
    return;
  }

  if ((und == 1) && (vnd == 1) && 
      (ud0 < HALF_DIGIT_RADIX) && (vd0 < HALF_DIGIT_RADIX)) {

    digit_type d = ud0 * vd0;
    COPY_DIGITS(us, unb, old_und, ud, unb + vnb, 1, &d);
    return;

  }

  length_type nd = und + vnd;
  
#ifdef MAX_NBITS
  digit_type d[MAX_NDIGITS];
#else
  digit_type *d = new digit_type[nd];
#endif
  
  vec_zero(nd, d);

  if ((und == 1) && (ud0 < HALF_DIGIT_RADIX))
    vec_mul_small(vnd, vd, ud0, d);
  
  else if ((vnd == 1) && (vd0 < HALF_DIGIT_RADIX))
    vec_mul_small(und, ud, vd0, d);
  
  else if (vnd < und)
    vec_mul(und, ud, vnd, vd, d);
  
  else
    vec_mul(vnd, vd, und, ud, d);
  
  COPY_DIGITS(us, unb, old_und, ud, unb + vnb, nd, d);
  
#ifndef MAX_NBITS
  delete [] d;
#endif

}

*/


void
mul_on_help_signed(small_type &us, 
                   length_type unb, length_type und, 
                   digit_type *ud, 
                   length_type vnb, length_type vnd,
                   const digit_type *vd)
{
#define CONVERT_SM_to_2C_to_SM convert_signed_SM_to_2C_to_SM
#define COPY_DIGITS copy_digits_signed

  {  // Body of mul_on_help

    length_type old_und = und;

    und = vec_skip_leading_zeros(und, ud);
    vnd = vec_skip_leading_zeros(vnd, vd);
    
    digit_type ud0 = (*ud);
    digit_type vd0 = (*vd);
    
    if ((vnd == 1) && (vd0 == 1)) {
      us = CONVERT_SM_to_2C_to_SM(us, unb, old_und, ud);
      return;
    }
    
    if ((und == 1) && (ud0 == 1)) {
      COPY_DIGITS(us, unb, old_und, ud, vnb, vnd, vd);
      return;
    }
    
    if ((und == 1) && (vnd == 1) && 
        (ud0 < HALF_DIGIT_RADIX) && (vd0 < HALF_DIGIT_RADIX)) {
      
      digit_type d = ud0 * vd0;
      COPY_DIGITS(us, unb, old_und, ud, unb + vnb, 1, &d);
      return;
      
    }
    
    length_type nd = und + vnd;
    
#ifdef MAX_NBITS
    digit_type d[MAX_NDIGITS];
#else
    digit_type *d = new digit_type[nd];
#endif
  
    vec_zero(nd, d);
    
    if ((und == 1) && (ud0 < HALF_DIGIT_RADIX))
      vec_mul_small(vnd, vd, ud0, d);
    
    else if ((vnd == 1) && (vd0 < HALF_DIGIT_RADIX))
      vec_mul_small(und, ud, vd0, d);
    
    else if (vnd < und)
      vec_mul(und, ud, vnd, vd, d);
    
    else
      vec_mul(vnd, vd, und, ud, d);
    
    COPY_DIGITS(us, unb, old_und, ud, unb + vnb, nd, d);
    
#ifndef MAX_NBITS
    delete [] d;
#endif
    
  }

#undef COPY_DIGITS
#undef CONVERT_SM_to_2C_to_SM

}


void
mul_on_help_unsigned(small_type &us, 
                     length_type unb, length_type und, 
                     digit_type *ud, 
                     length_type vnb, length_type vnd,
                     const digit_type *vd)
{
#define CONVERT_SM_to_2C_to_SM convert_unsigned_SM_to_2C_to_SM
#define COPY_DIGITS copy_digits_unsigned

  {  // Body of mul_on_help

    length_type old_und = und;

    und = vec_skip_leading_zeros(und, ud);
    vnd = vec_skip_leading_zeros(vnd, vd);
    
    digit_type ud0 = (*ud);
    digit_type vd0 = (*vd);
    
    if ((vnd == 1) && (vd0 == 1)) {
      us = CONVERT_SM_to_2C_to_SM(us, unb, old_und, ud);
      return;
    }
    
    if ((und == 1) && (ud0 == 1)) {
      COPY_DIGITS(us, unb, old_und, ud, vnb, vnd, vd);
      return;
    }
    
    if ((und == 1) && (vnd == 1) && 
        (ud0 < HALF_DIGIT_RADIX) && (vd0 < HALF_DIGIT_RADIX)) {
      
      digit_type d = ud0 * vd0;
      COPY_DIGITS(us, unb, old_und, ud, unb + vnb, 1, &d);
      return;
      
    }
    
    length_type nd = und + vnd;
    
#ifdef MAX_NBITS
    digit_type d[MAX_NDIGITS];
#else
    digit_type *d = new digit_type[nd];
#endif
  
    vec_zero(nd, d);
    
    if ((und == 1) && (ud0 < HALF_DIGIT_RADIX))
      vec_mul_small(vnd, vd, ud0, d);
    
    else if ((vnd == 1) && (vd0 < HALF_DIGIT_RADIX))
      vec_mul_small(und, ud, vd0, d);
    
    else if (vnd < und)
      vec_mul(und, ud, vnd, vd, d);
    
    else
      vec_mul(vnd, vd, und, ud, d);
    
    COPY_DIGITS(us, unb, old_und, ud, unb + vnb, nd, d);
    
#ifndef MAX_NBITS
    delete [] d;
#endif
    
  }

#undef COPY_DIGITS
#undef CONVERT_SM_to_2C_to_SM

}


/////////////////////////////////////////////////////////////////////////////
// SECTION: External functions for DIVISION operators.
/////////////////////////////////////////////////////////////////////////////


/*

// Handles the cases 3-4 and returns the result in u.
void
div_on_help(small_type &us, 
            length_type unb, length_type und, 
            digit_type *ud, 
            length_type vnb, length_type vnd,
            const digit_type *vd)
{

  length_type old_und = und;

  und = vec_skip_leading_zeros(und, ud);
  vnd = vec_skip_leading_zeros(vnd, vd);

  length_type cmp_res = vec_cmp(und, ud, vnd, vd);

  if (cmp_res < 0) { // u < v => u / v = 0 - case 4
    us = SC_ZERO;
    vec_zero(old_und, ud);
    return;
  }

  digit_type vd0 = (*vd);

  if ((cmp_res > 0) && (vnd == 1) && (vd0 == 1))  {
    us = CONVERT_SM_to_2C_to_SM(us, unb, old_und, ud);
    return;
  }

  // One extra digit for d is allocated to simplify vec_div_*().
  length_type nd = MAXT(und, vnd) + 1;

#ifdef MAX_NBITS
  digit_type d[MAX_NDIGITS + 1];
#else
  digit_type *d = new digit_type[nd];
#endif

  vec_zero(nd, d);

  // u = v => u / v = 1 - case 3
  if (cmp_res == 0)
    d[0] = 1;

  else if ((vnd == 1) && (und == 1))
    d[0] = (*ud) / vd0;
 
  else if ((vnd == 1) && (vd0 < HALF_DIGIT_RADIX))
    vec_div_small(und, ud, vd0, d);

  else
    vec_div_large(und, ud, vnd, vd, d);

  COPY_DIGITS(us, unb, old_und, ud, MAXT(unb, vnb), nd - 1, d);

#ifndef MAX_NBITS
  delete [] d;
#endif

}

*/


void
div_on_help_signed(small_type &us, 
                   length_type unb, length_type und, 
                   digit_type *ud, 
                   length_type vnb, length_type vnd,
                   const digit_type *vd)
{
#define CONVERT_SM_to_2C_to_SM convert_signed_SM_to_2C_to_SM
#define COPY_DIGITS copy_digits_signed

  {  // Body of div_on_help

    length_type old_und = und;
    
    und = vec_skip_leading_zeros(und, ud);
    vnd = vec_skip_leading_zeros(vnd, vd);
    
    length_type cmp_res = vec_cmp(und, ud, vnd, vd);
    
    if (cmp_res < 0) { // u < v => u / v = 0 - case 4
      us = SC_ZERO;
      vec_zero(old_und, ud);
      return;
    }
    
    digit_type vd0 = (*vd);
    
    if ((cmp_res > 0) && (vnd == 1) && (vd0 == 1))  {
      us = CONVERT_SM_to_2C_to_SM(us, unb, old_und, ud);
      return;
    }
    
    // One extra digit for d is allocated to simplify vec_div_*().
    length_type nd = MAXT(und, vnd) + 1;
    
#ifdef MAX_NBITS
    digit_type d[MAX_NDIGITS + 1];
#else
    digit_type *d = new digit_type[nd];
#endif
    
    vec_zero(nd, d);
    
    // u = v => u / v = 1 - case 3
    if (cmp_res == 0)
      d[0] = 1;
    
    else if ((vnd == 1) && (und == 1))
      d[0] = (*ud) / vd0;
    
    else if ((vnd == 1) && (vd0 < HALF_DIGIT_RADIX))
      vec_div_small(und, ud, vd0, d);
    
    else
      vec_div_large(und, ud, vnd, vd, d);
    
    COPY_DIGITS(us, unb, old_und, ud, MAXT(unb, vnb), nd - 1, d);
    
#ifndef MAX_NBITS
    delete [] d;
#endif
    
  }
  
#undef COPY_DIGITS
#undef CONVERT_SM_to_2C_to_SM

}


void
div_on_help_unsigned(small_type &us, 
                     length_type unb, length_type und, 
                     digit_type *ud, 
                     length_type vnb, length_type vnd,
                     const digit_type *vd)
{
#define CONVERT_SM_to_2C_to_SM convert_unsigned_SM_to_2C_to_SM
#define COPY_DIGITS copy_digits_unsigned

  {  // Body of div_on_help

    length_type old_und = und;
    
    und = vec_skip_leading_zeros(und, ud);
    vnd = vec_skip_leading_zeros(vnd, vd);
    
    length_type cmp_res = vec_cmp(und, ud, vnd, vd);
    
    if (cmp_res < 0) { // u < v => u / v = 0 - case 4
      us = SC_ZERO;
      vec_zero(old_und, ud);
      return;
    }
    
    digit_type vd0 = (*vd);
    
    if ((cmp_res > 0) && (vnd == 1) && (vd0 == 1))  {
      us = CONVERT_SM_to_2C_to_SM(us, unb, old_und, ud);
      return;
    }
    
    // One extra digit for d is allocated to simplify vec_div_*().
    length_type nd = MAXT(und, vnd) + 1;
    
#ifdef MAX_NBITS
    digit_type d[MAX_NDIGITS + 1];
#else
    digit_type *d = new digit_type[nd];
#endif
    
    vec_zero(nd, d);
    
    // u = v => u / v = 1 - case 3
    if (cmp_res == 0)
      d[0] = 1;
    
    else if ((vnd == 1) && (und == 1))
      d[0] = (*ud) / vd0;
    
    else if ((vnd == 1) && (vd0 < HALF_DIGIT_RADIX))
      vec_div_small(und, ud, vd0, d);
    
    else
      vec_div_large(und, ud, vnd, vd, d);
    
    COPY_DIGITS(us, unb, old_und, ud, MAXT(unb, vnb), nd - 1, d);
    
#ifndef MAX_NBITS
    delete [] d;
#endif
    
  }
  
#undef COPY_DIGITS
#undef CONVERT_SM_to_2C_to_SM

}


/////////////////////////////////////////////////////////////////////////////
// SECTION: External functions for MOD operators.
/////////////////////////////////////////////////////////////////////////////


/*

// Handles the cases 3-4 and returns the result.
void
mod_on_help(small_type &us, 
            length_type unb, length_type und, 
            digit_type *ud, 
            length_type vnb, length_type vnd,
            const digit_type *vd)
{

  length_type old_und = und;

  und = vec_skip_leading_zeros(und, ud);
  vnd = vec_skip_leading_zeros(vnd, vd);

  length_type cmp_res = vec_cmp(und, ud, vnd, vd);

  // u < v => u % v = u - case 4
  if (cmp_res < 0) 
    return;

  // u = v => u % v = 0 - case 3
  if (cmp_res == 0) { 
    us = SC_ZERO;
    vec_zero(old_und, ud);
    return;
  }

  // else if u > v - case 5

  digit_type vd0 = (*vd);

  if ((vnd == 1) && (vd0 == 1)) {
    us = SC_ZERO;
    vec_zero(old_und, ud);
    return;
  }

  // One extra digit for d is allocated to simplify vec_div_*().
  length_type nd = MAXT(und, vnd) + 1;

#ifdef MAX_NBITS
  digit_type d[MAX_NDIGITS + 1];
#else
  digit_type *d = new digit_type[nd];
#endif

  vec_zero(nd, d);

  if ((vnd == 1) && (und == 1))
    d[0] = (*ud) % vd0;

  if ((vnd == 1) && (vd0 < HALF_DIGIT_RADIX))
    d[0] = vec_rem_small(und, ud, vd0);

  else
    vec_rem_large(und, ud, vnd, vd, d);

  us = check_for_zero(us, nd - 1, d);

  if (us == SC_ZERO)
    vec_zero(old_und, ud);
  else
    COPY_DIGITS(us, unb, old_und, ud, MINT(unb, vnd), nd - 1, d);

#ifndef MAX_NBITS
  delete [] d;
#endif
  
}

*/


void
mod_on_help_signed(small_type &us, 
                   length_type unb, length_type und, 
                   digit_type *ud, 
                   length_type vnb, length_type vnd,
                   const digit_type *vd)
{

#define COPY_DIGITS copy_digits_signed

  { // Body of mod_on_help

    length_type old_und = und;
    
    und = vec_skip_leading_zeros(und, ud);
    vnd = vec_skip_leading_zeros(vnd, vd);
    
    length_type cmp_res = vec_cmp(und, ud, vnd, vd);
    
    // u < v => u % v = u - case 4
    if (cmp_res < 0) 
      return;
    
    // u = v => u % v = 0 - case 3
    if (cmp_res == 0) { 
      us = SC_ZERO;
      vec_zero(old_und, ud);
      return;
    }
    
    // else if u > v - case 5
    
    digit_type vd0 = (*vd);
    
    if ((vnd == 1) && (vd0 == 1)) {
      us = SC_ZERO;
      vec_zero(old_und, ud);
      return;
    }
    
    // One extra digit for d is allocated to simplify vec_div_*().
    length_type nd = MAXT(und, vnd) + 1;
    
#ifdef MAX_NBITS
    digit_type d[MAX_NDIGITS + 1];
#else
    digit_type *d = new digit_type[nd];
#endif
    
    vec_zero(nd, d);
    
    if ((vnd == 1) && (und == 1))
      d[0] = (*ud) % vd0;
    
    if ((vnd == 1) && (vd0 < HALF_DIGIT_RADIX))
      d[0] = vec_rem_small(und, ud, vd0);
    
    else
      vec_rem_large(und, ud, vnd, vd, d);
    
    us = check_for_zero(us, nd - 1, d);
    
    if (us == SC_ZERO)
      vec_zero(old_und, ud);
    else
      COPY_DIGITS(us, unb, old_und, ud, MINT(unb, vnd), nd - 1, d);
    
#ifndef MAX_NBITS
    delete [] d;
#endif
    
  }
  
#undef COPY_DIGITS
  
}


void
mod_on_help_unsigned(small_type &us, 
                     length_type unb, length_type und, 
                     digit_type *ud, 
                     length_type vnb, length_type vnd,
                     const digit_type *vd)
{

#define COPY_DIGITS copy_digits_unsigned

  { // Body of mod_on_help

    length_type old_und = und;
    
    und = vec_skip_leading_zeros(und, ud);
    vnd = vec_skip_leading_zeros(vnd, vd);
    
    length_type cmp_res = vec_cmp(und, ud, vnd, vd);
    
    // u < v => u % v = u - case 4
    if (cmp_res < 0) 
      return;
    
    // u = v => u % v = 0 - case 3
    if (cmp_res == 0) { 
      us = SC_ZERO;
      vec_zero(old_und, ud);
      return;
    }
    
    // else if u > v - case 5
    
    digit_type vd0 = (*vd);
    
    if ((vnd == 1) && (vd0 == 1)) {
      us = SC_ZERO;
      vec_zero(old_und, ud);
      return;
    }
    
    // One extra digit for d is allocated to simplify vec_div_*().
    length_type nd = MAXT(und, vnd) + 1;
    
#ifdef MAX_NBITS
    digit_type d[MAX_NDIGITS + 1];
#else
    digit_type *d = new digit_type[nd];
#endif
    
    vec_zero(nd, d);
    
    if ((vnd == 1) && (und == 1))
      d[0] = (*ud) % vd0;
    
    if ((vnd == 1) && (vd0 < HALF_DIGIT_RADIX))
      d[0] = vec_rem_small(und, ud, vd0);
    
    else
      vec_rem_large(und, ud, vnd, vd, d);
    
    us = check_for_zero(us, nd - 1, d);
    
    if (us == SC_ZERO)
      vec_zero(old_und, ud);
    else
      COPY_DIGITS(us, unb, old_und, ud, MINT(unb, vnd), nd - 1, d);
    
#ifndef MAX_NBITS
    delete [] d;
#endif
    
  }
  
#undef COPY_DIGITS
  
}


/////////////////////////////////////////////////////////////////////////////
// SECTION: External functions for AND operators.
/////////////////////////////////////////////////////////////////////////////


// Handles the cases 2-5 and returns the result in u.
void
and_on_help(small_type us, 
            length_type unb, length_type und, 
            digit_type *ud, 
            small_type vs,
            length_type vnb, length_type vnd,
            const digit_type *vd)
{

  register digit_type *x = ud;
  register const digit_type *y = vd;
  length_type xnd = und;
  length_type ynd = vnd;

  // Truncate y.
  if (xnd < ynd)
    ynd = xnd;

  const digit_type *xend = (x + xnd);
  const digit_type *yend = (y + ynd);

  // x is longer than y.

  small_type s = mul_signs(us, vs);

  if (s > 0) {

    if (us > 0) { // case 2

      while (y < yend)
        (*x++) &= (*y++);

      while (x < xend)
        (*x++) = 0;

    }
    else {  // case 3

      register digit_type xcarry = 1;
      register digit_type ycarry = 1;

      while (y < yend) {
        xcarry += (~(*x) & DIGIT_MASK);
        ycarry += (~(*y++) & DIGIT_MASK);
        (*x++) = (xcarry & ycarry) & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
        ycarry >>= BITS_PER_DIGIT;
      }

      while (x < xend) {
        xcarry += (~(*x) & DIGIT_MASK);
        ycarry += DIGIT_MASK;
        (*x++) = (xcarry & ycarry) & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
        ycarry >>= BITS_PER_DIGIT;
      }

    }
  }
  else {

    if (us > 0) { // case 4

      register digit_type ycarry = 1;

      while (y < yend) {
        ycarry += (~(*y++) & DIGIT_MASK);
        (*x++) &= ycarry & DIGIT_MASK;
        ycarry >>= BITS_PER_DIGIT;
      }

      while (x < xend) {
        ycarry += DIGIT_MASK;
        (*x++) &= ycarry & DIGIT_MASK;
        ycarry >>= BITS_PER_DIGIT;
      }

    }
    else {  // case 5

      register digit_type xcarry = 1;

      while (y < yend) {
        xcarry += (~(*x) & DIGIT_MASK);
        (*x++) = (xcarry & (*y++)) & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
      }

      while (x < xend)
        (*x++) = 0;

    }
  }
}


/////////////////////////////////////////////////////////////////////////////
// SECTION: External functions for OR operators.
/////////////////////////////////////////////////////////////////////////////


// Handles the cases 3-5 and returns the result in u.
void
or_on_help(small_type us, 
           length_type unb, length_type und, 
           digit_type *ud, 
           small_type vs,
           length_type vnb, length_type vnd,
           const digit_type *vd)
{
  
  register digit_type *x = ud;
  register const digit_type *y = vd;
  length_type xnd = und;
  length_type ynd = vnd;

  if (xnd < ynd)
    ynd = xnd;

  const digit_type *xend = (x + xnd);
  const digit_type *yend = (y + ynd);

  // x is longer than y.

  small_type s = mul_signs(us, vs);

  if (s > 0) {

    if (us > 0) { // case 3

      while (y < yend)
        (*x++) |= (*y++);

      // No change for the rest of x.

    }
    else {  // case 4

      register digit_type xcarry = 1;
      register digit_type ycarry = 1;

      while (y < yend) {
        xcarry += (~(*x) & DIGIT_MASK);
        ycarry += (~(*y++) & DIGIT_MASK);
        (*x++) = (xcarry | ycarry) & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
        ycarry >>= BITS_PER_DIGIT;
      }

      while (x < xend) {
        xcarry += (~(*x) & DIGIT_MASK);
        ycarry += DIGIT_MASK;
        (*x++) = (xcarry | ycarry) & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
        ycarry >>= BITS_PER_DIGIT;
      }
    }

  }
  else {

    if (us > 0) { // case 5

      register digit_type ycarry = 1;

      while (y < yend) {
        ycarry += (~(*y++) & DIGIT_MASK);
        (*x) = ((*x) | ycarry) & DIGIT_MASK;
        x++;
        ycarry >>= BITS_PER_DIGIT;
      }

      while (x < xend) {
        ycarry += DIGIT_MASK;
        (*x) = ((*x) | ycarry) & DIGIT_MASK;
        x++;
        ycarry >>= BITS_PER_DIGIT;
      }

    }
    else {  // case 6

      register digit_type xcarry = 1;

      while (y < yend) {
        xcarry += (~(*x) & DIGIT_MASK);
        (*x++) = (xcarry | (*y++)) & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
      }

      while (x < xend) {
        xcarry += (~(*x) & DIGIT_MASK);
        (*x++) = xcarry & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
      }
    }
  }
}


/////////////////////////////////////////////////////////////////////////////
// SECTION: External functions for XOR operators.
/////////////////////////////////////////////////////////////////////////////


// Handles the cases 3-5 and returns the result in u.
void
xor_on_help(small_type us, 
            length_type unb, length_type und, 
            digit_type *ud, 
            small_type vs,
            length_type vnb, length_type vnd,
            const digit_type *vd)
{
  
  register digit_type *x = ud;
  register const digit_type *y = vd;
  length_type xnd = und;
  length_type ynd = vnd;

  if (xnd < ynd)
    ynd = xnd;

  const digit_type *xend = (x + xnd);
  const digit_type *yend = (y + ynd);

  // x is longer than y.

  small_type s = mul_signs(us, vs);

  if (s > 0) {

    if (us > 0) { // case 3

      while (y < yend) {
        (*x) = ((*x) ^ (*y)) & DIGIT_MASK;
        x++;
        y++;
      }

      // No change for the rest of x.

    }
    else {  // case 4

      register digit_type xcarry = 1;
      register digit_type ycarry = 1;

      while (y < yend) {
        xcarry += (~(*x) & DIGIT_MASK);
        ycarry += (~(*y++) & DIGIT_MASK);
        (*x++) = (xcarry ^ ycarry) & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
        ycarry >>= BITS_PER_DIGIT;
      }

      while (x < xend) {
        xcarry += (~(*x) & DIGIT_MASK);
        ycarry += DIGIT_MASK;
        (*x++) = (xcarry ^ ycarry) & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
        ycarry >>= BITS_PER_DIGIT;
      }
    }
  }
  else {

    if (us > 0) { // case 5

      register digit_type ycarry = 1;

      while (y < yend) {
        ycarry += (~(*y++) & DIGIT_MASK);
        (*x) = ((*x) ^ ycarry) & DIGIT_MASK;
        x++;
        ycarry >>= BITS_PER_DIGIT;
      }

      while (x < xend) {
        ycarry += DIGIT_MASK;
        (*x) = ((*x) ^ ycarry) & DIGIT_MASK;
        x++;
        ycarry >>= BITS_PER_DIGIT;
      }

    }
    else {  // case 6

      register digit_type xcarry = 1;

      while (y < yend) {
        xcarry += (~(*x) & DIGIT_MASK);
        (*x++) = (xcarry ^ (*y++)) & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
      }

      while (x < xend) {
        xcarry += (~(*x) & DIGIT_MASK);
        (*x++) = xcarry & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
      }
    }
  }
}

// End of file
