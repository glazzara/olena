#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/debug/println.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>

#include "by_dilation.hh"


int main()
{
  using namespace mln;

  image2d<bool> f, g, o;
  neighb2d nbh = c4();

  bool gvals[9][5] = { { 0, 1, 1, 1, 0 },
		       { 0, 0, 0, 1, 0 },
		       { 0, 1, 1, 1, 0 },
		       { 0, 1, 0, 0, 0 },
		       { 0, 1, 1, 1, 0 },
		       { 0, 0, 0, 1, 0 },
		       { 0, 1, 1, 1, 0 },
		       { 0, 1, 0, 0, 0 },
		       { 0, 1, 1, 1, 0 } };
  g = make::image(gvals);
  debug::println("g", g);

  bool fvals[9][5] = { { 0, 0, 0, 0, 0 },
		       { 0, 0, 0, 0, 0 },
		       { 0, 0, 0, 0, 0 },
		       { 0, 0, 0, 0, 0 },
		       { 0, 0, 1, 0, 0 },
		       { 0, 0, 0, 0, 0 },
		       { 0, 0, 0, 0, 0 },
		       { 0, 0, 0, 0, 0 },
		       { 0, 0, 0, 0, 0 } };
  f = make::image(fvals);
  debug::println("f", f);

  o = morpho::reconstruction::impl::by_dilation_hybrid_on_set_fastest(f, g, nbh);
}




/*
  bool gvals[9][5] = { { 0, 0, 0, 0, 0 },
		       { 0, 1, 1, 0, 0 },
		       { 0, 1, 0, 0, 0 },
		       { 0, 1, 1, 1, 0 },
		       { 0, 0, 0, 1, 0 },
		       { 0, 1, 1, 1, 0 },
		       { 0, 1, 0, 0, 0 },
		       { 0, 1, 1, 1, 0 },
		       { 0, 0, 0, 0, 0 } };
  g = make::image(gvals);
  debug::println("g", g);

  bool fvals[9][5] = { { 0, 0, 0, 0, 0 },
		       { 0, 0, 0, 0, 0 },
		       { 0, 0, 0, 0, 0 },
		       { 0, 0, 1, 0, 0 },
		       { 0, 0, 0, 0, 0 },
		       { 0, 0, 0, 0, 0 },
		       { 0, 0, 0, 0, 0 },
		       { 0, 0, 0, 0, 0 },
		       { 0, 0, 0, 0, 0 } };
  f = make::image(fvals);
  debug::println("f", f);
*/
