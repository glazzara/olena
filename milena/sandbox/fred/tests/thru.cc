// Meta functions test
#include <mln/fun/v2v/convert.hh>
#include <mln/fun/math/cos.hh>
#include <mln/core/image/vmorph/thru_image.hh>
#include <mln/core/image/vmorph/thrubin_image.hh>
#include <mln/fun/compose.hh>

#include <mln/core/var.hh>
#include <mln/core/image/image2d.hh>

#include <mln/fun/point/row.hh>
#include <mln/fun/math/inf.hh>

#include <mln/value/int_u8.hh>
#include <mln/debug/all.hh>
#include <iostream>
#include <typeinfo>

#include <mln/trait/next/solve_proxy.hh>

#define dbg_print(val) std::cout << #val << "\n\t -> \t" << (val) << std::endl

int main()
{
  using namespace mln;

  fun::cos cos;
  fun::inf inf;

  typedef image2d<float> I;
  I ima(5, 5);
  image2d<value::int_u8> tmp(5, 5);


  debug::iota(tmp);
  data::fill_with_image(ima, tmp);

  debug::println(ima);
  debug::println(thru(cos, ima));
  debug::println(thru(inf, ima, tmp));

  mln_VAR(ima2, thru(cos(cos), ima));

  mln::fun::row row;
  mln_piter_(I) p(ima.domain());
  for_all(p)
    std::cout << row(p);

  debug::println(ima2);
}
