#include <mln/core/alias/box2d.hh>
#include <mln/core/site_set/p_array.hh>
#include <mln/make/box2d.hh>

#include <doc/tools/sample_utils.hh>

int main()
{
  using namespace mln;

  // \{
  box2d b(4,4);
  // \}

  doc::begin_output();
  // \{
  mln_piter_(box2d) p(b);
  for_all(p)
    std::cout << p << ", ";
  std::cout << std::endl;
  // \}
  doc::end_output();


  {
    // \{
    p_array<point2d> arr;
    arr.append(point2d(2,2));
    arr.append(point2d(1,2));
    // \}

    doc::begin_output();
    // \{
    mln_piter_(p_array<point2d>) p(arr);
    for_all(p)
      std::cout << p << ", ";
    std::cout << std::endl;
    // \}
    doc::end_output();
  }

  {
    // \{
    box2d b = make::box2d(-2,-2, 2, 2);
    // \}

    doc::begin_output();
    mln_piter_(box2d) p(b);
    for_all(p)
      std::cout << p << ", ";
    std::cout << std::endl;
    doc::end_output();
  }
}
