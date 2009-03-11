/*!
 * \file   test_pipo.cc
 * \author etiennefolio <ornthalas@gmail.com>
 */

#include "../../value/pipo.hh"

int main()
{
  using namespace mln;

  value::pipo p;
  std::cout << p << std::endl;

  p.c0 = -1;
  p.c1 = 0.42f;
  p.c2 = 42;
  std::cout << p << std::endl;

  return 0;
}
