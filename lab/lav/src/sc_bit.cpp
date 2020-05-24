#include "sc_bit.h"
#include "sc_exception.h"

// constructors
// MANDATORY
//  sc_bit ();
sc_bit::sc_bit (int i)
{
  if(i!=0 && i!=1)
    throw sc_edata("sc_bit: value is not valid");
  rep = (void*)this;
  val_or_index = i;
}
//  sc_bit (bool);
sc_bit::sc_bit (char c)
{
  if(c!='0' && c!='1')
    throw sc_edata("sc_bit: value is not valid");
  rep = (void*)this;
  val_or_index = c-'0';
}
// copy constructor
// MANDATORY
//  sc_bit (const sc_bit& );


// destructor
// MANDATORY
//  ~sc_bit ();

// assignments

// MANDATORY
//  sc_bit& operator = (const sc_bit&);
sc_bit& sc_bit::operator = (int i)
{
  if(i!=0 && i!=1)
    throw sc_edata("sc_bit: value is not valid");
  if ((void*)this == rep) { /* raw */
      val_or_index = i;
  } else {
      sc_bvrep_set_wb((sc_bvrep*)rep, 0, val_or_index, i);
  }
  return *this;
}
//  sc_bit& operator = (bool);
sc_bit& sc_bit::operator = (char c)
{
  if(c!='0' && c!='1')
    throw sc_edata("sc_bit: value is not valid");
  if ((void*)this == rep) { /* raw */
      val_or_index = c-'0';
  } else {
      sc_bvrep_set_wb((sc_bvrep*)rep, 0, val_or_index, c-'0');
  }
  return *this;
}
// MANDATORY
sc_bit& sc_bit::operator &= (const sc_bit& s)
{
  return  operator &= ((bool)s);
}
sc_bit& sc_bit::operator &= (int i)
{
  if(i!=0 && i!=1)
    throw sc_edata("sc_bit: value is not valid");
  return operator &= ((bool)i);
}
//  sc_bit& operator &= (bool);
sc_bit& sc_bit::operator &= (char c)
{
  if(c!='0' && c!='1')
    throw sc_edata("sc_bit: value is not valid");
  return operator &= ((bool)(c-'0'));
}

// MANDATORY
sc_bit& sc_bit::operator |= (const sc_bit& s)
{
  return  operator |= ((bool)s);
}
sc_bit& sc_bit::operator |= (int i)
{
  if(i!=0 && i!=1)
    throw sc_edata("sc_bit: value is not valid");
  return operator |= ((bool)i);
}
//  sc_bit& operator |= (bool);
sc_bit& sc_bit::operator |= (char c)
{
  if(c!='0' && c!='1')
    throw sc_edata("sc_bit: value is not valid");
  return operator |= ((bool)(c-'0'));
}

// MANDATORY
sc_bit& sc_bit::operator ^= (const sc_bit& s)
{
  return  operator ^= ((bool)s);
}
sc_bit& sc_bit::operator ^= (int i)
{
  if(i!=0 && i!=1)
    throw sc_edata("sc_bit: value is not valid");
  return operator ^= ((bool)i);
}
//  sc_bit& operator ^= (bool);
sc_bit& sc_bit::operator ^= (char c)
{
  if(c!='0' && c!='1')
    throw sc_edata("sc_bit: value is not valid");
  return operator ^= ((bool)(c-'0'));
}

// conversions
// MANDATORY
char sc_bit::to_char () const
{
  return (bool)(*this);
}
// OPEN
// decided that operator bool will suffice
// bool to_bool () const;

// operators

// relational operators

