#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/debug/println.hh>

#include "dilation_permissive.hh"


int main()
{
  using namespace mln;

  image2d<bool> f, g, d_ref, d_gen, d_fast, d_gen_alt, d_fast_alt;
  neighb2d nbh = c4();

  bool gvals[] = { 1, 1, 0, 0, 1,
		   1, 1, 1, 1, 0,
		   0, 1, 1, 1, 0,
		   0, 1, 1, 1, 0,
		   0, 1, 0, 0, 0 };
  g = make::image2d(gvals);
  debug::println("g", g);

  bool fvals[] = { 0, 0, 0, 0, 0,
		   0, 0, 1, 1, 1,
		   1, 1, 1, 1, 0,
		   1, 0, 0, 0, 0,
		   0, 0, 0, 0, 0 };
  f = make::image2d(fvals);
  debug::println("f", f);


  d_ref = morpho::geodesic::impl::dilation_strict_1_set_formula(f, g, nbh);
  debug::println("d", d_ref);

  d_gen = morpho::geodesic::impl::dilation_permissive_1_set_generic(f, g, nbh);
  mln_assertion(logical::and_(d_gen, g) == d_ref);

  d_fast = morpho::geodesic::impl::dilation_permissive_1_set_fastest(f, g, nbh);
  mln_assertion(d_fast == d_gen);

  d_gen_alt = morpho::geodesic::impl::dilation_permissive_1_set_generic_alt(f, g, nbh);
  mln_assertion(logical::and_(d_gen_alt, g) == d_ref);
  mln_assertion(d_gen_alt == d_gen);

  d_fast_alt = morpho::geodesic::impl::dilation_permissive_1_set_fastest_alt(f, g, nbh);
  mln_assertion(d_fast_alt == d_gen_alt);
}
