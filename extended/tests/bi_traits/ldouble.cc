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

//   check_trait(ldouble, bool);
  check_trait(ldouble, char);
  check_trait(ldouble, float);
  check_trait(ldouble, double);
  check_trait(ldouble, ldouble);
  check_trait(ldouble, uchar);
  check_trait(ldouble, schar);
  check_trait(ldouble, ushort);
  check_trait(ldouble, sshort);
  check_trait(ldouble, uint);
  check_trait(ldouble, sint);
  check_trait(ldouble, ulong);
  check_trait(ldouble, slong);

}
