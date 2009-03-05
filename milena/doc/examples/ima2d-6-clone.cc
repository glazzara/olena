#include <mln/essential/2d.hh>
int main()
{
  using namespace mln;

  // \{
  image2d<int> ima1(5, 5);
  image2d<int> ima3 = duplicate(ima1); // Makes a deep copy.

  point2d p(2, 2);
  ima3(p) = 3;

  std::cout << ima3(p) << " - " << ima1(p) << std::endl;
  std::cout << (ima3.id_() == ima1.id_()) << std::endl;
  // \}
}
