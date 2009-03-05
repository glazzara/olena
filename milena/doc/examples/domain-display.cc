#include <mln/essential/2d.hh>
int main()
{
  using namespace mln;

  // \{
  image2d<int> ima1(5, 5);
  image2d<int> ima2(10, 10);

  std::cout << "ima1.domain() = " << ima1.domain()
	    << std::endl;
  std::cout << "ima2.domain() = " << ima2.domain()
	    << std::endl;
  // \}
}
