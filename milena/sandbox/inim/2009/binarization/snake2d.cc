#include <mln/win/rectangle2d.hh>
#include <mln/accu/maj_h.hh>
#include <mln/value/int_u8.hh>
#include <mln/data/compare.hh>
#include <mln/make/image2d.hh>
#include <mln/border/thickness.hh>
#include "snake2d.hh"

#include <mln/debug/all.hh>

int main()
{
  using namespace mln;
  using value::int_u8;

  int_u8 vals [5][5] = {
    {0, 0, 0, 1, 1},
    {0, 0, 0, 1, 1},
    {0, 0, 1, 1, 1},
    {2, 3, 3, 3, 3},
    {5, 4, 4, 5, 5}
  };

  int_u8 valres [5][5] = {
    {0, 0, 0, 1, 1},
    {0, 0, 0, 1, 1},
    {0, 0, 0, 1, 1},
    {0, 0, 1, 1, 1},
    {0, 3, 3, 3, 3}
  };

  border::thickness = 0;
  image2d<int_u8> ima = make::image2d(vals);
  image2d<int_u8> res = snake2d(accu::maj_h<int_u8>(), ima, win::rectangle2d(5,5));
  mln_assertion (res == make::image2d(valres));
  image2d<int_u8> res2 = snake2d(accu::maj_h<int_u8>(), ima, win::rectangle2d(5,5));
  mln_assertion (res2 == make::image2d(valres));
  return 0;
}
