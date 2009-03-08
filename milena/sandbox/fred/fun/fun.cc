// Meta functions test
#include "abs.hh"
#include "cos.hh"
#include "inc.hh"
#include "norm.hh"
#include "red.hh"

#include <iostream>

#define dbg_print(val) std::cout << #val << "\n\t -> \t" << (val) << std::endl
int main()
{
  mln::meta::abs abs;
  mln::meta::cos cos;
  mln::meta::inc inc;
  mln::meta::red red;

  mln::meta::norm::l1 l1;
  mln::meta::norm::l2 l2;
  mln::meta::norm::linfty linfty;

  std::cout << "Read only tests." << std::endl;
  std::cout << "----------------" << std::endl;

  // ABS
  mln_invariant(abs(-1) == 1);
  dbg_print(abs(-1));
  dbg_print(abs(-3.1415926535));

  // INC
  mln_invariant(inc(-1) == 0);
  dbg_print(inc(-1));
  dbg_print(inc(-3.1415926535));

  // COS
  mln_invariant(cos(0.) == 1.);
  dbg_print(cos(0.));
  dbg_print(cos(mln::math::acos(0.5)));

  // RED
  mln_invariant(red(mln::value::rgb8(8,13,21)) == 8);
  dbg_print(red(mln::value::rgb8(8,13,21)));

  // NORM
  mln::algebra::vec<3, double> v;
  v[0] = 1;
  v[1] = -1;
  v[2] = 0;
  dbg_print(v);
  dbg_print(l1(v));
  dbg_print(l2(v));
  dbg_print(linfty(v));

  std::cout << "Read/Write tests." << std::endl;
  std::cout << "-----------------" << std::endl;

  // INC
  {
    int x;
    dbg_print(inc(x) = 1);
    mln_invariant(inc(x) == 1);
    dbg_print(inc(x));
    dbg_print(x);
  }

  // COS
  {
    double x;
    dbg_print(cos(x) = 1.);
    mln_invariant(cos(x) == 1.);
    dbg_print(x);
  }

  // RED
  {
    mln::value::rgb8 rgb(8,13,21);
    dbg_print(rgb);
    dbg_print(red(rgb) = 0);
// FIXME: Doesn't compile! mln_invariant(red(rgb) == 0);
    dbg_print(rgb);
  }

  // NORM
  {
    dbg_print(v);
    dbg_print(l1(v) = 1.0);
    dbg_print(l1(v));
    dbg_print(v);
//     mln_invariant(l1(v) == 1.0); FIXME: check with epsilon

    dbg_print(l2(v) = 1.0);
    dbg_print(l2(v));
    dbg_print(v);
//     mln_invariant(l2(v) == 1.0);

    dbg_print(linfty(v) = 1.0);
    dbg_print(linfty(v));
    dbg_print(v);
//     mln_invariant(linfty(v) == 1.0);
  }

  std::cout << "All invariants passed." << std::endl;
}