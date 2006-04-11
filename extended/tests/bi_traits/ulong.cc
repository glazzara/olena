#include <iostream>
#include <typeinfo>

#include <mlc/assert.hh>
#include <mlc/cmp.hh>

#include <xtd/optraits.hh>


# define check_op_trait(OpName, OpSymb, T1, T2) \
mlc::assert_< mlc::eq_< typeof(T1() OpSymb T2()), xtd_op_##OpName##_trait_(T1, T2) > >::check()

# define check_trait(T1, T2)			\
check_op_trait(plus, +, T1, T2);		\
check_op_trait(minus, -, T1, T2);		\
check_op_trait(mult, *, T1, T2)

// check_op_trait(mod, %, T1, T2);



int main()
{
  typedef long double ldouble;
  typedef unsigned char uchar;
  typedef signed char schar;
  typedef unsigned short ushort;
  typedef signed short sshort;
  typedef unsigned int uint;
  typedef signed int sint;
  typedef unsigned long ulong;
  typedef signed long slong;

  check_trait(ulong, bool);
  check_trait(ulong, char);
  check_trait(ulong, float);
  check_trait(ulong, double);
  check_trait(ulong, ldouble);
  check_trait(ulong, uchar);
  check_trait(ulong, schar);
  check_trait(ulong, ushort);
  check_trait(ulong, sshort);
  check_trait(ulong, uint);
  check_trait(ulong, sint);
  check_trait(ulong, ulong);
  check_trait(ulong, slong);

}
