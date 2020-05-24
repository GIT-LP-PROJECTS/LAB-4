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

    sc_nbfriends.cpp -- Friend functions for both sc_signed and
    sc_unsigned classes. The vec_* functions are called through either
    these functions or those in sc_nbexterns.cpp. These functions
    perform their work on two inputs u and v, and return the result
    object. The functions in sc_nbexterns.cpp perform their work on
    one of their inputs.

    The functions here try to use faster algorithms in case the input
    numbers are small. The bitwise functions (and, or, and xor) need
    the 2's complement representations of their inputs. Instead of
    complementing their inputs first and then processing, they
    complement their inputs while processing without allocating extra
    temporary memory.

    Original Author: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// Naming conventions:
// For sc_signed or sc_unsigned number u:
//   us : u's sign, unb : u's number of bits, 
//   und : u's number of digits, ud : u's digits array.
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// SECTION: Friend functions for PLUS operators.
/////////////////////////////////////////////////////////////////////////////

// Handles cases 3 and 4 and returns the result.
CLASS_TYPE
ADD_HELPER(small_type us, length_type unb, length_type und,
           const digit_type *ud, 
           small_type vs, length_type vnb, length_type vnd,
           const digit_type *vd)
{

  und = vec_skip_leading_zeros(und, ud);
  vnd = vec_skip_leading_zeros(vnd, vd);

  length_type nb = MAXT(unb, vnb);
  length_type nd = MAXT(und, vnd) + 1;

#ifdef MAX_NBITS
  test_bound(nb);
  digit_type d[MAX_NDIGITS];
#else
  digit_type *d = new digit_type[nd];
#endif
  
  d[nd - 1] = d[nd - 2] = 0;

  // case 3
  if (us == vs) {

    ++nb;

    if ((und == 1) && (vnd == 1)) {
      digit_type carry = (*ud) + (*vd);
      d[0] = carry & DIGIT_MASK;
      d[1] = carry >> BITS_PER_DIGIT;
    }

    else if (und >= vnd)
      vec_add(und, ud, vnd, vd, d);

    else
      vec_add(vnd, vd, und, ud, d);

  }

  // case 4
  else {

    length_type cmp_res = vec_cmp(und, ud, vnd, vd);
    
    if (cmp_res == 0) // u == v
      return CLASS_TYPE();
    
    if (cmp_res > 0)  { // u > v

      if ((und == 1) && (vnd == 1))
        d[0] = (*ud) - (*vd);
      else
        vec_sub(und, ud, vnd, vd, d);

    }
    else { // u < v

      us = -us;

      if ((und == 1) && (vnd == 1))
        d[0] = (*vd) - (*ud);
      else
        vec_sub(vnd, vd, und, ud, d);

    }
  }
  
  return CLASS_TYPE(us, nb, nd, d);
  
}


/////////////////////////////////////////////////////////////////////////////
// SECTION: Friend functions of MULTIPLICATION operators.
/////////////////////////////////////////////////////////////////////////////

// Handles the case 4 and returns the result.
CLASS_TYPE 
MUL_HELPER(small_type s, 
           length_type unb, length_type und, 
           const digit_type *ud, 
           length_type vnb, length_type vnd,
           const digit_type *vd)
{

  und = vec_skip_leading_zeros(und, ud);
  vnd = vec_skip_leading_zeros(vnd, vd);

  length_type nb = unb + vnb;
  length_type nd = und + vnd;
  
#ifdef MAX_NBITS
  test_bound(nb);
  digit_type d[MAX_NDIGITS];
#else
  digit_type *d = new digit_type[nd];
#endif

  vec_zero(nd, d);
    
  digit_type ud0 = (*ud);
  digit_type vd0 = (*vd);

  if ((vnd == 1) && (vd0 == 1))
    vec_copy(und, d, ud);

  else if ((und == 1) && (ud0 == 1))
    vec_copy(vnd, d, vd);

  else if ((und == 1) && (vnd == 1) && 
      (ud0 < HALF_DIGIT_RADIX) && (vd0 < HALF_DIGIT_RADIX))
    d[0] = ud0 * vd0;
  
  else if ((und == 1) && (ud0 < HALF_DIGIT_RADIX))
    vec_mul_small(vnd, vd, ud0, d);

  else if ((vnd == 1) && (vd0 < HALF_DIGIT_RADIX))
    vec_mul_small(und, ud, vd0, d);

  else if (vnd < und)
    vec_mul(und, ud, vnd, vd, d);

  else
    vec_mul(vnd, vd, und, ud, d);
  
  return CLASS_TYPE(s, nb, nd, d);

}


