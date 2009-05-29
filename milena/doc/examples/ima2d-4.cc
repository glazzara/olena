#include <mln/core/image/image2d.hh>
int main()
{
  using namespace mln;
  image2d<char> ima1(12, 12);
  image2d<char> ima2(12, 12);

  // \{
  point2d p(9,9);

  // At (9, 9), both values change.
  ima1(p) = 'M';
  ima2(p) = 'W';

  bool b = (ima1(p) == ima2(p));
  std::cout << (b ? "True" : "False") << std::endl;
  // \}
}

