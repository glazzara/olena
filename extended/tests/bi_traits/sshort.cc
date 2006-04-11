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

  check_trait(sshort, bool);
  check_trait(sshort, char);
  check_trait(sshort, float);
  check_trait(sshort, double);
  check_trait(sshort, ldouble);
  check_trait(sshort, uchar);
  check_trait(sshort, schar);
  check_trait(sshort, ushort);
  check_trait(sshort, sshort);
  check_trait(sshort, uint);
  check_trait(sshort, sint);
  check_trait(sshort, ulong);
  check_trait(sshort, slong);

}
