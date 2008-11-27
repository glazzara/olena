#include <mln/essential/2d.hh>
int main()
{
  using namespace mln;

  // \{
  dpoint2d dp(-1,0);
  point2d p(1,1);

  std::cout << p + dp << std::endl;
  // \}
}
