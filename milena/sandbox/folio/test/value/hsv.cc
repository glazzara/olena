
#include <mln/core/image/image2d.hh>
#include "../../mln/value/hsv.hh"
#include "../../mln/value/circular.hh"

int main()
{
  using namespace mln;
  using namespace mln::value;

  image2d< hsv_< circular<16, 0, 360>, float, float > > ima;

}
