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

  check_trait(sint, bool);
  check_trait(sint, char);
  check_trait(sint, float);
  check_trait(sint, double);
  check_trait(sint, ldouble);
  check_trait(sint, uchar);
  check_trait(sint, schar);
  check_trait(sint, ushort);
  check_trait(sint, sshort);
  check_trait(sint, uint);
  check_trait(sint, sint);
  check_trait(sint, ulong);
  check_trait(sint, slong);

}
