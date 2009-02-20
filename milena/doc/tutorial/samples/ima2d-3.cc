#include <mln/core/image/image2d.hh>
#include <mln/core/alias/box2d.hh>
#include <mln/opt/at.hh>
#include <mln/value/int_u8.hh>

int main()
{
  using namespace mln;

  // \{
  box2d b(2,3);
  image2d<value::int_u8> ima(b);

  // On image2d, Site <=> point2d
  point2d p(1, 2);

  // Associate '9' as value for the site/point2d (1,2).
  // The value is returned by reference and can be changed.
  opt::at(ima, 1,2) = 9;
  std::cout << "opt::at(ima, 1,2) = " << opt::at(ima, 1,2)
	    << std::endl;
  std::cout << "ima(p) = " << ima(p) << std::endl;

  std::cout << "---" << std::endl;


  // Associate '2' as value for the site/point2d (1,2).
  // The value is returned by reference
  // and can be changed as well.
  ima(p) = 2;
  std::cout << "opt::at(ima, 1,2) = " << opt::at(ima, 1,2)
	    << std::endl;
  std::cout << "ima(p) = " << ima(p) << std::endl;
  // \}
}
