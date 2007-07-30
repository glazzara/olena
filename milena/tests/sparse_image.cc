#include <mln/core/image2d_b.hh>
#include <mln/core/sparse_image.hh>
#include <mln/core/sparse_encode.hh>
#include <vector>

using namespace mln;


template <typename Pset>
void test(const Pset& my_set)
{
  typename Pset::fwd_piter run (my_set);
  for (run.start(); run.is_valid(); run.next())
    ;
  //std::cout << run << std::endl;

    //  std::cout << "Reverse" << std::endl;
  typename Pset::bkd_piter run2 (my_set);

  //std::cout << "Reverse\n";

  for (run2.start(); run2.is_valid(); run2.next())
    ;
  //   std::cout << run2 << std::endl;
}

template <typename I>
void test2(I& ima)
{
  typename I::fwd_piter run (ima.domain());
  for_all(run)
    ++ima(run);

  typename I::bkd_piter run2 (ima.domain());
  for_all(run2)
    assert(ima(run2) == 1);
}

int
main()
{
  mln::point2d p, q, r;
  p = mln::make::point2d(0, 1);
  q = mln::make::point2d(3, 0);
  r = mln::make::point2d(2, 2);
  mln::internal::run_pset_<mln::point2d> my_set;
  mln::sparse_image<mln::point2d, int> sparse;
  mln::sparse_image<mln::point2d, int> sparse2;


  my_set.insert(p, 5);
  my_set.insert(q, 8);
  test(my_set);

  std::vector<int> values;
  int a = 0;
  values.push_back(a);
  a = 0;
  values.push_back(a);
  a = 0;
  values.push_back(a);
  sparse.insert(q, 3, values);

  test(sparse.domain());
  test2(sparse);

   mln::image2d_b<int> ima2d (1, 5);

   ima2d(mln::make::point2d(0, 4)) = 5;
   ima2d(mln::make::point2d(0, 3)) = 2;

//   oln::debug::print(ima2d);

   sparse2 = sparse_encode(ima2d);
   

//   oln::debug::print(sparse2);
//   std::cout << std::endl;

  return 0;
}
