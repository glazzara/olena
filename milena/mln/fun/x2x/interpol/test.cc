#include "bilinear.hh"
#include "linear.hh"

int main()
{
  typedef mln::image2d<int> I;
  typedef mln::image1d<int> II;

  I ima2d;
  II ima1d;

  mln::fun::x2x::interpol::bilinear<I> interp(ima2d);
  mln::fun::x2x::interpol::linear<II> interp3(ima1d);
}
