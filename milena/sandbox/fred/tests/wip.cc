// Meta functions test
#include <mln/accu/min.hh>
#include <mln/fun/accu_result.hh>
#include <mln/fun/from_accu.hh>
#include <mln/fun/math/cos.hh>
#include <mln/fun/compose.hh>
#include <mln/core/var.hh>

#include <iostream>

#define dbg_print(val) std::cout << #val << "\n\t -> \t" << (val) << std::endl
int main()
{
  mln::accu::min<int> min;
  mln::fun::cos cos;
  mln::fun::accu_result result;
  mln::fun::from_accu< mln::accu::min<int> > frommin(&min);

  mln_VAR(cosfrommin, cos(frommin));

  for (int i = 5; i > 0; i--)
    std::cout << cosfrommin(i) << std::endl;
  for (int i = 1; i <= 5; i++)
    std::cout << cosfrommin(i) << std::endl;
}