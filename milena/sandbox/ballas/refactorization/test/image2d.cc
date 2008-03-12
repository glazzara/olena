#include <image2d.hh>


int main()
{
  using namespace mln;

  point2d<int> pmin(0, 0), pmax(5, 5);
  box2d<int> b(pmin, pmax);
  image2d<int> ima(b);

  ima(pmin) = 5;

   image2d<int>::fwd_piter p(ima.domain());
   for (p.start(); p.is_valid(); p.next())
     std::cout << ima(p) << std::endl;
}
