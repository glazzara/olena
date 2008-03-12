#include <box2d.hh>


int main()
{
  using namespace mln;

  point2d<int> pmin(0, 0), pmax(5, 5);
  std::cout << pmin << std::endl;
  std::cout << pmax << std::endl;

  box2d<int> b(pmin, pmax);
  std::cout << b << std::endl;

  box2d<int>::fwd_piter p(b);


  for (p.start(); p.is_valid(); p.next())
  {
    std::cout << p.to_site() << std::endl;
  }
}
