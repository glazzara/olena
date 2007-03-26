#include <iostream>
#include <oln/core/rle/rle_psite.hh>
#include <oln/core/2d/image2d.hh>
#include <oln/debug/print.hh>
#include <oln/core/rle/rle_pset.hh>
#include <oln/core/sparse/sparse_image.hh>
#include <oln/core/encode/sparse_encode.hh>


int main()
{
  oln::point2d p(0,1), q(2,2), r(3, 0);
  oln::image2d<int> ima2d (1, 5);
  oln::sparse_image<oln::point2d, int> sparse;
  oln::sparse_image<oln::point2d, int> sparse2;

  ima2d(oln::point2d(0, 4)) = 5;

  std::vector<int> values;
  int a =5;
  values.push_back(a);
  a = 6;
  values.push_back(a);
  a = 42;
  values.push_back(a);

  sparse.insert(q, 3, values);

  sparse2 = sparse_encode(ima2d);

  return 0;
}
