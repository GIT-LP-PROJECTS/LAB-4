#ifndef VS_ADDON
#define VS_ADDON
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
#endif
#include <stdlib.h>
#include <assert.h>

#include "sc_process.h"
#include "sc_simcontext.h"
#ifndef SC_LAMBDA_H
#define SC_LAMBDA_H
#include <assert.h>
#include "sc_macros.h"
#include "sc_mempool.h"
#include "sc_signal.h"

#define NO_INTEGER_LAMBDA

// Abbreviations: rator = operator, rand = operand

enum sc_lambda_rator_e
{
	SC_LAMBDA_FALSE = 0x00000000,
	SC_LAMBDA_TRUE,

	/* logical operators */
	SC_LAMBDA_AND,
	SC_LAMBDA_OR,
	SC_LAMBDA_NOT,

	SC_LAMBDA_BOOL,
	SC_LAMBDA_BOOL_EQ,
	SC_LAMBDA_BOOL_NE,

	/* relational operators for sc_logic */
	SC_LAMBDA_SUL_EQ,
	SC_LAMBDA_SUL_NE,

	/* relational operators for int */
	SC_LAMBDA_INT_EQ,
	SC_LAMBDA_INT_NE,
	SC_LAMBDA_INT_LE,
	SC_LAMBDA_INT_GE,
	SC_LAMBDA_INT_LT,
	SC_LAMBDA_INT_GT,

	/* arithmetic operators for sc_logic */
	SC_LAMBDA_SUL_BITAND,
	SC_LAMBDA_SUL_BITOR,
	SC_LAMBDA_SUL_BITNOT,
	SC_LAMBDA_SUL_BITXOR,

	/* arithmetic operators for ints */
	SC_LAMBDA_INT_ADD,
	SC_LAMBDA_INT_SUB,
	SC_LAMBDA_INT_MUL,
	SC_LAMBDA_INT_DIV,
	SC_LAMBDA_INT_REM,
	SC_LAMBDA_INT_BITAND,
	SC_LAMBDA_INT_BITOR,
	SC_LAMBDA_INT_BITNOT,
	SC_LAMBDA_INT_BITXOR
};

class sc_lambda;
class sc_lambda_rand;
class sc_lambda_ptr;

