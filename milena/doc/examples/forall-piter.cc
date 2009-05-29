#include <mln/core/alias/box2d.hh>

int main()
{
  using namespace mln;

  // \{
  box2d b(3, 2);
  mln_piter_(box2d) p(b);

  for_all(p)
    std::cout << p; //prints every site coordinates.
  // \}

}
