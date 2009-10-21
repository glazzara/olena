#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/logical/and.hh>

#include "dilation.hh"


int main()
{
  using namespace mln;

  image2d<bool> f, g, o;

  bool gvals[] = { 1, 1, 0, 0, 1,
		   1, 1, 1, 1, 0,
		   0, 1, 1, 1, 0,
		   0, 1, 1, 1, 0,
		   0, 1, 0, 0, 0 };
  g = make::image2d(gvals);

  bool fvals[] = { 0, 0, 0, 0, 0,
		   0, 0, 1, 1, 1,
		   1, 1, 1, 1, 0,
		   1, 0, 0, 0, 0,
		   0, 0, 0, 0, 0 };
  f = make::image2d(fvals);

  o = morpho::conditional::dilation(f, g, c4().win());
}
