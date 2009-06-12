#include <mln/core/image/image2d.hh>
#include <mln/opt/at.hh>
#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>
#include <mln/accu/shape/bbox.hh>
#include <mln/canvas/morpho/connected_filter.hh>
#include <mln/core/alias/neighb2d.hh>
#include "value_wrapper.hh"
#include "site_wrapper.hh"

using namespace mln;

int main()
{
  typedef mln::image2d<mln::point2d> I;

  I a(8, 8);
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      for (int k = 0; k < 2; k++)
	opt::at(a, i, j)[k] = 20 + (k ? i : j);

  debug::println(canvas::morpho::connected_filter(a, c4(), morpho::attribute::site_wrapper< accu::shape::bbox<mln::point2d> >(), make::box2d(8, 8), true));
}