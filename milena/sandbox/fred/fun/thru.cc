// Meta functions test
#include "cos.hh"
#include "thru_morpher.hh"

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/debug/all.hh>
#include <iostream>

#define dbg_print(val) std::cout << #val << "\n\t -> \t" << (val) << std::endl

int main()
{
  using namespace mln;

  meta::cos cos;
  typedef image2d<float> I;
  I ima(5, 5);

  image2d<value::int_u8> tmp(5, 5);
  debug::iota(tmp);
  data::fill_with_image(ima, tmp);

  debug::println(ima);
  debug::println(thru(meta::cos(), ima));

  thru_image<I, mln::fun::cos<float> > ima2 = thru(meta::cos(), ima);
  data::fill_with_image(ima2, (pw::value(tmp) - pw::cst(13.0f)) / pw::cst(12.0f) | tmp.domain());

  debug::println(ima);
}