class sc_lambda
{
	friend class sc_lambda_ptr;
	friend class sc_lambda_rand;
	friend class sc_port_manager;

#ifdef SC_LAMBDA_H

#define L_DECL1(op, rator, ty1, exp1) \
	friend inline sc_lambda_ptr operator op(ty1);

#define L_DECL1SPEC(op, ty1, code) \
	friend inline sc_lambda_ptr operator op(ty1);

#define L_DECL2(op, rator, ty1, ty2, exp1, exp2) \
	friend inline sc_lambda_ptr operator op(ty1, ty2);

#define L_DECL2SPEC(op, ty1, ty2, code) \
	friend inline sc_lambda_ptr operator op(ty1, ty2);

#ifdef L_DECL1

#ifndef NO_INTEGER_LAMBDA
	L_DECL2(== , SC_LAMBDA_INT_EQ, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_INT_EQ, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(== , SC_LAMBDA_INT_EQ, int, const sc_signal_int_deval&, s1, s2)

		L_DECL2(== , SC_LAMBDA_INT_EQ, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_INT_EQ, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)
#endif

		L_DECL2(== , SC_LAMBDA_BOOL_EQ, const sc_signal_edgy_deval&, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_BOOL_EQ, const sc_signal_edgy_deval&, bool, s1, s2)
		L_DECL2(== , SC_LAMBDA_BOOL_EQ, bool, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_BOOL_EQ, const sc_lambda_ptr&, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_BOOL_EQ, const sc_signal_edgy_deval&, const sc_lambda_ptr&, s1, s2)

		L_DECL2SPEC(== , const sc_lambda_ptr&, const sc_lambda_ptr&,
	if (s1->is_sc_logic()) {
		assert(s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_SUL_EQ,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else if (s1->is_bool()) {
		assert(s2->is_bool());
		return new sc_lambda(SC_LAMBDA_BOOL_EQ,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else {
		assert(s2->is_int());
		return new sc_lambda(SC_LAMBDA_INT_EQ,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

#ifndef NO_INTEGER_LAMBDA
		//abc
		L_DECL2SPEC(== , const sc_lambda_ptr&, int,
	if (s1->is_bool()) {
		return new sc_lambda(SC_LAMBDA_BOOL_EQ,
			new sc_lambda_rand(s1),
			new sc_lambda_rand((bool)(s2 & 1)));
	}
	else {
		assert(s1->is_int());
		return new sc_lambda(SC_LAMBDA_INT_EQ,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

		L_DECL2SPEC(== , int, const sc_lambda_ptr&,
	if (s2->is_bool()) {
		return new sc_lambda(SC_LAMBDA_BOOL_EQ,
			new sc_lambda_rand((bool)(s1 & 1)),
			new sc_lambda_rand(s2));
	}
	else {
		assert(s2->is_int());
		return new sc_lambda(SC_LAMBDA_INT_EQ,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)
#endif

		/*---------------------------------------------------------------------------*/

		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
		L_DECL2(== , SC_LAMBDA_SUL_EQ, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_lambda_ptr&, char, s1, sc_logic(s2))
		L_DECL2(== , SC_LAMBDA_SUL_EQ, char, const sc_lambda_ptr&, sc_logic(s1), s2)

		/*---------------------------------------------------------------------------*/

#if 0
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const signal_sl&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_lambda_ptr&, const signal_sl&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const signal_sl&, const signal_sl&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const signal_sl&, const sc_logic&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_logic&, const signal_sl&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const signal_sl&, char, s1, sc_logic(s2))
		L_DECL2(== , SC_LAMBDA_SUL_EQ, char, const signal_sl&, sc_logic(s1), s2)
#endif

		/*---------------------------------------------------------------------------*/

		// Overloading operator!=() clashes with STL.  Should look for a way around
		// this, since STL has become a standard library for ANSI C++

#ifndef NO_INTEGER_LAMBDA
		L_DECL2(!= , SC_LAMBDA_INT_NE, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_INT_NE, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(!= , SC_LAMBDA_INT_NE, int, const sc_signal_int_deval&, s1, s2)

		L_DECL2(!= , SC_LAMBDA_INT_NE, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_INT_NE, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)
#endif

		L_DECL2(!= , SC_LAMBDA_BOOL_NE, const sc_signal_edgy_deval&, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_BOOL_NE, const sc_signal_edgy_deval&, bool, s1, s2)
		L_DECL2(!= , SC_LAMBDA_BOOL_NE, bool, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_BOOL_NE, const sc_lambda_ptr&, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_BOOL_NE, const sc_signal_edgy_deval&, const sc_lambda_ptr&, s1, s2)

		L_DECL2SPEC(!= , const sc_lambda_ptr&, const sc_lambda_ptr&,
	if (s1->is_sc_logic()) {
		assert(s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_SUL_NE,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else if (s1->is_bool()) {
		assert(s2->is_bool());
		return new sc_lambda(SC_LAMBDA_BOOL_NE,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else {
		assert(s2->is_int());
		return new sc_lambda(SC_LAMBDA_INT_NE,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

#ifndef NO_INTEGER_LAMBDA
		L_DECL2SPEC(!= , const sc_lambda_ptr&, int,
	if (s1->is_bool()) {
		return new sc_lambda(SC_LAMBDA_BOOL_NE,
			new sc_lambda_rand(s1),
			new sc_lambda_rand((bool)(s2 & 1)));
	}
	else {
		assert(s1->is_int());
		return new sc_lambda(SC_LAMBDA_INT_NE,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

		L_DECL2SPEC(!= , int, const sc_lambda_ptr&,
	if (s2->is_bool()) {
		return new sc_lambda(SC_LAMBDA_BOOL_NE,
			new sc_lambda_rand((bool)(s1 & 1)),
			new sc_lambda_rand(s2));
	}
	else {
		assert(s2->is_int());
		return new sc_lambda(SC_LAMBDA_INT_NE,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)
#endif

		/*---------------------------------------------------------------------------*/

		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
		L_DECL2(!= , SC_LAMBDA_SUL_NE, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_lambda_ptr&, char, s1, sc_logic(s2))
		L_DECL2(!= , SC_LAMBDA_SUL_NE, char, const sc_lambda_ptr&, sc_logic(s1), s2)

		/*---------------------------------------------------------------------------*/

#if 0
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const signal_sl&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_lambda_ptr&, const signal_sl&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const signal_sl&, const signal_sl&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const signal_sl&, const sc_logic&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_logic&, const signal_sl&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const signal_sl&, char, s1, sc_logic(s2))
		L_DECL2(!= , SC_LAMBDA_SUL_NE, char, const signal_sl&, sc_logic(s1), s2)
#endif

		/*---------------------------------------------------------------------------*/
		/*---------------------------------------------------------------------------*/

#ifndef NO_INTEGER_LAMBDA

		L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(<, SC_LAMBDA_INT_LT, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(>, SC_LAMBDA_INT_GT, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(%, SC_LAMBDA_INT_REM, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/
		/*---------------------------------------------------------------------------*/

		L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(&, SC_LAMBDA_INT_BITAND, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(&, SC_LAMBDA_INT_BITAND, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

#endif

		L_DECL2SPEC(&, const sc_lambda_ptr&, const sc_lambda_ptr&,
	if (s1->is_sc_logic()) {
		assert(s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_SUL_BITAND,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else {
		assert(!s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_INT_BITAND,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

		/*---------------------------------------------------------------------------*/

		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)

		/*---------------------------------------------------------------------------*/

#if 0
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const signal_sl&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_lambda_ptr&, const signal_sl&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const signal_sl&, const signal_sl&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const signal_sl&, const sc_logic&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const signal_sl&, char, s1, sc_logic(s2))
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_logic&, const signal_sl&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, char, const signal_sl&, sc_logic(s1), s2)
#endif

		/*---------------------------------------------------------------------------*/
		/*---------------------------------------------------------------------------*/

#ifndef NO_INTEGER_LAMBDA
		L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(| , SC_LAMBDA_INT_BITOR, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(| , SC_LAMBDA_INT_BITOR, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)
#endif

		L_DECL2SPEC(| , const sc_lambda_ptr&, const sc_lambda_ptr&,
	if (s1->is_sc_logic()) {
		assert(s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_SUL_BITOR,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else {
		assert(!s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_INT_BITOR,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

		/*---------------------------------------------------------------------------*/

		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)

		/*---------------------------------------------------------------------------*/

#if 0
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const signal_sl&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_lambda_ptr&, const signal_sl&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const signal_sl&, const signal_sl&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const signal_sl&, const sc_logic&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const signal_sl&, char, s1, sc_logic(s2))
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_logic&, const signal_sl&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, char, const signal_sl&, sc_logic(s1), s2)
#endif

		/*---------------------------------------------------------------------------*/
		/*---------------------------------------------------------------------------*/

#ifndef NO_INTEGER_LAMBDA
		L_DECL1(~, SC_LAMBDA_INT_BITNOT, const sc_signal_int_deval&, s1)
#endif

		L_DECL1(~, SC_LAMBDA_SUL_BITNOT, const sc_signal_sc_logic_deval&, s1)
#if 0
		L_DECL1(~, SC_LAMBDA_SUL_BITNOT, const signal_sl&, s1)
#endif

		L_DECL1SPEC(~, const sc_lambda_ptr&,
	if (s1->is_sc_logic()) {
		return new sc_lambda(SC_LAMBDA_SUL_BITNOT,
			new sc_lambda_rand(s1));
	}
	else {
		return new sc_lambda(SC_LAMBDA_INT_BITNOT,
			new sc_lambda_rand(s1));
	}
	)

		/*---------------------------------------------------------------------------*/

#ifndef NO_INTEGER_LAMBDA
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)
#endif

		L_DECL2SPEC(^, const sc_lambda_ptr&, const sc_lambda_ptr&,
	if (s1->is_sc_logic()) {
		assert(s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_SUL_BITXOR,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else {
		assert(!s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_INT_BITXOR,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

		/*---------------------------------------------------------------------------*/

		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)

		/*---------------------------------------------------------------------------*/

#if 0
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const signal_sl&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_lambda_ptr&, const signal_sl&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const signal_sl&, const signal_sl&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const signal_sl&, const sc_logic&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const signal_sl&, char, s1, sc_logic(s2))
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_logic&, const signal_sl&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, char, const signal_sl&, sc_logic(s1), s2)
#endif

		/*---------------------------------------------------------------------------*/
		/*---------------------------------------------------------------------------*/

		L_DECL2(|| , SC_LAMBDA_OR, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(|| , SC_LAMBDA_OR, const sc_signal_edgy_deval&, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(|| , SC_LAMBDA_OR, const sc_signal_edgy_deval&, bool, s1, s2)
		L_DECL2(|| , SC_LAMBDA_OR, bool, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(|| , SC_LAMBDA_OR, const sc_signal_edgy_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(|| , SC_LAMBDA_OR, const sc_lambda_ptr&, const sc_signal_edgy_deval&, s1, s2)

		L_DECL2(&&, SC_LAMBDA_AND, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(&&, SC_LAMBDA_AND, const sc_signal_edgy_deval&, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(&&, SC_LAMBDA_AND, const sc_signal_edgy_deval&, bool, s1, s2)
		L_DECL2(&&, SC_LAMBDA_AND, bool, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(&&, SC_LAMBDA_AND, const sc_signal_edgy_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(&&, SC_LAMBDA_AND, const sc_lambda_ptr&, const sc_signal_edgy_deval&, s1, s2)

		L_DECL1(!, SC_LAMBDA_NOT, const sc_lambda_ptr&, s1)
		L_DECL1(!, SC_LAMBDA_NOT, const sc_signal_edgy_deval&, s1)

#endif

#undef L_DECL1
#undef L_DECL1SPEC
#undef L_DECL2
#undef L_DECL2SPEC

#endif

public:
	static void* operator new(size_t sz){ return sc_mempool::allocate(sz); }
	static void operator delete(void* p, size_t sz) { sc_mempool::release(p, sz); }

	// Create a lambda out of a bool signal
	explicit sc_lambda(const sc_signal_edgy& b);

	// Evaluates the expression tree.
	bool eval() const;

	// Destructor
	~sc_lambda();

public: // private: // (changed)
	// Constructor - takes an operator code and two operands
	sc_lambda(sc_lambda_rator_e op,
		sc_lambda_rand* o1 = 0, sc_lambda_rand* o2 = 0);
private:
	// Evaluates the expression as an int
	int int_eval() const;

	// Evalutes the expression as a sc_logic
	sc_logic sc_logic_eval() const;

	// Returns true if the type of the expression tree is bool
	bool is_bool() const
	{
		return ((SC_LAMBDA_AND <= rator_ty) && (rator_ty <= SC_LAMBDA_INT_GT));
	}

	// Returns true if the type of the expression tree is int
	bool is_int() const
	{
		return ((SC_LAMBDA_INT_ADD <= rator_ty) &&
			(rator_ty <= SC_LAMBDA_INT_BITXOR));
	}

	// Returns true if the type of the expression tree is sc_logic
	bool is_sc_logic() const
	{
		return ((SC_LAMBDA_SUL_BITAND <= rator_ty) &&
			(rator_ty <= SC_LAMBDA_SUL_BITXOR));
	}

	void replace_ports(void(*fn)(sc_port_manager*, sc_lambda_rand*),
		sc_port_manager* m);

private:
	sc_lambda_rator_e rator_ty;
	sc_lambda_rand* op1;
	sc_lambda_rand* op2;
	int ref_count;              // reference count

private:
	sc_lambda();
	static sc_lambda dummy_lambda;
};

class sc_lambda_ptr
{
	friend class sc_sync;
	friend class sc_sync_process;
	friend class sc_clock;
	friend class sc_lambda;
	friend class sc_lambda_rand;
	friend class sc_port_manager;

#ifdef SC_LAMBDA_H

#define L_DECL1(op, rator, ty1, exp1) \
	friend inline sc_lambda_ptr operator op(ty1);

#define L_DECL1SPEC(op, ty1, code) \
	friend inline sc_lambda_ptr operator op(ty1);

#define L_DECL2(op, rator, ty1, ty2, exp1, exp2) \
	friend inline sc_lambda_ptr operator op(ty1, ty2);

#define L_DECL2SPEC(op, ty1, ty2, code) \
	friend inline sc_lambda_ptr operator op(ty1, ty2);

#ifdef L_DECL1

#ifndef NO_INTEGER_LAMBDA
	L_DECL2(== , SC_LAMBDA_INT_EQ, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_INT_EQ, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(== , SC_LAMBDA_INT_EQ, int, const sc_signal_int_deval&, s1, s2)

		L_DECL2(== , SC_LAMBDA_INT_EQ, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_INT_EQ, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)
#endif

		L_DECL2(== , SC_LAMBDA_BOOL_EQ, const sc_signal_edgy_deval&, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_BOOL_EQ, const sc_signal_edgy_deval&, bool, s1, s2)
		L_DECL2(== , SC_LAMBDA_BOOL_EQ, bool, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_BOOL_EQ, const sc_lambda_ptr&, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_BOOL_EQ, const sc_signal_edgy_deval&, const sc_lambda_ptr&, s1, s2)

		L_DECL2SPEC(== , const sc_lambda_ptr&, const sc_lambda_ptr&,
	if (s1->is_sc_logic()) {
		assert(s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_SUL_EQ,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else if (s1->is_bool()) {
		assert(s2->is_bool());
		return new sc_lambda(SC_LAMBDA_BOOL_EQ,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else {
		assert(s2->is_int());
		return new sc_lambda(SC_LAMBDA_INT_EQ,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

#ifndef NO_INTEGER_LAMBDA
		//abc
		L_DECL2SPEC(== , const sc_lambda_ptr&, int,
	if (s1->is_bool()) {
		return new sc_lambda(SC_LAMBDA_BOOL_EQ,
			new sc_lambda_rand(s1),
			new sc_lambda_rand((bool)(s2 & 1)));
	}
	else {
		assert(s1->is_int());
		return new sc_lambda(SC_LAMBDA_INT_EQ,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

		L_DECL2SPEC(== , int, const sc_lambda_ptr&,
	if (s2->is_bool()) {
		return new sc_lambda(SC_LAMBDA_BOOL_EQ,
			new sc_lambda_rand((bool)(s1 & 1)),
			new sc_lambda_rand(s2));
	}
	else {
		assert(s2->is_int());
		return new sc_lambda(SC_LAMBDA_INT_EQ,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)
#endif

		/*---------------------------------------------------------------------------*/

		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
		L_DECL2(== , SC_LAMBDA_SUL_EQ, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_lambda_ptr&, char, s1, sc_logic(s2))
		L_DECL2(== , SC_LAMBDA_SUL_EQ, char, const sc_lambda_ptr&, sc_logic(s1), s2)

		/*---------------------------------------------------------------------------*/

#if 0
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const signal_sl&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_lambda_ptr&, const signal_sl&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const signal_sl&, const signal_sl&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const signal_sl&, const sc_logic&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_logic&, const signal_sl&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const signal_sl&, char, s1, sc_logic(s2))
		L_DECL2(== , SC_LAMBDA_SUL_EQ, char, const signal_sl&, sc_logic(s1), s2)
#endif

		/*---------------------------------------------------------------------------*/

		// Overloading operator!=() clashes with STL.  Should look for a way around
		// this, since STL has become a standard library for ANSI C++

#ifndef NO_INTEGER_LAMBDA
		L_DECL2(!= , SC_LAMBDA_INT_NE, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_INT_NE, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(!= , SC_LAMBDA_INT_NE, int, const sc_signal_int_deval&, s1, s2)

		L_DECL2(!= , SC_LAMBDA_INT_NE, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_INT_NE, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)
#endif

		L_DECL2(!= , SC_LAMBDA_BOOL_NE, const sc_signal_edgy_deval&, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_BOOL_NE, const sc_signal_edgy_deval&, bool, s1, s2)
		L_DECL2(!= , SC_LAMBDA_BOOL_NE, bool, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_BOOL_NE, const sc_lambda_ptr&, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_BOOL_NE, const sc_signal_edgy_deval&, const sc_lambda_ptr&, s1, s2)

		L_DECL2SPEC(!= , const sc_lambda_ptr&, const sc_lambda_ptr&,
	if (s1->is_sc_logic()) {
		assert(s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_SUL_NE,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else if (s1->is_bool()) {
		assert(s2->is_bool());
		return new sc_lambda(SC_LAMBDA_BOOL_NE,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else {
		assert(s2->is_int());
		return new sc_lambda(SC_LAMBDA_INT_NE,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

#ifndef NO_INTEGER_LAMBDA
		L_DECL2SPEC(!= , const sc_lambda_ptr&, int,
	if (s1->is_bool()) {
		return new sc_lambda(SC_LAMBDA_BOOL_NE,
			new sc_lambda_rand(s1),
			new sc_lambda_rand((bool)(s2 & 1)));
	}
	else {
		assert(s1->is_int());
		return new sc_lambda(SC_LAMBDA_INT_NE,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

		L_DECL2SPEC(!= , int, const sc_lambda_ptr&,
	if (s2->is_bool()) {
		return new sc_lambda(SC_LAMBDA_BOOL_NE,
			new sc_lambda_rand((bool)(s1 & 1)),
			new sc_lambda_rand(s2));
	}
	else {
		assert(s2->is_int());
		return new sc_lambda(SC_LAMBDA_INT_NE,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)
#endif

		/*---------------------------------------------------------------------------*/

		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
		L_DECL2(!= , SC_LAMBDA_SUL_NE, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_lambda_ptr&, char, s1, sc_logic(s2))
		L_DECL2(!= , SC_LAMBDA_SUL_NE, char, const sc_lambda_ptr&, sc_logic(s1), s2)

		/*---------------------------------------------------------------------------*/

#if 0
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const signal_sl&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_lambda_ptr&, const signal_sl&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const signal_sl&, const signal_sl&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const signal_sl&, const sc_logic&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_logic&, const signal_sl&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const signal_sl&, char, s1, sc_logic(s2))
		L_DECL2(!= , SC_LAMBDA_SUL_NE, char, const signal_sl&, sc_logic(s1), s2)
#endif

		/*---------------------------------------------------------------------------*/
		/*---------------------------------------------------------------------------*/

#ifndef NO_INTEGER_LAMBDA

		L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(<, SC_LAMBDA_INT_LT, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(>, SC_LAMBDA_INT_GT, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(%, SC_LAMBDA_INT_REM, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/
		/*---------------------------------------------------------------------------*/

		L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(&, SC_LAMBDA_INT_BITAND, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(&, SC_LAMBDA_INT_BITAND, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

#endif

		L_DECL2SPEC(&, const sc_lambda_ptr&, const sc_lambda_ptr&,
	if (s1->is_sc_logic()) {
		assert(s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_SUL_BITAND,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else {
		assert(!s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_INT_BITAND,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

		/*---------------------------------------------------------------------------*/

		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)

		/*---------------------------------------------------------------------------*/

#if 0
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const signal_sl&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_lambda_ptr&, const signal_sl&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const signal_sl&, const signal_sl&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const signal_sl&, const sc_logic&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const signal_sl&, char, s1, sc_logic(s2))
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_logic&, const signal_sl&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, char, const signal_sl&, sc_logic(s1), s2)
#endif

		/*---------------------------------------------------------------------------*/
		/*---------------------------------------------------------------------------*/

#ifndef NO_INTEGER_LAMBDA
		L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(| , SC_LAMBDA_INT_BITOR, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(| , SC_LAMBDA_INT_BITOR, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)
#endif

		L_DECL2SPEC(| , const sc_lambda_ptr&, const sc_lambda_ptr&,
	if (s1->is_sc_logic()) {
		assert(s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_SUL_BITOR,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else {
		assert(!s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_INT_BITOR,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

		/*---------------------------------------------------------------------------*/

		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)

		/*---------------------------------------------------------------------------*/

#if 0
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const signal_sl&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_lambda_ptr&, const signal_sl&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const signal_sl&, const signal_sl&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const signal_sl&, const sc_logic&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const signal_sl&, char, s1, sc_logic(s2))
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_logic&, const signal_sl&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, char, const signal_sl&, sc_logic(s1), s2)
#endif

		/*---------------------------------------------------------------------------*/
		/*---------------------------------------------------------------------------*/

#ifndef NO_INTEGER_LAMBDA
		L_DECL1(~, SC_LAMBDA_INT_BITNOT, const sc_signal_int_deval&, s1)
#endif

		L_DECL1(~, SC_LAMBDA_SUL_BITNOT, const sc_signal_sc_logic_deval&, s1)
#if 0
		L_DECL1(~, SC_LAMBDA_SUL_BITNOT, const signal_sl&, s1)
#endif

		L_DECL1SPEC(~, const sc_lambda_ptr&,
	if (s1->is_sc_logic()) {
		return new sc_lambda(SC_LAMBDA_SUL_BITNOT,
			new sc_lambda_rand(s1));
	}
	else {
		return new sc_lambda(SC_LAMBDA_INT_BITNOT,
			new sc_lambda_rand(s1));
	}
	)

		/*---------------------------------------------------------------------------*/

#ifndef NO_INTEGER_LAMBDA
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)
#endif

		L_DECL2SPEC(^, const sc_lambda_ptr&, const sc_lambda_ptr&,
	if (s1->is_sc_logic()) {
		assert(s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_SUL_BITXOR,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else {
		assert(!s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_INT_BITXOR,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

		/*---------------------------------------------------------------------------*/

		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)

		/*---------------------------------------------------------------------------*/

#if 0
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const signal_sl&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_lambda_ptr&, const signal_sl&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const signal_sl&, const signal_sl&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const signal_sl&, const sc_logic&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const signal_sl&, char, s1, sc_logic(s2))
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_logic&, const signal_sl&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, char, const signal_sl&, sc_logic(s1), s2)
#endif

		/*---------------------------------------------------------------------------*/
		/*---------------------------------------------------------------------------*/

		L_DECL2(|| , SC_LAMBDA_OR, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(|| , SC_LAMBDA_OR, const sc_signal_edgy_deval&, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(|| , SC_LAMBDA_OR, const sc_signal_edgy_deval&, bool, s1, s2)
		L_DECL2(|| , SC_LAMBDA_OR, bool, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(|| , SC_LAMBDA_OR, const sc_signal_edgy_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(|| , SC_LAMBDA_OR, const sc_lambda_ptr&, const sc_signal_edgy_deval&, s1, s2)

		L_DECL2(&&, SC_LAMBDA_AND, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(&&, SC_LAMBDA_AND, const sc_signal_edgy_deval&, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(&&, SC_LAMBDA_AND, const sc_signal_edgy_deval&, bool, s1, s2)
		L_DECL2(&&, SC_LAMBDA_AND, bool, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(&&, SC_LAMBDA_AND, const sc_signal_edgy_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(&&, SC_LAMBDA_AND, const sc_lambda_ptr&, const sc_signal_edgy_deval&, s1, s2)

		L_DECL1(!, SC_LAMBDA_NOT, const sc_lambda_ptr&, s1)
		L_DECL1(!, SC_LAMBDA_NOT, const sc_signal_edgy_deval&, s1)

#endif

#undef L_DECL1
#undef L_DECL1SPEC
#undef L_DECL2
#undef L_DECL2SPEC

#endif

public:
	static void* operator new(size_t sz){ return sc_mempool::allocate(sz); }
	static void* operator new(size_t, void* p){ return p; }
	static void operator delete(void* p, size_t sz) { sc_mempool::release(p, sz); }

	// Constructors
	// <group>
	sc_lambda_ptr() : ptr(&sc_lambda::dummy_lambda)
	{
		ptr->ref_count++;
	}

	sc_lambda_ptr(const sc_lambda_ptr& p)
		: ptr(p.ptr)
	{
		// Update reference count
		ptr->ref_count++;
	}

	explicit sc_lambda_ptr(const sc_signal_edgy_deval& b)
		: ptr(new sc_lambda(reinterpret_cast<const sc_signal_edgy&>(b)))
	{
		ptr->ref_count++;
	}
	// </group>

	~sc_lambda_ptr()
	{
		if (--(ptr->ref_count) == 0)
			delete ptr;
	}

	// Assignment operator - simply copy the pointer member
	sc_lambda_ptr& operator=(const sc_lambda_ptr& p)
	{
		if (&p == this)
			return *this;
		// Update reference count on original lambda
		if (--(ptr->ref_count) == 0)
			delete ptr;
		ptr = p.ptr;
		// Update reference count
		ptr->ref_count++;
		return *this;
	}

	operator bool() const
	{
		// Provide this implicit type conversion so that the code
		// would still work even if the user makes a mistake, e.g.
		// if (foo == '1') as opposed to if (foo.read() == '1').
		// Should a warning be issued here?
		// cerr << "SystemC: warning: implicit conversion of lambda to bool\n";
		return ptr->eval();
	}

private:
	sc_lambda* ptr;
	sc_lambda_ptr(sc_lambda* p)
		: ptr(p)
	{
		// Update reference count
		ptr->ref_count++;
	}
	sc_lambda_ptr& operator=(sc_lambda* p)
	{
		// Update reference count on original lambda
		if (--(ptr->ref_count) == 0)
			delete ptr;
		ptr = p;
		ptr->ref_count++;
		return *this;
	}
	sc_lambda* operator->() const { return ptr; }
};

/*---------------------------------------------------------------------------*/

enum sc_lambda_rand_e
{
	SC_LAMBDA_RAND_LAMBDA,
	SC_LAMBDA_RAND_SIGNAL_INT,
	SC_LAMBDA_RAND_SIGNAL_SUL,
	SC_LAMBDA_RAND_SIGNAL_BOOL,
	SC_LAMBDA_RAND_INT,
	SC_LAMBDA_RAND_SUL,
	SC_LAMBDA_RAND_BOOL
};

class sc_lambda_rand
{
	friend class sc_lambda;
	friend class sc_port_manager;

public:
	static void* operator new(size_t sz){ return sc_mempool::allocate(sz); }
	static void operator delete(void* p, size_t sz) { sc_mempool::release(p, sz); }

#ifdef SC_LAMBDA_H

#define L_DECL1(op, rator, ty1, exp1) \
	friend inline sc_lambda_ptr operator op(ty1);

#define L_DECL1SPEC(op, ty1, code) \
	friend inline sc_lambda_ptr operator op(ty1);

#define L_DECL2(op, rator, ty1, ty2, exp1, exp2) \
	friend inline sc_lambda_ptr operator op(ty1, ty2);

#define L_DECL2SPEC(op, ty1, ty2, code) \
	friend inline sc_lambda_ptr operator op(ty1, ty2);

#ifdef L_DECL1

#ifndef NO_INTEGER_LAMBDA
	L_DECL2(== , SC_LAMBDA_INT_EQ, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_INT_EQ, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(== , SC_LAMBDA_INT_EQ, int, const sc_signal_int_deval&, s1, s2)

		L_DECL2(== , SC_LAMBDA_INT_EQ, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_INT_EQ, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)
#endif

		L_DECL2(== , SC_LAMBDA_BOOL_EQ, const sc_signal_edgy_deval&, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_BOOL_EQ, const sc_signal_edgy_deval&, bool, s1, s2)
		L_DECL2(== , SC_LAMBDA_BOOL_EQ, bool, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_BOOL_EQ, const sc_lambda_ptr&, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_BOOL_EQ, const sc_signal_edgy_deval&, const sc_lambda_ptr&, s1, s2)

		L_DECL2SPEC(== , const sc_lambda_ptr&, const sc_lambda_ptr&,
	if (s1->is_sc_logic()) {
		assert(s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_SUL_EQ,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else if (s1->is_bool()) {
		assert(s2->is_bool());
		return new sc_lambda(SC_LAMBDA_BOOL_EQ,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else {
		assert(s2->is_int());
		return new sc_lambda(SC_LAMBDA_INT_EQ,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

#ifndef NO_INTEGER_LAMBDA
		//abc
		L_DECL2SPEC(== , const sc_lambda_ptr&, int,
	if (s1->is_bool()) {
		return new sc_lambda(SC_LAMBDA_BOOL_EQ,
			new sc_lambda_rand(s1),
			new sc_lambda_rand((bool)(s2 & 1)));
	}
	else {
		assert(s1->is_int());
		return new sc_lambda(SC_LAMBDA_INT_EQ,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

		L_DECL2SPEC(== , int, const sc_lambda_ptr&,
	if (s2->is_bool()) {
		return new sc_lambda(SC_LAMBDA_BOOL_EQ,
			new sc_lambda_rand((bool)(s1 & 1)),
			new sc_lambda_rand(s2));
	}
	else {
		assert(s2->is_int());
		return new sc_lambda(SC_LAMBDA_INT_EQ,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)
#endif

		/*---------------------------------------------------------------------------*/

		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
		L_DECL2(== , SC_LAMBDA_SUL_EQ, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_lambda_ptr&, char, s1, sc_logic(s2))
		L_DECL2(== , SC_LAMBDA_SUL_EQ, char, const sc_lambda_ptr&, sc_logic(s1), s2)

		/*---------------------------------------------------------------------------*/

#if 0
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const signal_sl&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_lambda_ptr&, const signal_sl&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const signal_sl&, const signal_sl&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const signal_sl&, const sc_logic&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_logic&, const signal_sl&, s1, s2)
		L_DECL2(== , SC_LAMBDA_SUL_EQ, const signal_sl&, char, s1, sc_logic(s2))
		L_DECL2(== , SC_LAMBDA_SUL_EQ, char, const signal_sl&, sc_logic(s1), s2)
#endif

		/*---------------------------------------------------------------------------*/

		// Overloading operator!=() clashes with STL.  Should look for a way around
		// this, since STL has become a standard library for ANSI C++

#ifndef NO_INTEGER_LAMBDA
		L_DECL2(!= , SC_LAMBDA_INT_NE, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_INT_NE, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(!= , SC_LAMBDA_INT_NE, int, const sc_signal_int_deval&, s1, s2)

		L_DECL2(!= , SC_LAMBDA_INT_NE, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_INT_NE, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)
#endif

		L_DECL2(!= , SC_LAMBDA_BOOL_NE, const sc_signal_edgy_deval&, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_BOOL_NE, const sc_signal_edgy_deval&, bool, s1, s2)
		L_DECL2(!= , SC_LAMBDA_BOOL_NE, bool, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_BOOL_NE, const sc_lambda_ptr&, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_BOOL_NE, const sc_signal_edgy_deval&, const sc_lambda_ptr&, s1, s2)

		L_DECL2SPEC(!= , const sc_lambda_ptr&, const sc_lambda_ptr&,
	if (s1->is_sc_logic()) {
		assert(s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_SUL_NE,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else if (s1->is_bool()) {
		assert(s2->is_bool());
		return new sc_lambda(SC_LAMBDA_BOOL_NE,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else {
		assert(s2->is_int());
		return new sc_lambda(SC_LAMBDA_INT_NE,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

#ifndef NO_INTEGER_LAMBDA
		L_DECL2SPEC(!= , const sc_lambda_ptr&, int,
	if (s1->is_bool()) {
		return new sc_lambda(SC_LAMBDA_BOOL_NE,
			new sc_lambda_rand(s1),
			new sc_lambda_rand((bool)(s2 & 1)));
	}
	else {
		assert(s1->is_int());
		return new sc_lambda(SC_LAMBDA_INT_NE,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

		L_DECL2SPEC(!= , int, const sc_lambda_ptr&,
	if (s2->is_bool()) {
		return new sc_lambda(SC_LAMBDA_BOOL_NE,
			new sc_lambda_rand((bool)(s1 & 1)),
			new sc_lambda_rand(s2));
	}
	else {
		assert(s2->is_int());
		return new sc_lambda(SC_LAMBDA_INT_NE,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)
#endif

		/*---------------------------------------------------------------------------*/

		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
		L_DECL2(!= , SC_LAMBDA_SUL_NE, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_lambda_ptr&, char, s1, sc_logic(s2))
		L_DECL2(!= , SC_LAMBDA_SUL_NE, char, const sc_lambda_ptr&, sc_logic(s1), s2)

		/*---------------------------------------------------------------------------*/

#if 0
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const signal_sl&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_lambda_ptr&, const signal_sl&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const signal_sl&, const signal_sl&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const signal_sl&, const sc_logic&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_logic&, const signal_sl&, s1, s2)
		L_DECL2(!= , SC_LAMBDA_SUL_NE, const signal_sl&, char, s1, sc_logic(s2))
		L_DECL2(!= , SC_LAMBDA_SUL_NE, char, const signal_sl&, sc_logic(s1), s2)
#endif

		/*---------------------------------------------------------------------------*/
		/*---------------------------------------------------------------------------*/

#ifndef NO_INTEGER_LAMBDA

		L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(<, SC_LAMBDA_INT_LT, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(<, SC_LAMBDA_INT_LT, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(>, SC_LAMBDA_INT_GT, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(>, SC_LAMBDA_INT_GT, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/

		L_DECL2(%, SC_LAMBDA_INT_REM, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(%, SC_LAMBDA_INT_REM, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

		/*---------------------------------------------------------------------------*/
		/*---------------------------------------------------------------------------*/

		L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(&, SC_LAMBDA_INT_BITAND, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(&, SC_LAMBDA_INT_BITAND, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

#endif

		L_DECL2SPEC(&, const sc_lambda_ptr&, const sc_lambda_ptr&,
	if (s1->is_sc_logic()) {
		assert(s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_SUL_BITAND,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else {
		assert(!s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_INT_BITAND,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

		/*---------------------------------------------------------------------------*/

		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)

		/*---------------------------------------------------------------------------*/

#if 0
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const signal_sl&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_lambda_ptr&, const signal_sl&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const signal_sl&, const signal_sl&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const signal_sl&, const sc_logic&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const signal_sl&, char, s1, sc_logic(s2))
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_logic&, const signal_sl&, s1, s2)
		L_DECL2(&, SC_LAMBDA_SUL_BITAND, char, const signal_sl&, sc_logic(s1), s2)
#endif

		/*---------------------------------------------------------------------------*/
		/*---------------------------------------------------------------------------*/

#ifndef NO_INTEGER_LAMBDA
		L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(| , SC_LAMBDA_INT_BITOR, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(| , SC_LAMBDA_INT_BITOR, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)
#endif

		L_DECL2SPEC(| , const sc_lambda_ptr&, const sc_lambda_ptr&,
	if (s1->is_sc_logic()) {
		assert(s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_SUL_BITOR,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else {
		assert(!s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_INT_BITOR,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

		/*---------------------------------------------------------------------------*/

		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)

		/*---------------------------------------------------------------------------*/

#if 0
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const signal_sl&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_lambda_ptr&, const signal_sl&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const signal_sl&, const signal_sl&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const signal_sl&, const sc_logic&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const signal_sl&, char, s1, sc_logic(s2))
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_logic&, const signal_sl&, s1, s2)
		L_DECL2(| , SC_LAMBDA_SUL_BITOR, char, const signal_sl&, sc_logic(s1), s2)
#endif

		/*---------------------------------------------------------------------------*/
		/*---------------------------------------------------------------------------*/

#ifndef NO_INTEGER_LAMBDA
		L_DECL1(~, SC_LAMBDA_INT_BITNOT, const sc_signal_int_deval&, s1)
#endif

		L_DECL1(~, SC_LAMBDA_SUL_BITNOT, const sc_signal_sc_logic_deval&, s1)
#if 0
		L_DECL1(~, SC_LAMBDA_SUL_BITNOT, const signal_sl&, s1)
#endif

		L_DECL1SPEC(~, const sc_lambda_ptr&,
	if (s1->is_sc_logic()) {
		return new sc_lambda(SC_LAMBDA_SUL_BITNOT,
			new sc_lambda_rand(s1));
	}
	else {
		return new sc_lambda(SC_LAMBDA_INT_BITNOT,
			new sc_lambda_rand(s1));
	}
	)

		/*---------------------------------------------------------------------------*/

#ifndef NO_INTEGER_LAMBDA
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_signal_int_deval&, int, s1, s2)
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, int, const sc_signal_int_deval&, s1, s2)
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_lambda_ptr&, int, s1, s2)
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, int, const sc_lambda_ptr&, s1, s2)
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
		L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)
#endif

		L_DECL2SPEC(^, const sc_lambda_ptr&, const sc_lambda_ptr&,
	if (s1->is_sc_logic()) {
		assert(s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_SUL_BITXOR,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	else {
		assert(!s2->is_sc_logic());
		return new sc_lambda(SC_LAMBDA_INT_BITXOR,
			new sc_lambda_rand(s1),
			new sc_lambda_rand(s2));
	}
	)

		/*---------------------------------------------------------------------------*/

		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)

		/*---------------------------------------------------------------------------*/

#if 0
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const signal_sl&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_lambda_ptr&, const signal_sl&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const signal_sl&, const signal_sl&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const signal_sl&, const sc_logic&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const signal_sl&, char, s1, sc_logic(s2))
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_logic&, const signal_sl&, s1, s2)
		L_DECL2(^, SC_LAMBDA_SUL_BITXOR, char, const signal_sl&, sc_logic(s1), s2)
#endif

		/*---------------------------------------------------------------------------*/
		/*---------------------------------------------------------------------------*/

		L_DECL2(|| , SC_LAMBDA_OR, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(|| , SC_LAMBDA_OR, const sc_signal_edgy_deval&, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(|| , SC_LAMBDA_OR, const sc_signal_edgy_deval&, bool, s1, s2)
		L_DECL2(|| , SC_LAMBDA_OR, bool, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(|| , SC_LAMBDA_OR, const sc_signal_edgy_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(|| , SC_LAMBDA_OR, const sc_lambda_ptr&, const sc_signal_edgy_deval&, s1, s2)

		L_DECL2(&&, SC_LAMBDA_AND, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(&&, SC_LAMBDA_AND, const sc_signal_edgy_deval&, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(&&, SC_LAMBDA_AND, const sc_signal_edgy_deval&, bool, s1, s2)
		L_DECL2(&&, SC_LAMBDA_AND, bool, const sc_signal_edgy_deval&, s1, s2)
		L_DECL2(&&, SC_LAMBDA_AND, const sc_signal_edgy_deval&, const sc_lambda_ptr&, s1, s2)
		L_DECL2(&&, SC_LAMBDA_AND, const sc_lambda_ptr&, const sc_signal_edgy_deval&, s1, s2)

		L_DECL1(!, SC_LAMBDA_NOT, const sc_lambda_ptr&, s1)
		L_DECL1(!, SC_LAMBDA_NOT, const sc_signal_edgy_deval&, s1)

#endif

#undef L_DECL1
#undef L_DECL1SPEC
#undef L_DECL2
#undef L_DECL2SPEC

#endif

private:
	sc_lambda_rand_e rand_ty;
	union {
		char lamb_space[sizeof(sc_lambda_ptr)];
		char ch_space[sizeof(sc_logic)];
		const sc_signal<sc_logic>* sul_sig;
		const sc_signal<int>* int_sig;
		const sc_signal_edgy* edgy_sig;
		int val;
	};

	sc_lambda_rand(int i)
		: rand_ty(SC_LAMBDA_RAND_INT)
	{
		val = i;
	}
	sc_lambda_rand(bool b)
		: rand_ty(SC_LAMBDA_RAND_BOOL)
	{
		val = int(b);
	}
	sc_lambda_rand(const sc_logic& c)
		: rand_ty(SC_LAMBDA_RAND_SUL)
	{
		(void) new(ch_space)sc_logic(c);
	}
	sc_lambda_rand(const sc_signal_sc_logic_deval& s)
		: rand_ty(SC_LAMBDA_RAND_SIGNAL_SUL)
	{
		sul_sig = (const sc_signal<sc_logic>*) ((const void*)&s);
	}
	sc_lambda_rand(const sc_signal<int>& s)
		: rand_ty(SC_LAMBDA_RAND_SIGNAL_INT)
	{
		int_sig = &s;
	}
	sc_lambda_rand(const sc_signal_edgy& s)
		: rand_ty(SC_LAMBDA_RAND_SIGNAL_BOOL)
	{
		edgy_sig = &s;
	}
	sc_lambda_rand(const sc_signal_edgy_deval& s)
		: rand_ty(SC_LAMBDA_RAND_SIGNAL_BOOL)
	{
		edgy_sig = reinterpret_cast<const sc_signal_edgy*>(&s);
	}

	sc_lambda_rand(const sc_lambda_ptr& l)
		: rand_ty(SC_LAMBDA_RAND_LAMBDA)
	{
		(void) new(lamb_space)sc_lambda_ptr(l);
	}

	~sc_lambda_rand();

	int int_read() const;
	sc_logic sc_logic_read() const;
	bool bool_read() const;

	void replace_ports(void(*fn)(sc_port_manager*, sc_lambda_rand*),
		sc_port_manager*);
};

/*---------------------------------------------------------------------------*/

#ifndef SC_LAMBDA_DEFS_H
#define SC_LAMBDA_DEFS_H

#define L_DECL1(op, rator, ty1, exp1) \
	inline sc_lambda_ptr \
	operator op(ty1 s1) \
{ \
	return new sc_lambda(rator, \
	new sc_lambda_rand(exp1)); \
}

#define L_DECL1SPEC(op, ty1, code) \
	inline sc_lambda_ptr \
	operator op(ty1 s1) \
{ \
	code \
}

#define L_DECL2(op, rator, ty1, ty2, exp1, exp2) \
	inline sc_lambda_ptr \
	operator op(ty1 s1, ty2 s2) \
{ \
	return new sc_lambda(rator, \
	new sc_lambda_rand(exp1), \
	new sc_lambda_rand(exp2)); \
}

#define L_DECL2SPEC(op, ty1, ty2, code) \
	inline sc_lambda_ptr \
	operator op(ty1 s1, ty2 s2) \
{ \
	code \
}

#ifdef L_DECL1

#ifndef NO_INTEGER_LAMBDA
L_DECL2(== , SC_LAMBDA_INT_EQ, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
L_DECL2(== , SC_LAMBDA_INT_EQ, const sc_signal_int_deval&, int, s1, s2)
L_DECL2(== , SC_LAMBDA_INT_EQ, int, const sc_signal_int_deval&, s1, s2)

L_DECL2(== , SC_LAMBDA_INT_EQ, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
L_DECL2(== , SC_LAMBDA_INT_EQ, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)
#endif

L_DECL2(== , SC_LAMBDA_BOOL_EQ, const sc_signal_edgy_deval&, const sc_signal_edgy_deval&, s1, s2)
L_DECL2(== , SC_LAMBDA_BOOL_EQ, const sc_signal_edgy_deval&, bool, s1, s2)
L_DECL2(== , SC_LAMBDA_BOOL_EQ, bool, const sc_signal_edgy_deval&, s1, s2)
L_DECL2(== , SC_LAMBDA_BOOL_EQ, const sc_lambda_ptr&, const sc_signal_edgy_deval&, s1, s2)
L_DECL2(== , SC_LAMBDA_BOOL_EQ, const sc_signal_edgy_deval&, const sc_lambda_ptr&, s1, s2)

L_DECL2SPEC(== , const sc_lambda_ptr&, const sc_lambda_ptr&,
if (s1->is_sc_logic()) {
	assert(s2->is_sc_logic());
	return new sc_lambda(SC_LAMBDA_SUL_EQ,
		new sc_lambda_rand(s1),
		new sc_lambda_rand(s2));
}
else if (s1->is_bool()) {
	assert(s2->is_bool());
	return new sc_lambda(SC_LAMBDA_BOOL_EQ,
		new sc_lambda_rand(s1),
		new sc_lambda_rand(s2));
}
else {
	assert(s2->is_int());
	return new sc_lambda(SC_LAMBDA_INT_EQ,
		new sc_lambda_rand(s1),
		new sc_lambda_rand(s2));
}
)

#ifndef NO_INTEGER_LAMBDA
//abc
L_DECL2SPEC(== , const sc_lambda_ptr&, int,
if (s1->is_bool()) {
	return new sc_lambda(SC_LAMBDA_BOOL_EQ,
		new sc_lambda_rand(s1),
		new sc_lambda_rand((bool)(s2 & 1)));
}
else {
	assert(s1->is_int());
	return new sc_lambda(SC_LAMBDA_INT_EQ,
		new sc_lambda_rand(s1),
		new sc_lambda_rand(s2));
}
)

L_DECL2SPEC(== , int, const sc_lambda_ptr&,
if (s2->is_bool()) {
	return new sc_lambda(SC_LAMBDA_BOOL_EQ,
		new sc_lambda_rand((bool)(s1 & 1)),
		new sc_lambda_rand(s2));
}
else {
	assert(s2->is_int());
	return new sc_lambda(SC_LAMBDA_INT_EQ,
		new sc_lambda_rand(s1),
		new sc_lambda_rand(s2));
}
)
#endif

/*---------------------------------------------------------------------------*/

L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
L_DECL2(== , SC_LAMBDA_SUL_EQ, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)
L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_lambda_ptr&, char, s1, sc_logic(s2))
L_DECL2(== , SC_LAMBDA_SUL_EQ, char, const sc_lambda_ptr&, sc_logic(s1), s2)

/*---------------------------------------------------------------------------*/

#if 0
L_DECL2(== , SC_LAMBDA_SUL_EQ, const signal_sl&, const sc_lambda_ptr&, s1, s2)
L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_lambda_ptr&, const signal_sl&, s1, s2)
L_DECL2(== , SC_LAMBDA_SUL_EQ, const signal_sl&, const signal_sl&, s1, s2)
L_DECL2(== , SC_LAMBDA_SUL_EQ, const signal_sl&, const sc_logic&, s1, s2)
L_DECL2(== , SC_LAMBDA_SUL_EQ, const sc_logic&, const signal_sl&, s1, s2)
L_DECL2(== , SC_LAMBDA_SUL_EQ, const signal_sl&, char, s1, sc_logic(s2))
L_DECL2(== , SC_LAMBDA_SUL_EQ, char, const signal_sl&, sc_logic(s1), s2)
#endif

/*---------------------------------------------------------------------------*/

// Overloading operator!=() clashes with STL.  Should look for a way around
// this, since STL has become a standard library for ANSI C++

#ifndef NO_INTEGER_LAMBDA
L_DECL2(!= , SC_LAMBDA_INT_NE, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
L_DECL2(!= , SC_LAMBDA_INT_NE, const sc_signal_int_deval&, int, s1, s2)
L_DECL2(!= , SC_LAMBDA_INT_NE, int, const sc_signal_int_deval&, s1, s2)

L_DECL2(!= , SC_LAMBDA_INT_NE, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
L_DECL2(!= , SC_LAMBDA_INT_NE, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)
#endif

L_DECL2(!= , SC_LAMBDA_BOOL_NE, const sc_signal_edgy_deval&, const sc_signal_edgy_deval&, s1, s2)
L_DECL2(!= , SC_LAMBDA_BOOL_NE, const sc_signal_edgy_deval&, bool, s1, s2)
L_DECL2(!= , SC_LAMBDA_BOOL_NE, bool, const sc_signal_edgy_deval&, s1, s2)
L_DECL2(!= , SC_LAMBDA_BOOL_NE, const sc_lambda_ptr&, const sc_signal_edgy_deval&, s1, s2)
L_DECL2(!= , SC_LAMBDA_BOOL_NE, const sc_signal_edgy_deval&, const sc_lambda_ptr&, s1, s2)

L_DECL2SPEC(!= , const sc_lambda_ptr&, const sc_lambda_ptr&,
if (s1->is_sc_logic()) {
	assert(s2->is_sc_logic());
	return new sc_lambda(SC_LAMBDA_SUL_NE,
		new sc_lambda_rand(s1),
		new sc_lambda_rand(s2));
}
else if (s1->is_bool()) {
	assert(s2->is_bool());
	return new sc_lambda(SC_LAMBDA_BOOL_NE,
		new sc_lambda_rand(s1),
		new sc_lambda_rand(s2));
}
else {
	assert(s2->is_int());
	return new sc_lambda(SC_LAMBDA_INT_NE,
		new sc_lambda_rand(s1),
		new sc_lambda_rand(s2));
}
)

#ifndef NO_INTEGER_LAMBDA
L_DECL2SPEC(!= , const sc_lambda_ptr&, int,
if (s1->is_bool()) {
	return new sc_lambda(SC_LAMBDA_BOOL_NE,
		new sc_lambda_rand(s1),
		new sc_lambda_rand((bool)(s2 & 1)));
}
else {
	assert(s1->is_int());
	return new sc_lambda(SC_LAMBDA_INT_NE,
		new sc_lambda_rand(s1),
		new sc_lambda_rand(s2));
}
)

L_DECL2SPEC(!= , int, const sc_lambda_ptr&,
if (s2->is_bool()) {
	return new sc_lambda(SC_LAMBDA_BOOL_NE,
		new sc_lambda_rand((bool)(s1 & 1)),
		new sc_lambda_rand(s2));
}
else {
	assert(s2->is_int());
	return new sc_lambda(SC_LAMBDA_INT_NE,
		new sc_lambda_rand(s1),
		new sc_lambda_rand(s2));
}
)
#endif

/*---------------------------------------------------------------------------*/

L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
L_DECL2(!= , SC_LAMBDA_SUL_NE, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)
L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_lambda_ptr&, char, s1, sc_logic(s2))
L_DECL2(!= , SC_LAMBDA_SUL_NE, char, const sc_lambda_ptr&, sc_logic(s1), s2)

/*---------------------------------------------------------------------------*/

#if 0
L_DECL2(!= , SC_LAMBDA_SUL_NE, const signal_sl&, const sc_lambda_ptr&, s1, s2)
L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_lambda_ptr&, const signal_sl&, s1, s2)
L_DECL2(!= , SC_LAMBDA_SUL_NE, const signal_sl&, const signal_sl&, s1, s2)
L_DECL2(!= , SC_LAMBDA_SUL_NE, const signal_sl&, const sc_logic&, s1, s2)
L_DECL2(!= , SC_LAMBDA_SUL_NE, const sc_logic&, const signal_sl&, s1, s2)
L_DECL2(!= , SC_LAMBDA_SUL_NE, const signal_sl&, char, s1, sc_logic(s2))
L_DECL2(!= , SC_LAMBDA_SUL_NE, char, const signal_sl&, sc_logic(s1), s2)
#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef NO_INTEGER_LAMBDA

L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_signal_int_deval&, int, s1, s2)
L_DECL2(<= , SC_LAMBDA_INT_LE, int, const sc_signal_int_deval&, s1, s2)
L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_lambda_ptr&, int, s1, s2)
L_DECL2(<= , SC_LAMBDA_INT_LE, int, const sc_lambda_ptr&, s1, s2)
L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
L_DECL2(<= , SC_LAMBDA_INT_LE, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

/*---------------------------------------------------------------------------*/

L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_signal_int_deval&, int, s1, s2)
L_DECL2(>= , SC_LAMBDA_INT_GE, int, const sc_signal_int_deval&, s1, s2)
L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_lambda_ptr&, int, s1, s2)
L_DECL2(>= , SC_LAMBDA_INT_GE, int, const sc_lambda_ptr&, s1, s2)
L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
L_DECL2(>= , SC_LAMBDA_INT_GE, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

/*---------------------------------------------------------------------------*/

L_DECL2(<, SC_LAMBDA_INT_LT, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
L_DECL2(<, SC_LAMBDA_INT_LT, const sc_signal_int_deval&, int, s1, s2)
L_DECL2(<, SC_LAMBDA_INT_LT, int, const sc_signal_int_deval&, s1, s2)
L_DECL2(<, SC_LAMBDA_INT_LT, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
L_DECL2(<, SC_LAMBDA_INT_LT, const sc_lambda_ptr&, int, s1, s2)
L_DECL2(<, SC_LAMBDA_INT_LT, int, const sc_lambda_ptr&, s1, s2)
L_DECL2(<, SC_LAMBDA_INT_LT, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
L_DECL2(<, SC_LAMBDA_INT_LT, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

/*---------------------------------------------------------------------------*/

L_DECL2(>, SC_LAMBDA_INT_GT, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
L_DECL2(>, SC_LAMBDA_INT_GT, const sc_signal_int_deval&, int, s1, s2)
L_DECL2(>, SC_LAMBDA_INT_GT, int, const sc_signal_int_deval&, s1, s2)
L_DECL2(>, SC_LAMBDA_INT_GT, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
L_DECL2(>, SC_LAMBDA_INT_GT, const sc_lambda_ptr&, int, s1, s2)
L_DECL2(>, SC_LAMBDA_INT_GT, int, const sc_lambda_ptr&, s1, s2)
L_DECL2(>, SC_LAMBDA_INT_GT, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
L_DECL2(>, SC_LAMBDA_INT_GT, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

/*---------------------------------------------------------------------------*/

L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_signal_int_deval&, int, s1, s2)
L_DECL2(+, SC_LAMBDA_INT_ADD, int, const sc_signal_int_deval&, s1, s2)
L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_lambda_ptr&, int, s1, s2)
L_DECL2(+, SC_LAMBDA_INT_ADD, int, const sc_lambda_ptr&, s1, s2)
L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
L_DECL2(+, SC_LAMBDA_INT_ADD, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

/*---------------------------------------------------------------------------*/

L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_signal_int_deval&, int, s1, s2)
L_DECL2(-, SC_LAMBDA_INT_SUB, int, const sc_signal_int_deval&, s1, s2)
L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_lambda_ptr&, int, s1, s2)
L_DECL2(-, SC_LAMBDA_INT_SUB, int, const sc_lambda_ptr&, s1, s2)
L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
L_DECL2(-, SC_LAMBDA_INT_SUB, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

/*---------------------------------------------------------------------------*/

L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_signal_int_deval&, int, s1, s2)
L_DECL2(*, SC_LAMBDA_INT_MUL, int, const sc_signal_int_deval&, s1, s2)
L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_lambda_ptr&, int, s1, s2)
L_DECL2(*, SC_LAMBDA_INT_MUL, int, const sc_lambda_ptr&, s1, s2)
L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
L_DECL2(*, SC_LAMBDA_INT_MUL, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

/*---------------------------------------------------------------------------*/

L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_signal_int_deval&, int, s1, s2)
L_DECL2(/ , SC_LAMBDA_INT_DIV, int, const sc_signal_int_deval&, s1, s2)
L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_lambda_ptr&, int, s1, s2)
L_DECL2(/ , SC_LAMBDA_INT_DIV, int, const sc_lambda_ptr&, s1, s2)
L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
L_DECL2(/ , SC_LAMBDA_INT_DIV, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

/*---------------------------------------------------------------------------*/

L_DECL2(%, SC_LAMBDA_INT_REM, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
L_DECL2(%, SC_LAMBDA_INT_REM, const sc_signal_int_deval&, int, s1, s2)
L_DECL2(%, SC_LAMBDA_INT_REM, int, const sc_signal_int_deval&, s1, s2)
L_DECL2(%, SC_LAMBDA_INT_REM, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
L_DECL2(%, SC_LAMBDA_INT_REM, const sc_lambda_ptr&, int, s1, s2)
L_DECL2(%, SC_LAMBDA_INT_REM, int, const sc_lambda_ptr&, s1, s2)
L_DECL2(%, SC_LAMBDA_INT_REM, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
L_DECL2(%, SC_LAMBDA_INT_REM, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_signal_int_deval&, int, s1, s2)
L_DECL2(&, SC_LAMBDA_INT_BITAND, int, const sc_signal_int_deval&, s1, s2)
L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_lambda_ptr&, int, s1, s2)
L_DECL2(&, SC_LAMBDA_INT_BITAND, int, const sc_lambda_ptr&, s1, s2)
L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
L_DECL2(&, SC_LAMBDA_INT_BITAND, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)

#endif

L_DECL2SPEC(&, const sc_lambda_ptr&, const sc_lambda_ptr&,
if (s1->is_sc_logic()) {
	assert(s2->is_sc_logic());
	return new sc_lambda(SC_LAMBDA_SUL_BITAND,
		new sc_lambda_rand(s1),
		new sc_lambda_rand(s2));
}
else {
	assert(!s2->is_sc_logic());
	return new sc_lambda(SC_LAMBDA_INT_BITAND,
		new sc_lambda_rand(s1),
		new sc_lambda_rand(s2));
}
)

/*---------------------------------------------------------------------------*/

L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
L_DECL2(&, SC_LAMBDA_SUL_BITAND, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)

/*---------------------------------------------------------------------------*/

#if 0
L_DECL2(&, SC_LAMBDA_SUL_BITAND, const signal_sl&, const sc_lambda_ptr&, s1, s2)
L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_lambda_ptr&, const signal_sl&, s1, s2)
L_DECL2(&, SC_LAMBDA_SUL_BITAND, const signal_sl&, const signal_sl&, s1, s2)
L_DECL2(&, SC_LAMBDA_SUL_BITAND, const signal_sl&, const sc_logic&, s1, s2)
L_DECL2(&, SC_LAMBDA_SUL_BITAND, const signal_sl&, char, s1, sc_logic(s2))
L_DECL2(&, SC_LAMBDA_SUL_BITAND, const sc_logic&, const signal_sl&, s1, s2)
L_DECL2(&, SC_LAMBDA_SUL_BITAND, char, const signal_sl&, sc_logic(s1), s2)
#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef NO_INTEGER_LAMBDA
L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_signal_int_deval&, int, s1, s2)
L_DECL2(| , SC_LAMBDA_INT_BITOR, int, const sc_signal_int_deval&, s1, s2)
L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_lambda_ptr&, int, s1, s2)
L_DECL2(| , SC_LAMBDA_INT_BITOR, int, const sc_lambda_ptr&, s1, s2)
L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
L_DECL2(| , SC_LAMBDA_INT_BITOR, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)
#endif

L_DECL2SPEC(| , const sc_lambda_ptr&, const sc_lambda_ptr&,
if (s1->is_sc_logic()) {
	assert(s2->is_sc_logic());
	return new sc_lambda(SC_LAMBDA_SUL_BITOR,
		new sc_lambda_rand(s1),
		new sc_lambda_rand(s2));
}
else {
	assert(!s2->is_sc_logic());
	return new sc_lambda(SC_LAMBDA_INT_BITOR,
		new sc_lambda_rand(s1),
		new sc_lambda_rand(s2));
}
)

/*---------------------------------------------------------------------------*/

L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
L_DECL2(| , SC_LAMBDA_SUL_BITOR, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)

/*---------------------------------------------------------------------------*/

#if 0
L_DECL2(| , SC_LAMBDA_SUL_BITOR, const signal_sl&, const sc_lambda_ptr&, s1, s2)
L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_lambda_ptr&, const signal_sl&, s1, s2)
L_DECL2(| , SC_LAMBDA_SUL_BITOR, const signal_sl&, const signal_sl&, s1, s2)
L_DECL2(| , SC_LAMBDA_SUL_BITOR, const signal_sl&, const sc_logic&, s1, s2)
L_DECL2(| , SC_LAMBDA_SUL_BITOR, const signal_sl&, char, s1, sc_logic(s2))
L_DECL2(| , SC_LAMBDA_SUL_BITOR, const sc_logic&, const signal_sl&, s1, s2)
L_DECL2(| , SC_LAMBDA_SUL_BITOR, char, const signal_sl&, sc_logic(s1), s2)
#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef NO_INTEGER_LAMBDA
L_DECL1(~, SC_LAMBDA_INT_BITNOT, const sc_signal_int_deval&, s1)
#endif

L_DECL1(~, SC_LAMBDA_SUL_BITNOT, const sc_signal_sc_logic_deval&, s1)
#if 0
L_DECL1(~, SC_LAMBDA_SUL_BITNOT, const signal_sl&, s1)
#endif

L_DECL1SPEC(~, const sc_lambda_ptr&,
if (s1->is_sc_logic()) {
	return new sc_lambda(SC_LAMBDA_SUL_BITNOT,
		new sc_lambda_rand(s1));
}
else {
	return new sc_lambda(SC_LAMBDA_INT_BITNOT,
		new sc_lambda_rand(s1));
}
)

/*---------------------------------------------------------------------------*/

#ifndef NO_INTEGER_LAMBDA
L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_signal_int_deval&, const sc_signal_int_deval&, s1, s2)
L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_signal_int_deval&, int, s1, s2)
L_DECL2(^, SC_LAMBDA_INT_BITXOR, int, const sc_signal_int_deval&, s1, s2)
L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_lambda_ptr&, int, s1, s2)
L_DECL2(^, SC_LAMBDA_INT_BITXOR, int, const sc_lambda_ptr&, s1, s2)
L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_lambda_ptr&, const sc_signal_int_deval&, s1, s2)
L_DECL2(^, SC_LAMBDA_INT_BITXOR, const sc_signal_int_deval&, const sc_lambda_ptr&, s1, s2)
#endif

L_DECL2SPEC(^, const sc_lambda_ptr&, const sc_lambda_ptr&,
if (s1->is_sc_logic()) {
	assert(s2->is_sc_logic());
	return new sc_lambda(SC_LAMBDA_SUL_BITXOR,
		new sc_lambda_rand(s1),
		new sc_lambda_rand(s2));
}
else {
	assert(!s2->is_sc_logic());
	return new sc_lambda(SC_LAMBDA_INT_BITXOR,
		new sc_lambda_rand(s1),
		new sc_lambda_rand(s2));
}
)

/*---------------------------------------------------------------------------*/

L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_signal_sc_logic_deval&, const sc_lambda_ptr&, s1, s2)
L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_lambda_ptr&, const sc_signal_sc_logic_deval&, s1, s2)
L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_signal_sc_logic_deval&, const sc_signal_sc_logic_deval&, s1, s2)
L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_signal_sc_logic_deval&, const sc_logic&, s1, s2)
L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_signal_sc_logic_deval&, char, s1, sc_logic(s2))
L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_logic&, const sc_signal_sc_logic_deval&, s1, s2)
L_DECL2(^, SC_LAMBDA_SUL_BITXOR, char, const sc_signal_sc_logic_deval&, sc_logic(s1), s2)

/*---------------------------------------------------------------------------*/

#if 0
L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const signal_sl&, const sc_lambda_ptr&, s1, s2)
L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_lambda_ptr&, const signal_sl&, s1, s2)
L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const signal_sl&, const signal_sl&, s1, s2)
L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const signal_sl&, const sc_logic&, s1, s2)
L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const signal_sl&, char, s1, sc_logic(s2))
L_DECL2(^, SC_LAMBDA_SUL_BITXOR, const sc_logic&, const signal_sl&, s1, s2)
L_DECL2(^, SC_LAMBDA_SUL_BITXOR, char, const signal_sl&, sc_logic(s1), s2)
#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

L_DECL2(|| , SC_LAMBDA_OR, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
L_DECL2(|| , SC_LAMBDA_OR, const sc_signal_edgy_deval&, const sc_signal_edgy_deval&, s1, s2)
L_DECL2(|| , SC_LAMBDA_OR, const sc_signal_edgy_deval&, bool, s1, s2)
L_DECL2(|| , SC_LAMBDA_OR, bool, const sc_signal_edgy_deval&, s1, s2)
L_DECL2(|| , SC_LAMBDA_OR, const sc_signal_edgy_deval&, const sc_lambda_ptr&, s1, s2)
L_DECL2(|| , SC_LAMBDA_OR, const sc_lambda_ptr&, const sc_signal_edgy_deval&, s1, s2)

L_DECL2(&&, SC_LAMBDA_AND, const sc_lambda_ptr&, const sc_lambda_ptr&, s1, s2)
L_DECL2(&&, SC_LAMBDA_AND, const sc_signal_edgy_deval&, const sc_signal_edgy_deval&, s1, s2)
L_DECL2(&&, SC_LAMBDA_AND, const sc_signal_edgy_deval&, bool, s1, s2)
L_DECL2(&&, SC_LAMBDA_AND, bool, const sc_signal_edgy_deval&, s1, s2)
L_DECL2(&&, SC_LAMBDA_AND, const sc_signal_edgy_deval&, const sc_lambda_ptr&, s1, s2)
L_DECL2(&&, SC_LAMBDA_AND, const sc_lambda_ptr&, const sc_signal_edgy_deval&, s1, s2)

L_DECL1(!, SC_LAMBDA_NOT, const sc_lambda_ptr&, s1)
L_DECL1(!, SC_LAMBDA_NOT, const sc_signal_edgy_deval&, s1)

#endif

#undef L_DECL1
#undef L_DECL1SPEC
#undef L_DECL2
#undef L_DECL2SPEC

#endif

#endif


sc_lambda::sc_lambda(sc_lambda_rator_e op,
sc_lambda_rand*   o1,
sc_lambda_rand*   o2)
: rator_ty(op), op1(o1), op2(o2)
{
	ref_count = 0;
}

#endif

#include "sc_wait.h"
#include "sc_list.h"
#include "sc_vector.h"
#include "sc_process_int.h"
#include "sc_context_switch.h"
#include "sc_except.h"
#include "sc_port.h"
#include "sc_port_manager.h"
#include "sc_process_int.h"
#include "sc_simcontext_int.h"
#if 1
void
halt(sc_simcontext* simc)
{
	const sc_curr_proc_info* cpi = simc->get_curr_proc_info();
	assert(SC_CURR_PROC_SYNC == cpi->kind);
	sc_sync_process_handle handle = (sc_sync_process_handle)cpi->process_handle;
	handle->wait_cycles = 1;
	handle->wait_state = sc_sync_process::WAIT_CLOCK;
#ifndef WIN32
	context_switch(sc_sync_process_yieldhelp, handle, 0, handle->next_qt());
#else
	context_switch(handle->next_fiber());
#endif
	throw sc_halt();
}

void
wait(sc_simcontext* simc)
{
	const sc_curr_proc_info* cpi = simc->get_curr_proc_info();
	switch (cpi->kind) {
	case SC_CURR_PROC_SYNC: {
								sc_sync_process_handle handle = (sc_sync_process_handle)cpi->process_handle;
								handle->wait_cycles = 1;
								handle->wait_state = sc_sync_process::WAIT_CLOCK;
#ifndef WIN32
								context_switch(sc_sync_process_yieldhelp, handle, 0, handle->next_qt());
#else
								context_switch(handle->next_fiber());
#endif
								handle->wait_state = sc_sync_process::WAIT_UNKNOWN;
								int exception_level = handle->exception_level;
								if (0 == exception_level) {
									throw sc_user();
								}
								else if (exception_level > 0) {
									throw exception_level;
								}
								break;
	}

	case SC_CURR_PROC_APROC: {
								 sc_aproc_process_handle handle = (sc_aproc_process_handle)cpi->process_handle;
#ifndef WIN32
								 qt_t* nqt = simc->next_aproc_qt();
								 handle->set_in_updateq(false);
								 /* No need to do context_switch because this is a tail call;
								 registers won't be needed even if they're messed up. */
								 QT_BLOCK(sc_aproc_process_yieldhelp, handle, 0, nqt);
#else
								 PVOID next_fiber = simc->next_aproc_fiber();
								 handle->set_in_updateq(false);
								 SwitchToFiber(next_fiber);
#endif
								 break;
	}

	default:
		cerr << "SystemC warning: Calling wait() in an SC_METHOD has no effect. Use wait() for SC_THREADs and SC_CTHREADs only." << endl;
		break;
	}
}

void
wait(int n, sc_simcontext* simc)
{
	const sc_curr_proc_info* cpi = simc->get_curr_proc_info();
	switch (cpi->kind) {
	case SC_CURR_PROC_SYNC: {
								sc_sync_process_handle handle = (sc_sync_process_handle)cpi->process_handle;
								handle->wait_cycles = n;
								handle->wait_state = sc_sync_process::WAIT_CLOCK;
#ifndef WIN32
								context_switch(sc_sync_process_yieldhelp, handle, 0, handle->next_qt());
#else
								context_switch(handle->next_fiber());
#endif
								handle->wait_state = sc_sync_process::WAIT_UNKNOWN;
								int exception_level = handle->exception_level;
								if (0 == exception_level) {
									throw sc_user();
								}
								else if (exception_level > 0) {
									throw exception_level;
								}
								break;
	}

	default:
		cerr << "SystemC warning: wait(n) works only for SC_CTHREADs. For other process types, it has no effect." << endl;
		break;
	}
}

void
wait_until(const sc_lambda_ptr& lambda, sc_simcontext* simc)
{
	const sc_curr_proc_info* cpi = simc->get_curr_proc_info();
	switch (cpi->kind) {
	case SC_CURR_PROC_SYNC: {
								sc_sync_process_handle handle = (sc_sync_process_handle)cpi->process_handle;
								handle->wait_lambda = lambda;
								handle->wait_state = sc_sync_process::WAIT_LAMBDA;

#ifndef WIN32
								context_switch(sc_sync_process_yieldhelp, handle, 0, handle->next_qt());
#else
								context_switch(handle->next_fiber());
#endif

								handle->wait_state = sc_sync_process::WAIT_UNKNOWN;
								int exception_level = handle->exception_level;
								if (0 == exception_level) {
									throw sc_user();
								}
								else if (exception_level > 0) {
									throw exception_level;
								}
								break;
	}

	default:
		cerr << "SystemC warning: wait_until() works only for SC_CTHREADs. For other process types, it has no effect; For them, you can use a do-while loop with wait() inside to get a similar effect." << endl;
		break;
	}
}

void
watching_before_simulation(const sc_lambda_ptr& lambda, sc_simcontext* simc)
{
	const sc_curr_proc_info* cpi = simc->get_curr_proc_info();
	simc->get_port_manager()->add_lambda_for_resolution(lambda);
	switch (cpi->kind) {
	case SC_CURR_PROC_SYNC: {
								sc_sync_process_handle handle = (sc_sync_process_handle)cpi->process_handle;
								int wlevel = handle->watch_level;
								assert(wlevel < SYSTEMC_MAX_WATCH_LEVEL);
								handle->dwatchlists[wlevel]->push_back(new sc_lambda_ptr(lambda));
								break;
	}
	default:
		cerr << "SystemC warning: watching works only for SC_CTHREADs. For other process types, watching has no effect." << endl;
		break;
	}
}

void
watching_during_simulation(const sc_lambda_ptr& lambda, sc_simcontext* simc)
{
	const sc_curr_proc_info* cpi = simc->get_curr_proc_info();
	switch (cpi->kind) {
	case SC_CURR_PROC_SYNC: {
								sc_sync_process_handle handle = (sc_sync_process_handle)cpi->process_handle;
								int wlevel = handle->watch_level;
								assert(wlevel < SYSTEMC_MAX_WATCH_LEVEL);
								handle->dwatchlists[wlevel]->push_back(new sc_lambda_ptr(lambda));
								break;
	}
	default:
		cerr << "SystemC warning: watching works only for SC_CTHREADs. For other process types, watching has no effect." << endl;
		break;
	}
}

void
__reset_watching(sc_sync_process_handle handle)
{
	handle->__reset_watching();
}

void
__open_watching(sc_sync_process_handle handle)
{
	handle->__open_watching();
}

void
__close_watching(sc_sync_process_handle handle)
{
	handle->__close_watching();
	assert(handle->__watch_level() >= 0);
}

int
__watch_level(sc_sync_process_handle handle)
{
	return handle->__watch_level();
}

void
__watching_first(sc_sync_process_handle handle)
{
	if (handle->eval_watchlist_curr_level()) {
		throw handle->exception_level;
	}
}

void
__sanitycheck_watchlists(sc_sync_process_handle handle)
{
	assert(handle->dwatchlists[handle->__watch_level()]->empty());
}

void
sc_set_location(const char* file, int lineno,
sc_simcontext* simc)
{
	const sc_curr_proc_info* cpi = simc->get_curr_proc_info();
	sc_process_b* handle = cpi->process_handle;
	handle->file = file;
	handle->lineno = lineno;
}


void
at_posedge(const sc_signal<sc_logic>& s, sc_simcontext* simc)
{
	if (s.read() == '1') {
		wait_until(s.delayed() == '0', simc);
		wait_until(s.delayed() == '1', simc);
	}
	else {
		wait_until(s.delayed() == '1', simc);
	}
}

void at_posedge(const sc_signal_edgy& s, sc_simcontext* simc)
{
	if (s.edgy_read()) {
		wait_until(s.delayed() == 0);
		wait_until(s.delayed());
	}
	else {
		wait_until(s.delayed());
	}
}

void at_negedge(const sc_signal<sc_logic>& s, sc_simcontext* simc)
{
	if (s.read() == '0') {
		wait_until(s.delayed() == '1', simc);
		wait_until(s.delayed() == '0', simc);
	}
	else {
		wait_until(s.delayed() == '0', simc);
	}
}

void at_negedge(const sc_signal_edgy& s, sc_simcontext* simc)
{
	if (!s.edgy_read()) {
		wait_until(s.delayed());
		wait_until(s.delayed() == 0);
	}
	else {
		wait_until(s.delayed() == 0);
	}
}
#endif