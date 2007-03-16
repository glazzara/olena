#include <iostream>
#include <oln/core/rle/rle_psite.hh>
#include <oln/core/rle/rle_image.hh>
#include <oln/core/2d/image2d.hh>
#include <oln/debug/print.hh>
#include <oln/core/rle/rle_pset.hh>
#include <oln/core/encode/rle_encode.hh>


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
  oln::rle_pset<oln::point2d> my_set;
  oln::rle_image<oln::point2d, int> rle;
  oln::rle_image<oln::point2d, int> rle2;

  my_set.insert(p, 5);
  my_set.insert(q, 8);

  rle.insert(p, 5, 4);
  rle.insert(q, 8, 9);
  std::cout << my_set.has(q) << std::endl;
  test(my_set);
  std::cout << "reverse" << std::endl;
  rtest(my_set);
  std::cout << "ima" << std::endl;
  print(rle);

  oln::debug::print(rle);
  std::cout << rle.points().npoints() << std::endl;

  std::cout << "encode rle" << std::endl;
  oln::image2d<int> ima2d (1, 5);
  ima2d(oln::point2d(0, 4)) = 5;

  oln::debug::print(ima2d);

  rle2 = rle_encode(ima2d);

  oln::debug::print(rle2);
  return 0;
}
