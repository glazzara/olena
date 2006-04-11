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

//   check_trait(double, bool);
  check_trait(double, char);
  check_trait(double, float);
  check_trait(double, double);
  check_trait(double, ldouble);
  check_trait(double, uchar);
  check_trait(double, schar);
  check_trait(double, ushort);
  check_trait(double, sshort);
  check_trait(double, uint);
  check_trait(double, sint);
  check_trait(double, ulong);
  check_trait(double, slong);

}
