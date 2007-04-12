#include <iostream>
#include <oln/core/2d/image2d.hh>
#include <oln/core/rle/rle_psite.hh>
#include <oln/core/rle/rle_image.hh>
#include <oln/debug/print.hh>
#include <oln/core/rle/rle_pset.hh>
#include <oln/core/encode/rle_encode.hh>


int main()
{
  oln::point2d p(0,1), q(2,2), r(3, 0);
  oln::rle_pset<oln::point2d> my_set;
  oln::rle_image<oln::point2d, int> rle;
  oln::rle_image<oln::point2d, int> rle2;

  my_set.insert(p, 5);
  my_set.insert(q, 8);

  rle.insert(p, 5, 4);
  rle.insert(q, 8, 9);

  oln::image2d<int> ima2d (1, 5);
  ima2d(oln::point2d(0, 4)) = 5;

  rle2 = rle_encode(ima2d);

  return 0;
}
