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

//   check_trait(bool, bool);
//   check_trait(bool, char);
//   check_trait(bool, float);
//   check_trait(bool, double);
//   check_trait(bool, ldouble);
  check_trait(bool, uchar);
  check_trait(bool, schar);
  check_trait(bool, ushort);
  check_trait(bool, sshort);
//   check_trait(bool, uint);
  check_trait(bool, sint);
//   check_trait(bool, ulong);
//   check_trait(bool, slong);

}
