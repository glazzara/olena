#include <mln/essential/2d.hh>
int main()
{
  using namespace mln;

  // Define a box2d from (-2,-3) to (3,5).
  box2d b = make::box2d(-2,-3, 3,5);
  // Initialize an image with b as domain.
  image2d<int> ima(b);

  std::cout << "b = " << b << std::endl;
  std::cout << "domain = " << ima.domain() << std::endl;
}