/////////////////////////////////////////////////////////////////////////////
// SECTION: Friend functions for DIVISION operators.
/////////////////////////////////////////////////////////////////////////////

// Handles the cases 3-4 and returns the result.
CLASS_TYPE 
DIV_HELPER(small_type s, 
           length_type unb, length_type und, 
           const digit_type *ud, 
           length_type vnb, length_type vnd,
           const digit_type *vd)
{

  und = vec_skip_leading_zeros(und, ud);
  vnd = vec_skip_leading_zeros(vnd, vd);

  length_type cmp_res = vec_cmp(und, ud, vnd, vd);

  // u < v => u / v = 0 - case 4
  if (cmp_res < 0) 
    return CLASS_TYPE();

  // One extra digit for d is allocated to simplify vec_div_*().
  length_type nd = MAXT(und, vnd) + 1;

#ifdef MAX_NBITS
  digit_type d[MAX_NDIGITS + 1];
#else
  digit_type *d = new digit_type[nd];
#endif

  vec_zero(nd, d);

  digit_type vd0 = (*vd);

  // u = v => u / v = 1 - case 3
  if (cmp_res == 0)
    d[0] = 1;

  // else if u > v - case 5

  else if ((vnd == 1) && (vd0 == 1))
    vec_copy(und, d, ud);

  else if ((vnd == 1) && (und == 1))
    d[0] = (*ud) / vd0;
 
  else if ((vnd == 1) && (vd0 < HALF_DIGIT_RADIX))
    vec_div_small(und, ud, vd0, d);

  else
    vec_div_large(und, ud, vnd, vd, d);

  return CLASS_TYPE(s, MAXT(unb, vnb), nd - 1, d);
  
}


/////////////////////////////////////////////////////////////////////////////
// SECTION: Friend functions for MOD operators.
/////////////////////////////////////////////////////////////////////////////

// Handles the cases 3-4 and returns the result.
CLASS_TYPE 
MOD_HELPER(small_type us, 
           length_type unb, length_type und, 
           const digit_type *ud, 
           length_type vnb, length_type vnd,
           const digit_type *vd)
{

  und = vec_skip_leading_zeros(und, ud);
  vnd = vec_skip_leading_zeros(vnd, vd);

  length_type cmp_res = vec_cmp(und, ud, vnd, vd);

  // u = v => u % v = 0 - case 3
  if (cmp_res == 0) 
    return CLASS_TYPE();

  digit_type vd0 = (*vd);

  if ((cmp_res > 0) && (vnd == 1) && (vd0 == 1))
    return CLASS_TYPE();

  // One extra digit for d is allocated to simplify vec_div_*().
  length_type nd = MAXT(und, vnd) + 1;

#ifdef MAX_NBITS
  digit_type d[MAX_NDIGITS + 1];
#else
  digit_type *d = new digit_type[nd];
#endif

  vec_zero(nd, d);

  // u < v => u % v = u - case 4
  if (cmp_res < 0) 
    vec_copy(und, d, ud);

  // else if u > v - case 5

  else if ((vnd == 1) && (und == 1))
    d[0] = (*ud) % vd0;

  else if ((vnd == 1) && (vd0 < HALF_DIGIT_RADIX))
    d[0] = vec_rem_small(und, ud, vd0);

  else
    vec_rem_large(und, ud, vnd, vd, d);

  us = check_for_zero(us, nd - 1, d);

  if (us == SC_ZERO)
    return CLASS_TYPE();
  else
    return CLASS_TYPE(us, MINT(unb, vnb), nd - 1, d);

}


/////////////////////////////////////////////////////////////////////////////
// SECTION: Friend functions for AND operators.
/////////////////////////////////////////////////////////////////////////////

