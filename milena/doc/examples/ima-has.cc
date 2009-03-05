#include <mln/essential/2d.hh>
int main()
{
  using namespace mln;

  // \{
  // Default border size is set to 0.

  // Image defined on a box2d from
  // (0, 0) to (2, 2)
  image2d<int> ima1(2, 3);

  std::cout << "ima1.has(0, 0) : "
	    << ima1.has(point2d(0, 0)) << std::endl;

  std::cout << "ima1.has(-3, 0) : "
	    << ima1.has(point2d(-3, 0)) << std::endl;

  std::cout << "ima1.has(2, 5) : "
	    << ima1.has(point2d(2, 5)) << std::endl;

  std::cout << "=========" << std::endl;

  // Set default border size to 0.
  border::thickness = 0;

  // Image defined on a box2d from
  // (0, 0) to (2, 2)
  image2d<int> ima2(2, 3);

  std::cout << "ima2.has(0, 0) : "
	    << ima2.has(point2d(0, 0)) << std::endl;

  std::cout << "ima2.has(-3, 0) : "
	    << ima2.has(point2d(-3, 0)) << std::endl;

  std::cout << "ima2.has(2, 5) : "
	    << ima2.has(point2d(2, 5)) << std::endl;
  // \}
}
