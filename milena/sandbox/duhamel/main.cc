#include <mln/core/image2d_b.hh>
#include <mln/value/int_u8.hh>
#include <mln/debug/println.hh>


#include <mln/geom/nrows.hh>
#include <mln/geom/ncols.hh>

#include <mln/level/fill.hh>

#include <mln/debug/println_with_border.hh>

#include "paste.hh"
#include "fill.hh"

  using namespace mln;

int main (void)
{
  image2d_b<int> i1(3, 3);
  image2d_b<int> i2(3, 3);
  mln::sparse_image<mln::point2d, int> sparse;
  mln::sparse_image<mln::point2d, int> sparse2;
  mln::rle_image<mln::point2d, int> rle1;
  mln::rle_image<mln::point2d, int> rle2;

//   level::fill_opt2(i1, 8);
//   debug::println_with_border(i1);
  level::paste(rle1, rle2);
 //  level::fill(sparse, 42);
//   debug::println_with_border(i2);

  return (0);
}
