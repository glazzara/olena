#include <mln/core/alias/box2d.hh>
#include <mln/core/site_set/p_array.hh>
#include <mln/make/box2d.hh>

int main()
{
  using namespace mln;

  // \{
  box2d b(4,4);
  // \}

  std::cout << "// \\{" << std::endl;
  // \{
  mln_piter_(box2d) p(b);
  for_all(p)
    std::cout << p << ", ";
  std::cout << std::endl;
  // \}
  std::cout << "// \\}" << std::endl;


  {
    // \{
    p_array<point2d> arr;
    arr.append(point2d(2,2));
    arr.append(point2d(1,2));
    // \}

    std::cout << "// \\{" << std::endl;
    // \{
    mln_piter_(p_array<point2d>) p(arr);
    for_all(p)
      std::cout << p << ", ";
    std::cout << std::endl;
    // \}
    std::cout << "// \\}" << std::endl;
  }

  {
    // \{
    box2d b = make::box2d(-2,-2, 2, 2);
    // \}

    std::cout << "// \\{" << std::endl;
    mln_piter_(box2d) p(b);
    for_all(p)
      std::cout << p << ", ";
    std::cout << std::endl;
    std::cout << "// \\}" << std::endl;
  }
}
