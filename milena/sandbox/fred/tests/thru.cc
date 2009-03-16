// Meta functions test
#include <mln/fun/essential.hh>
#include <mln/fun/math/cos.hh>
#include <mln/core/image/thru_morpher.hh>
#include <mln/fun/compose.hh>

#include <mln/core/var.hh>
#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/debug/all.hh>
#include <iostream>
#include <typeinfo>

#define dbg_print(val) std::cout << #val << "\n\t -> \t" << (val) << std::endl

int main()
{
  using namespace mln;

  fun::cos cos;
  typedef image2d<float> I;
  I ima(5, 5);

  image2d<value::int_u8> tmp(5, 5);
  debug::iota(tmp);
  data::fill_with_image(ima, tmp);

  debug::println(ima);
  debug::println(thru(cos, ima));

  thru_image<I, fun::cos::with<fun::cos>::ret::result::with<float>::ret > ima2;
  ima2 = thru(cos(cos), ima);

  data::fill_with_image(ima2, (pw::value(tmp) - pw::cst(13.0f)) / pw::cst(12.0f) | tmp.domain());

  debug::println(ima);
}