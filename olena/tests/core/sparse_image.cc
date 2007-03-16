#include <iostream>
#include <oln/core/rle/rle_psite.hh>
#include <oln/core/2d/image2d.hh>
#include <oln/debug/print.hh>
#include <oln/core/rle/rle_pset.hh>
#include <oln/core/sparse/sparse_image.hh>
#include <oln/core/encode/sparse_encode.hh>

template <typename Ps>
void test(const Ps& pset)
{
  typename Ps::fwd_piter it (pset);

  for (it.start(); it.is_valid(); it.next())
    std::cout << it.to_point() << std::endl;
}

template <typename Ps>
void rtest(const Ps& pset)
{
  typename Ps::bkd_piter it (pset);

  for (it.start(); it.is_valid(); it.next())
    std::cout << it.to_point() << std::endl;
}


  template <typename I>
  void print(I& ima)
{
  typename I::piter it (ima.points());

  for (it.start(); it.is_valid(); it.next())
  {
    std::cout << (oln::point2d) it << std::endl;
    std::cout << ima(it) << std::endl;
  }
}


int main()
{
  oln::point2d p(0,1), q(2,2), r(3, 0);
  oln::image2d<int> ima2d (1, 5);
  oln::sparse_image<oln::point2d, int> sparse;
  oln::sparse_image<oln::point2d, int> sparse2;

  ima2d(oln::point2d(0, 4)) = 5;
  oln::debug::print(ima2d);

  std::vector<int> values;
  int a =5;
  values.push_back(a);
  a = 6;
  values.push_back(a);
  a = 42;
  values.push_back(a);

  sparse.insert(q, 3, values);

  oln::debug::print(sparse);

  std::cout << "encode sparse" << std::endl;
  sparse2 = sparse_encode(ima2d);

  oln::debug::print(sparse2);

  return 0;
}
