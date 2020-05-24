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

    sc_signed_subref.h -- Proxy class that is declared in sc_signed.h.  

    Original Author: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


sc_signed_subref& 
sc_signed_subref::operator=(const sc_signed_subref& v)
{
  if (this == &v)
    return *this;

  return operator=(v.operator sc_signed());
}


sc_signed_subref& 
sc_signed_subref::operator=(const sc_signed& v)
{

  if (right >= v.nbits) {
    warn(right < v.nbits, "Misaligned assignment - no assignment is done.");
    return *this;
  }

  length_type l = MINT(left, v.nbits - 1 + right);

  for (register length_type i = right; i < l; ++i)
    snum->set(i, v.test(i - right));

  return *this;
  
}


sc_signed_subref& 
sc_signed_subref::operator=(const sc_unsigned_subref& v)
{
  return operator=(v.operator sc_unsigned());
}


sc_signed_subref& 
sc_signed_subref::operator=(const sc_unsigned& v)
{

  if (right >= v.nbits) {
    warn(right < v.nbits, "Misaligned assignment - no assignment is done.");
    return *this;
  }

  length_type l = MINT(left, v.nbits - 1 + right);

  for (register length_type i = right; i < l; ++i)
    snum->set(i, v.test(i - right));

  return *this;
  
}


sc_signed_subref& 
sc_signed_subref::operator=(const char* v)
{

  length_type nb = left - right + 1;
  length_type nd = DIV_CEIL(nb);

#ifdef MAX_NBITS

  digit_type d[MAX_NDIGITS];

#else

  digit_type *d = new digit_type[nd];

#endif

  vec_from_str(nb, nd, d, v);

  digit_type val = 1;  // Bit value.
  length_type j = 0;   // Current digit in d.

  register length_type i = 0;  // Current bit in d.

  while (i < nb) {

    snum->set(i + right, (bool) (d[j] & val));

    ++i;

    if (i % BITS_PER_DIGIT == 0) {
      val = 1;
      ++j;
    }
    else
      val <<= 1;

  }

#ifndef MAX_NBITS

  delete [] d;

#endif

  return *this;

}


sc_signed_subref& 
sc_signed_subref::operator=(int64 v) 
{

  uint64 v2 = (uint64) v;

  for (register length_type i = right; i <= left; ++i) {
    snum->set(i, (v2 & 1));
    v2 >>= 1;
  }

  return *this;

}


sc_signed_subref& 
sc_signed_subref::operator=(uint64 v)
{

  for (register length_type i = right; i <= left; ++i) {
    snum->set(i, (v & 1));
    v >>= 1;
  }

  return *this;

}


sc_signed_subref& 
sc_signed_subref::operator=(long v) 
{

  unsigned long v2 = (unsigned long) v;

  for (register length_type i = right; i <= left; ++i) {
    snum->set(i, (v2 & 1));
    v2 >>= 1;
  }

  return *this;

}


sc_signed_subref& 
sc_signed_subref::operator=(unsigned long v)
{

  for (register length_type i = right; i <= left; ++i) {
    snum->set(i, (v & 1));
    v >>= 1;
  }

  return *this;

}


sc_signed_subref& 
sc_signed_subref::operator=(double v)
{

  is_bad_double(v);

  length_type nb = left - right + 1;
  length_type nd = DIV_CEIL(nb);

#ifdef MAX_NBITS

  digit_type d[MAX_NDIGITS];

#else

  digit_type *d = new digit_type[nd];

#endif

  if (v < 0)
    v = -v;

  register length_type i = 0;

  while (floor(v) && (i < nd)) {
#ifndef WIN32
    d[i++] = (digit_type) floor(remainder(v, DIGIT_RADIX));
#else
    d[i++] = (digit_type) floor(fmod(v, DIGIT_RADIX));
#endif
    v /= DIGIT_RADIX;
  }

  vec_zero(i, nd, d);

  digit_type val = 1;  // Bit value.
  length_type j = 0;   // Current digit in d.

  i = 0;  // Current bit in d.

  while (i < nb) {

    snum->set(i + right, (bool) (d[j] & val));

    ++i;

    if (i % BITS_PER_DIGIT == 0) {
      val = 1;
      ++j;
    }
    else
      val <<= 1;

  }

#ifndef MAX_NBITS

  delete [] d;

#endif

  return *this;
}


sc_signed_subref::operator sc_signed() const
{
  return sc_signed(snum, left, right);
}


istream& operator>>(istream& is, sc_signed_subref& u)
{

  sc_signed u2(u.left - u.right + 1);
  is >> u2;
  u = u2;
  return is;
}


ostream& operator<<(ostream& os, const sc_signed_subref& u)
{
  os << u.operator sc_signed();
  return os;
}


// End of file
