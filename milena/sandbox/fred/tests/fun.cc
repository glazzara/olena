// Meta functions test
#include <mln/fun/math/abs.hh>
#include <mln/fun/math/cos.hh>
#include <mln/fun/math/norm.hh>
// #include <mln/fun/math/inc.hh>
#include <mln/fun/component/red.hh>
#include <mln/fun/component/comp.hh>
#include <mln/value/rgb8.hh>
#include <iostream>

#define dbg_print(val) std::cout << #val << "\n\t -> \t" << (val) << std::endl
int main()
{
  mln::fun::abs abs;
  mln::fun::cos cos;
//   mln::fun::inc inc;
  mln::fun::red red;
  mln::fun::comp comp;
  mln::fun::pcomp comp1(1);

  mln::fun::norm::l1 l1;
  mln::fun::norm::l2 l2;
  mln::fun::norm::linfty linfty;

  std::cout << "Read only tests." << std::endl;
  std::cout << "----------------" << std::endl;

  // ABS
  mln_invariant(abs(-1) == 1);
  dbg_print(abs(-1));
  dbg_print(abs(-3.1415926535));

  int x = 1;
//   abs(x) = 2;
//   abs.set(x, 2);

//   // INC
//   mln_invariant(inc(-1) == 0);
//   dbg_print(inc(-1));
//   dbg_print(inc(-3.1415926535));

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
//   dbg_print(cos(l1)(v));
  dbg_print(l2(v));
  dbg_print(linfty(v));

  std::cout << "Read/Write tests." << std::endl;
  std::cout << "-----------------" << std::endl;

  // INC
//   {
//     int x;
//     dbg_print(inc(x) = 1);
//     mln_invariant(inc(x) == 1);
//     dbg_print(inc(x));
//     dbg_print(x);
//   }

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
    mln_invariant(red(rgb) == 0);
    dbg_print(rgb);
    dbg_print(comp(0, rgb));
    dbg_print(comp(1, rgb));
    dbg_print(comp(2, rgb));
    comp1(rgb) = 2;
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
