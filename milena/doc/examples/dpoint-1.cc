#include <mln/core/alias/dpoint2d.hh>
#include <mln/core/alias/point2d.hh>
int main()
{
  using namespace mln;

  // \{
  dpoint2d dp(-1,0);
  point2d p(1,1);

  std::cout << p + dp << std::endl;
  // \}
}