// Handles the cases 2-5 and returns the result.
CLASS_TYPE 
AND_HELPER(small_type us, 
           length_type unb, length_type und, 
           const digit_type *ud, 
           small_type vs,
           length_type vnb, length_type vnd,
           const digit_type *vd)
{

  length_type nb = MAXT(unb, vnb);
  length_type nd = MAXT(und, vnd);

#ifdef MAX_NBITS
  digit_type dbegin[MAX_NDIGITS];
#else
  digit_type *dbegin = new digit_type[nd];
#endif

  digit_type *d = dbegin;

  register const digit_type *x;
  register const digit_type *y;
  length_type xnd;
  length_type ynd;
  small_type xs;
  small_type ys;

  if (und >= vnd) {
    x = ud;
    y = vd;
    xnd = und;
    ynd = vnd;
    xs = us;
    ys = vs;
  }
  else {
    y = ud;
    x = vd;
    ynd = und;
    xnd = vnd;
    ys = us;
    xs = vs;
  }

  const digit_type *xend = (x + xnd);
  const digit_type *yend = (y + ynd);

  // x is longer than y.

  small_type s = mul_signs(xs, ys);

  if (s > 0) {

    if (xs > 0) { // case 2

      while (y < yend)
        (*d++) = (*x++) & (*y++);

      while (x++ < xend) 
        (*d++) = 0;

    }
    else {  // case 3

      register digit_type xcarry = 1;
      register digit_type ycarry = 1;

      while (y < yend) {
        xcarry += (~(*x++) & DIGIT_MASK);
        ycarry += (~(*y++) & DIGIT_MASK);
        (*d++) = (xcarry & ycarry) & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
        ycarry >>= BITS_PER_DIGIT;
      }

      while (x < xend) {
        xcarry += (~(*x++) & DIGIT_MASK);
        ycarry += DIGIT_MASK;
        (*d++) = (xcarry & ycarry) & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
        ycarry >>= BITS_PER_DIGIT;
      }

    }
  }
  else {

    if (xs > 0) { // case 4

      register digit_type ycarry = 1;

      while (y < yend) {
        ycarry += (~(*y++) & DIGIT_MASK);
        (*d++) = ((*x++) & ycarry) & DIGIT_MASK;
        ycarry >>= BITS_PER_DIGIT;
      }

      while (x < xend) {
        ycarry += DIGIT_MASK;
        (*d++) = ((*x++) & ycarry) & DIGIT_MASK;
        ycarry >>= BITS_PER_DIGIT;
      }

    }
    else {  // case 5

      register digit_type xcarry = 1;

      while (y < yend) {
        xcarry += (~(*x++) & DIGIT_MASK);
        (*d++) = (xcarry & (*y++)) & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
      }

      while (x++ < xend)
        (*d++) = 0;

    }
  }

  s = convert_signed_2C_to_SM(nb, nd, dbegin);

  return CLASS_TYPE(s, nb, nd, dbegin);  

}


/////////////////////////////////////////////////////////////////////////////
// SECTION: Friend functions for OR operators.
/////////////////////////////////////////////////////////////////////////////

// Handles the cases 3-5 and returns the result.
CLASS_TYPE 
OR_HELPER(small_type us, 
          length_type unb, length_type und, 
          const digit_type *ud, 
          small_type vs,
          length_type vnb, length_type vnd,
          const digit_type *vd)
{
  
  length_type nb = MAXT(unb, vnb);
  length_type nd = MAXT(und, vnd);

#ifdef MAX_NBITS
  digit_type dbegin[MAX_NDIGITS];
#else
  digit_type *dbegin = new digit_type[nd];
#endif

  digit_type *d = dbegin;

  register const digit_type *x;
  register const digit_type *y;
  length_type xnd;
  length_type ynd;
  small_type xs;
  small_type ys;

  if (und >= vnd) {
    x = ud;
    y = vd;
    xnd = und;
    ynd = vnd;
    xs = us;
    ys = vs;
  }
  else {
    y = ud;
    x = vd;
    ynd = und;
    xnd = vnd;
    ys = us;
    xs = vs;
  }

  const digit_type *xend = (x + xnd);
  const digit_type *yend = (y + ynd);

  // x is longer than y.

  small_type s = mul_signs(xs, ys);

  if (s > 0) {

    if (xs > 0) { // case 3

      while (y < yend)
        (*d++) = (*x++) | (*y++);

      while (x < xend)
        (*d++) = (*x++);

    }
    else {  // case 4

      register digit_type xcarry = 1;
      register digit_type ycarry = 1;

      while (y < yend) {
        xcarry += (~(*x++) & DIGIT_MASK);
        ycarry += (~(*y++) & DIGIT_MASK);
        (*d++) = (xcarry | ycarry) & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
        ycarry >>= BITS_PER_DIGIT;
      }

      while (x < xend) {
        xcarry += (~(*x++) & DIGIT_MASK);
        ycarry += DIGIT_MASK;
        (*d++) = (xcarry | ycarry) & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
        ycarry >>= BITS_PER_DIGIT;
      }

    }

  }
  else {

    if (xs > 0) { // case 5

      register digit_type ycarry = 1;

      while (y < yend) {
        ycarry += (~(*y++) & DIGIT_MASK);
        (*d++) = ((*x++) | ycarry) & DIGIT_MASK;
        ycarry >>= BITS_PER_DIGIT;
      }

      while (x < xend) {
        ycarry += DIGIT_MASK;
        (*d++) = ((*x++) | ycarry) & DIGIT_MASK;
        ycarry >>= BITS_PER_DIGIT;
      }

    }
    else {  // case 6

      register digit_type xcarry = 1;

      while (y < yend) {
        xcarry += (~(*x++) & DIGIT_MASK);
        (*d++) = (xcarry | (*y++)) & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
      }

      while (x < xend) {
        xcarry += (~(*x++) & DIGIT_MASK);
        (*d++) = xcarry & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
      }
    }

  }

  s = convert_signed_2C_to_SM(nb, nd, dbegin);

  return CLASS_TYPE(s, nb, nd, dbegin);

}