// MANDATORY
#if !defined(__GNUC__)
#if defined(_MSC_VER)
#pragma warning(disable: 4805)
#endif
bool operator == (const sc_bit& a, const sc_bit& b){return (bool)a==(bool)b;}
bool operator == (const sc_bit& a, int i){return (bool)a==i;}
bool operator == (const sc_bit& a, bool b){return (bool)a==b;}
bool operator == (const sc_bit& a, char c){return (bool)a==(int)(c-'0');}
bool operator == (int i, const sc_bit& a){return a==i;}
bool operator == (bool b, const sc_bit& a){return a==b;}
bool operator == (char c, const sc_bit& a){return a==c;}
// MANDATORY
bool operator != (const sc_bit& a, const sc_bit& b){return (bool)a!=(bool)b;}
bool operator != (const sc_bit& a, int i){return (bool)a!=i;}
bool operator != (const sc_bit& a, bool b){return (bool)a!=b;}
bool operator != (const sc_bit& a, char c){return (bool)a!=(int)(c-'0');}
bool operator != (int i, const sc_bit& a){return a!=i;}
bool operator != (bool b, const sc_bit& a){return a!=b;}
bool operator != (char c, const sc_bit& a){return a!=c;}
#if defined(_MSC_VER)
#pragma warning(default: 4805)
#endif
// bitwise operators and functions

// bitwise complement

// MANDATORY
const sc_bit operator ~ (const sc_bit& a)
{
  sc_bit result(a);
  result.val_or_index = !result.val_or_index;
  return result;
}



// bitwise or
// MANDATORY
const sc_bit operator | (const sc_bit& a, const sc_bit& b)
{
  sc_bit result(a);
  return result|=b;
}
const sc_bit operator | (const sc_bit& a, int i)
{
  sc_bit result(a);
  return result|=i;
}
const sc_bit operator | (const sc_bit& a, bool b)
{
  sc_bit result(a);
  return result|=b;
}
const sc_bit operator | (const sc_bit& a, char c)
{
  sc_bit result(a);
  return result|=c;
}
const sc_bit operator | (int i, const sc_bit& a)
{
  sc_bit result(a);
  return result|=i;
}
const sc_bit operator | (bool b, const sc_bit& a)
{
  sc_bit result(a);
  return result|=b;
}
const sc_bit operator | (char c, const sc_bit& a)
{
  sc_bit result(a);
  return result|=c;
}

// bitwise and

// MANDATORY
const sc_bit operator & (const sc_bit& a, const sc_bit& b)
{
  sc_bit result(a);
  return result&=b;
}
const sc_bit operator & (const sc_bit& a, int i)
{
  sc_bit result(a);
  return result&=i;
}
const sc_bit operator & (const sc_bit& a, bool b)
{
  sc_bit result(a);
  return result&=b;
}
const sc_bit operator & (const sc_bit& a, char c)
{
  sc_bit result(a);
  return result&=c;
}
const sc_bit operator & (int i, const sc_bit& a)
{
  sc_bit result(a);
  return result&=i;
}
const sc_bit operator & (bool b, const sc_bit& a)
{
  sc_bit result(a);
  return result&=b;
}
const sc_bit operator & (char c, const sc_bit& a)
{
  sc_bit result(a);
  return result&=c;
}

// bitwise exor

// MANDATORY
const sc_bit operator ^ (const sc_bit& a, const sc_bit& b)
{
  sc_bit result(a);
  return result^=b;
}
const sc_bit operator ^ (const sc_bit& a, int i)
{
  sc_bit result(a);
  return result&=i;
}
const sc_bit operator ^ (const sc_bit& a, bool b)
{
  sc_bit result(a);
  return result^=b;
}
const sc_bit operator ^ (const sc_bit& a, char c)
{
  sc_bit result(a);
  return result^=c;
}
const sc_bit operator ^ (int i, const sc_bit& a)
{
  sc_bit result(a);
  return result^=i;
}
const sc_bit operator ^ (bool b, const sc_bit& a)
{
  sc_bit result(a);
  return result^=b;
}
const sc_bit operator ^ (char c, const sc_bit& a)
{
  sc_bit result(a);
  return result^=c;
}
#endif
