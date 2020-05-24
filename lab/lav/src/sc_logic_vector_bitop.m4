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

    sc_logic_vector_bitop.m4 - m4 template for bitwise operators for
                               sc_logic_vector

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
 *  _define_ IDENTITY to     `1'         `0'         `0'
 */

sc_logic_vector&
sc_logic_vector::operator OP= ( const sc_logic_vector& rhs )
{
    if (&rhs == this)
        return *this;
    int leng = rhs.length();
    check_length(leng);

    if (overlapp(rhs)) {
        sc_logic* tmp_rhs = new sc_logic[leng];
        rhs.to_cpp_array(tmp_rhs);
        for (int i = leng - 1; i >= 0; --i) {
            *get_data_ptr(i) OP= tmp_rhs[i];
        }
        delete[] tmp_rhs;
    } else {
        for (int i = leng - 1; i >= 0; --i) {
            *get_data_ptr(i) OP= *rhs.get_data_ptr(i);
        }
    }
    return *this;
}

sc_logic_vector&
sc_logic_vector::operator OP= ( const sc_bool_vector& rhs )
{
    int leng = rhs.length();
    check_length(leng);

    for (int i = leng - 1; i >= 0; --i) {
        *get_data_ptr(i) OP= sc_logic((char)(*rhs.get_data_ptr(i)));
    }
    return *this;
}

sc_logic_vector&
sc_logic_vector::operator OP= ( const char* rhs )
{
    int leng = length();
    rhs += (strlen(rhs) - leng);
    for (int i = leng - 1; i >= 0; --i) {
        *get_data_ptr(i) OP= *rhs;
        ++rhs;
    }
    return *this;
}

sc_logic_vector
operator OP ( const sc_logic_vector& a, const sc_logic_vector& b )
{
    int leng = b.length();
    a.check_length(leng);
    sc_logic* c = new sc_logic[leng];
    for (int i = leng - 1; i >= 0; --i) {
        c[i] = *a.get_data_ptr(i) OP *b.get_data_ptr(i);
    }
    return sc_logic_vector( c, leng );
}

sc_logic_vector
operator OP ( const sc_logic_vector& a, const sc_bool_vector& b )
{
    int leng = b.length();
    a.check_length(leng);
    sc_logic* c = new sc_logic[leng];
    for (int i = leng - 1; i >= 0; --i) {
        c[i] = *a.get_data_ptr(i) OP sc_logic((char)(*b.get_data_ptr(i)));
    }
    return sc_logic_vector( c, leng );
}

sc_logic_vector
operator OP ( const sc_logic_vector& a, const char* b )
{
    int leng = a.length();
    b += (strlen(b) - leng);
    sc_logic* c = new sc_logic[leng];
    for (int i = leng - 1; i >= 0; --i) {
        c[i] = *a.get_data_ptr(i) OP sc_logic(*b);
        ++b;
    }
    return sc_logic_vector( c, leng );
}

sc_logic
sc_logic_vector::OP_REDUCE () const
{
    sc_logic result(IDENTITY);
    for (int i = length() - 1; i >= 0; --i) {
        result = result OP *get_data_ptr(i);
    }
    return result;
}
