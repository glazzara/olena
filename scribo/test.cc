#include <mln/essential/2d.hh>
#include <mln/util/ord.hh>

int main()
{
  using namespace mln;


  point2d p1(0, 2);
  point2d p2(0, 2);

  std::cout << util::ord_strict(p1, p2) << std::endl;
  std::cout << util::ord_weak(p1, p2) << std::endl;

  std::cout << util::ord_strict(p2, p1) << std::endl;
  std::cout << util::ord_weak(p2, p1) << std::endl;

}
