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

  check_trait(ushort, bool);
  check_trait(ushort, char);
  check_trait(ushort, float);
  check_trait(ushort, double);
  check_trait(ushort, ldouble);
  check_trait(ushort, uchar);
  check_trait(ushort, schar);
  check_trait(ushort, ushort);
  check_trait(ushort, sshort);
  check_trait(ushort, uint);
  check_trait(ushort, sint);
  check_trait(ushort, ulong);
  check_trait(ushort, slong);

}
