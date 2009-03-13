// Meta functions test
#include "cos.hh"
#include "compose.hh"

#include <iostream>

#define dbg_print(val) std::cout << #val << "\n\t -> \t" << (val) << std::endl
int main()
{
  mln::fun::cos cos;
  mln::fun::compose compose;

  double x;
  dbg_print(cos(compose)(cos,cos)(x) = 0.857553);
  dbg_print(x);
  dbg_print(cos(compose)(cos,cos)(0.));

  // COS
  {
    mln_invariant(cos(0.) == 1.);
    dbg_print(cos(0.));
    dbg_print(cos(mln::math::acos(0.5)));
  }

  // COS
  {
    double x;
    dbg_print(cos(x) = 1.);
    mln_invariant(cos(x) == 1.);
    dbg_print(x);
  }

  std::cout << "Ok." << std::endl;
}