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

    sc_nbdefs.h -- Top level header file for arbitrary precision
    signed/unsigned arithmetic. This file defines all the constants
    needed.

    Original Author: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)
  
******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_DEFS_H
#define SC_DEFS_H

#include <assert.h>

#ifndef _MSC_VER
#include <iostream>
using std::ostream;
using std::istream;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::hex;
#else
// MSVC6.0 has bugs in standard library
#include <iostream.h>
#include "../sc_cmnhdr.h"
#endif

#include <stdio.h>
#include <stdlib.h>  // For abort().
#include <limits.h>  // For CHAR_BIT.

#include "sc_constants.h"   // For MAX_NBITS
#include "sc_string.h"      // For sc_numrep

// Sign of a number:
#define SC_NEG       -1     // Negative number
#define SC_ZERO      0      // Zero
#define SC_POS       1      // Positive number
#define SC_NOSIGN    2      // Uninitialized sc_signed number

typedef bool          bit;
typedef unsigned char uchar;

// A small_type number is at least a char. Defining an int is probably
// better for alignment.
typedef int small_type;

// Type of a digit. The number of bits in digit_type must be at least
// 3 * CHAR_BIT.
typedef unsigned long digit_type;

// Type of the number for the number of digits.
typedef int length_type;

// Attributes of a byte.
const uchar      BITS_PER_BYTE = CHAR_BIT;
const digit_type BYTE_RADIX    = ((digit_type) 1) << BITS_PER_BYTE;
const digit_type BYTE_MASK     = BYTE_RADIX - 1;

// LOG2_BITS_PER_BYTE = log2(BITS_PER_BYTE), assuming that
// BITS_PER_BYTE is a power of 2. 
const uchar      LOG2_BITS_PER_BYTE = 3;  

// Attributes of the digit_type. These definitions are used mainly in
// the functions that are aware of the internal representation of
// digits, e.g., get/set_packed_rep().
const digit_type BYTES_PER_DIGIT_TYPE = sizeof(digit_type);
const digit_type BITS_PER_DIGIT_TYPE  = BYTES_PER_DIGIT_TYPE * BITS_PER_BYTE;

// Attributes of a digit, i.e., digit_type less the overflow bits.
const digit_type BYTES_PER_DIGIT = BYTES_PER_DIGIT_TYPE;
const digit_type BITS_PER_DIGIT  = BITS_PER_DIGIT_TYPE - 2;
const digit_type DIGIT_RADIX     = ((digit_type) 1) << BITS_PER_DIGIT;
const digit_type DIGIT_MASK      = DIGIT_RADIX - 1;
// Make sure that BYTES_PER_DIGIT = ceil(BITS_PER_DIGIT / BITS_PER_BYTE).

// Similar attributes for the half of a digit. Note that
// HALF_DIGIT_RADIX is equal to the square root of DIGIT_RADIX. These
// definitions are used mainly in the multiplication routines.
const digit_type BITS_PER_HALF_DIGIT = BITS_PER_DIGIT / 2;
const digit_type HALF_DIGIT_RADIX    = ((digit_type) 1) << BITS_PER_HALF_DIGIT;
const digit_type HALF_DIGIT_MASK     = HALF_DIGIT_RADIX - 1;

// DIV_CEIL2(x, y) = ceil(x / y). x and y are positive numbers.
#define DIV_CEIL2(x, y) (((x) - 1) / (y) + 1)

// DIV_CEIL(x) = ceil(x / BITS_PER_DIGIT) = the number of digits to
// store x bits. x is a positive number.
#define DIV_CEIL(x) DIV_CEIL2(x, BITS_PER_DIGIT)

#ifdef MAX_NBITS
const length_type MAX_NDIGITS      = DIV_CEIL(MAX_NBITS) + 2;
// Consider a number with x bits another with y bits. The maximum
// number of bits happens when we multiply them. The result will have
// (x + y) bits. Assume that x + y <= MAX_NBITS. Then, DIV_CEIL(x) +
// DIV_CEIL(y) <= DIV_CEIL(MAX_NBITS) + 2. This is the reason for +2
// above. With this change, MAX_NDIGITS must be enough to hold the
// result of any operation.
#endif

// Support for the long long type. This type is not in the standard
// but is usually supported by compilers.
#ifndef WIN32
typedef long long           int64;
typedef unsigned long long uint64;
#else
typedef __int64             int64;
typedef unsigned __int64   uint64;
#endif

// Bits per ...
const length_type BITS_PER_CHAR   = BITS_PER_BYTE;
const length_type BITS_PER_INT    = BITS_PER_BYTE * sizeof(int);
const length_type BITS_PER_LONG   = BITS_PER_BYTE * sizeof(long);
const length_type BITS_PER_INT64  = BITS_PER_BYTE * sizeof(int64);
const length_type BITS_PER_UINT   = BITS_PER_INT + 1;
const length_type BITS_PER_ULONG  = BITS_PER_LONG + 1;
const length_type BITS_PER_UINT64 = BITS_PER_INT64 + 1;

// Digits per ...
const length_type DIGITS_PER_CHAR   = DIV_CEIL(BITS_PER_CHAR);
const length_type DIGITS_PER_INT    = DIV_CEIL(BITS_PER_INT);
const length_type DIGITS_PER_LONG   = DIV_CEIL(BITS_PER_LONG);
const length_type DIGITS_PER_INT64  = DIV_CEIL(BITS_PER_INT64);
const length_type DIGITS_PER_UINT   = DIV_CEIL(BITS_PER_UINT);
const length_type DIGITS_PER_ULONG  = DIV_CEIL(BITS_PER_ULONG);
const length_type DIGITS_PER_UINT64 = DIV_CEIL(BITS_PER_UINT64);

// Above, BITS_PER_X is mainly used for sc_signed, and BITS_PER_UX is
// mainly used for sc_unsigned.

#if defined(__GNUC__)
typedef ios ios_base;
#endif

#if defined(WIN32) || defined(__SUNPRO_CC)
typedef unsigned long fmtflags;
#else
typedef ios_base::fmtflags fmtflags;
#endif

const char       NEW_LINE = '\n';
const small_type NB_DEFAULT_BASE = SC_DEC;

#define NEW_SEMANTICS

// For sc_int code:
#define LLWIDTH  BITS_PER_INT64
#define INTWIDTH BITS_PER_INT

#endif
