/* -*- c++ -*- */

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

    sc_bool_vector_bitop.m4 - m4 template for bitwise operators for bool_vector.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


/*define(head,AUTOMATICALLY GENERATED -- DO NOT EDIT)
 * Use process_m4.sh to generate this file
 *  head
 */

/*
 *  USE m4 to process this file.
 *
 *  _define_ OP to           &           |           ^ 
 *  _define_ OP_REDUCE to    and_reduce  or_reduce   xor_reduce
 *  _define_ IDENTITY to     true        false       false
 *  _define_ OP_NAME  to     and         or          xor
 */

sc_bool_vector&
sc_bool_vector::operator OP= ( const sc_bool_vector& rhs )
{
    if (&rhs == this)
        return *this;
    int leng = rhs.length();
    check_length(leng);

    if (overlapp(rhs)) {
        const int tmp_alloc = leng * sizeof(bool);
        bool* tmp_rhs = (bool*) sc_mempool::allocate(tmp_alloc);
        rhs.to_cpp_array(tmp_rhs);
        for (int i = leng - 1; i >= 0; --i) {
#ifdef WIN32
            // MSVC++ miscompiles the original; so we'll spell it out.
            bool* tmp = get_data_ptr(i);
            int tmp2 = *tmp;
            tmp2 OP= int(tmp_rhs[i]);
            *tmp = (0 != tmp2);
#else
            *(get_data_ptr(i)) OP= tmp_rhs[i];
#endif
        }
        sc_mempool::release(tmp_rhs, tmp_alloc);
    } else {
        for (int i = leng - 1; i >= 0; --i) {
#ifdef WIN32
            // MSVC++ miscompiles the original; so we'll spell it out.
            bool* tmp = get_data_ptr(i);
            int tmp2 = *tmp;
            tmp2 OP= int(*rhs.get_data_ptr(i));
            *tmp = (0 != tmp2);
#else
            *(get_data_ptr(i)) OP= *(rhs.get_data_ptr(i));
#endif
        }
    }
    return *this;
}

sc_bool_vector&
sc_bool_vector::operator OP= ( const sc_logic_vector& rhs )
{
    int leng = rhs.length();
    check_length(leng);

    for (int i = leng - 1; i >= 0; --i) {
#ifdef WIN32
        // MSVC++ miscompiles the original; so we'll spell it out.
        bool* tmp = get_data_ptr(i);
        int tmp2 = *tmp;
        tmp2 OP= int(rhs.get_data_ptr(i)->to_bool());
        *tmp = (0 != tmp2);
#else
        *(get_data_ptr(i)) OP= rhs.get_data_ptr(i)->to_bool();
#endif
    }
    return *this;
}

sc_bool_vector&
sc_bool_vector::operator OP= ( const char* rhs )
{
    int leng = length();
    rhs += (strlen(rhs) - leng);
    for (int i = leng - 1; i >= 0; --i) {
#ifdef WIN32
        // MSVC++ miscompiles the original; so we'll spell it out.
        bool* tmp = get_data_ptr(i);
        int tmp2 = *tmp;
        tmp2 OP= int(*rhs != '0');
        *tmp = (0 != tmp2);
#else
        *(get_data_ptr(i)) OP= (*rhs != '0');
#endif
        ++rhs;
    }
    return *this;
}

sc_bool_vector&
sc_bool_vector::`asgn_'OP_NAME`op1'( const sc_bool_vector* a, const sc_bool_vector* b )
{
    int leng = length();
    a->check_length(leng);
    b->check_length(leng);
    for (int i = leng - 1; i >= 0; --i) {
#ifdef WIN32
        assign_help(i, (0 != (int(*(a->get_data_ptr(i))) OP int(*(b->get_data_ptr(i))))));
#else
        assign_help(i, *(a->get_data_ptr(i)) OP *(b->get_data_ptr(i)));
#endif
    }
    return *this;
}

sc_bool_vector&
sc_bool_vector::`asgn_'OP_NAME`op2'( const sc_bool_vector* a, const char* b )
{
    int leng = length();
    a->check_length(leng);
    b += (strlen(b) - leng);
    for (int i = leng - 1; i >= 0; --i) {
#ifdef WIN32
        assign_help(i, (0 != (int(*(a->get_data_ptr(i))) OP int(*b != '0'))));
#else
        assign_help(i, *(a->get_data_ptr(i)) OP (*b != '0'));
#endif
        ++b;
    }
    return *this;
}

bool
sc_bool_vector::OP_REDUCE () const
{
    bool result = IDENTITY;
    for (int i = length() - 1; i >= 0; --i) {
#ifdef WIN32
        result = (0 != (int(result) OP int(*(get_data_ptr(i)))));
#else
        result = result OP *(get_data_ptr(i));
#endif
    }
    return result;
}

sc_signal_bool_vector&
sc_signal_bool_vector::write( const `sc_bool_vector_'OP_NAME`op1&' x )
{
    const sc_bool_vector* a = x.a;
    const sc_bool_vector* b = x.b;
    for (int i = length() - 1; i >= 0; --i) {
#ifdef WIN32
        super::write_help( i, (0 != (int(*(a->get_data_ptr(i))) OP int(*(b->get_data_ptr(i))))));
#else
        super::write_help( i, *(a->get_data_ptr(i)) OP *(b->get_data_ptr(i)));
#endif
    }
    return *this;
}

sc_signal_bool_vector&
sc_signal_bool_vector::write( const `sc_bool_vector_'OP_NAME`op2&' x )
{
    const sc_bool_vector* a = x.a;
    const char* b = x.b;
    int leng = length();
    a->check_length(leng);
    b += (strlen(b) - leng);

    for (int i = leng - 1; i >= 0; --i) {
#ifdef WIN32
        super::write_help( i, (0 != (int(*(a->get_data_ptr(i))) OP int(*b != '0'))));
#else
        super::write_help( i, *(a->get_data_ptr(i)) OP (*b != '0'));
#endif
        ++b;
    }
    return *this;
}
