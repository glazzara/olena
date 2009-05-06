#include <mln/core/var.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>

#include "dilation.hh"



int slope(mln::point2d p)
{
  return p.row() + p.col();
}


int main()
{
  using namespace mln;

  image2d<int> ima(2,2);
  debug::iota(ima);
  debug::println(ima);

  debug::println(morpho::dilation(ima,
					 win_c4p()) );

  mln_VAR(ima2, slope | box2d(3,3));
  debug::println(ima2);
  debug::println(morpho::dilation(ima2, c4().win()) );
}
