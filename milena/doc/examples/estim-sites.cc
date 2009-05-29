#include <mln/core/image/image2d.hh>
#include <mln/geom/nsites.hh>
int main()
{
  using namespace mln;

  image2d<int> ima(2, 3);

  // \{
  unsigned nsites = geom::nsites(ima);
  // \}

  (void) nsites;
}
