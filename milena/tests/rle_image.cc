#include <mln/core/image2d.hh>
#include <mln/core/rle_image.hh>
#include <mln/core/rle_encode.hh>

using namespace mln;

template <typename Pset>
void test(const Pset& my_set)
{
  typename Pset::fwd_piter run (my_set);
  for (run.start(); run.is_valid(); run.next())
    /*std::cout << run << std::endl*/;

  //  std::cout << "Reverse" << std::endl;

  typename Pset::bkd_piter run2 (my_set);
  for (run2.start(); run2.is_valid(); run2.next())
    /*std::cout << run2 << std::endl*/;
}

int
main()
{
  mln::point2d p, q, r;
  r = make::point2d(0, 1);
  q = make::point2d(2, 2);
  r = make::point2d(3, 0);
  mln::internal::run_pset_<mln::point2d> my_set;
  mln::rle_image<mln::point2d, int> rle;
  mln::rle_image<mln::point2d, int> rle2;

  my_set.insert(p, 5);
  my_set.insert(q, 8);
  test(my_set);

  rle.insert(p, 5, 4);
  rle.insert(q, 8, 9);

//   mln::debug::print(rle);
//   std::cout << std::endl;

  mln::image2d<int> ima2d (1, 5);
  ima2d(make::point2d(0, 4)) = 5;

//   mln::debug::print(ima2d);

  rle2 = rle_encode(ima2d);

  mln::rle_image<mln::point2d, int>::fwd_piter p1(rle2.domain());
  for_all(p1)
  {
    assert(ima2d(p1) == rle2(p1));
  }

//   mln::debug::print(rle2);
//   std::cout << std::endl;

  return 0;
}
