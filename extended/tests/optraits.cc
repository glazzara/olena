#include <iostream>
#include <xtd/optraits.hh>
#include <xtd/math.hh>


template <typename T>
static void foo()
{
  xtd_op_plus_trait(int, char) tmp;
}


template <typename E1, typename E2>
static
void test_plus(const xtd::abstract::fun_expr_<E1>& expr1,
	       const xtd::abstract::fun_expr_<E2>& expr2)
{
  const xtd_op_plus_trait(E1, E2)& tmp = expr1 + expr2;
  std::cout << tmp(1.) << std::endl;
}


int main()
{
  using xtd::cos;
  using xtd::sin;
  using xtd::_1;

  typeof(1+2u) tmp;
//   2 * cos(_1);

  test_plus(2 * cos(_1), sin(_1));
}