/////////////////////////////////////////////////////////////////////////////
// SECTION: Friend functions for XOR operators.
/////////////////////////////////////////////////////////////////////////////

// Handles the cases 3-5 and returns the result.
CLASS_TYPE 
XOR_HELPER(small_type us, 
           length_type unb, length_type und, 
           const digit_type *ud, 
           small_type vs,
           length_type vnb, length_type vnd,
           const digit_type *vd)
{
  
  length_type nb = MAXT(unb, vnb);
  length_type nd = MAXT(und, vnd);

#ifdef MAX_NBITS
  digit_type dbegin[MAX_NDIGITS];
#else
  digit_type *dbegin = new digit_type[nd];
#endif

  digit_type *d = dbegin;

  register const digit_type *x;
  register const digit_type *y;
  length_type xnd;
  length_type ynd;
  small_type xs;
  small_type ys;

  if (und >= vnd) {
    x = ud;
    y = vd;
    xnd = und;
    ynd = vnd;
    xs = us;
    ys = vs;
  }
  else {
    y = ud;
    x = vd;
    ynd = und;
    xnd = vnd;
    ys = us;
    xs = vs;
  }

  const digit_type *xend = (x + xnd);
  const digit_type *yend = (y + ynd);

  // x is longer than y.

  small_type s = mul_signs(xs, ys);

  if (s > 0) {

    if (xs > 0) { // case 3

      while (y < yend)
        (*d++) = ((*x++) ^ (*y++)) & DIGIT_MASK;

      while (x < xend)
        (*d++) = (*x++);

    }
    else {  // case 4

      register digit_type xcarry = 1;
      register digit_type ycarry = 1;

      while (y < yend) {
        xcarry += (~(*x++) & DIGIT_MASK);
        ycarry += (~(*y++) & DIGIT_MASK);
        (*d++) = (xcarry ^ ycarry) & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
        ycarry >>= BITS_PER_DIGIT;
      }

      while (x < xend) {
        xcarry += (~(*x++) & DIGIT_MASK);
        ycarry += DIGIT_MASK;
        (*d++) = (xcarry ^ ycarry) & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
        ycarry >>= BITS_PER_DIGIT;
      }

    }
  }
  else {

    if (xs > 0) { // case 5

      register digit_type ycarry = 1;

      while (y < yend) {
        ycarry += (~(*y++) & DIGIT_MASK);
        (*d++) = ((*x++) ^ ycarry) & DIGIT_MASK;
        ycarry >>= BITS_PER_DIGIT;
      }

      while (x < xend) {
        ycarry += DIGIT_MASK;
        (*d++) = ((*x++) ^ ycarry) & DIGIT_MASK;
        ycarry >>= BITS_PER_DIGIT;
      }

    }
    else {  // case 6

      register digit_type xcarry = 1;

      while (y < yend) {
        xcarry += (~(*x++) & DIGIT_MASK);
        (*d++) = (xcarry ^ (*y++)) & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
      }

      while (x < xend) {
        xcarry += (~(*x++) & DIGIT_MASK);
        (*d++) = xcarry & DIGIT_MASK;
        xcarry >>= BITS_PER_DIGIT;
      }
    }
  }

  s = convert_signed_2C_to_SM(nb, nd, dbegin);

  return CLASS_TYPE(s, nb, nd, dbegin);

}

// End of file.

