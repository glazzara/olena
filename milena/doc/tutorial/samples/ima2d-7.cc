#include <mln/essential/2d.hh>
int main()
{
  using namespace mln;

  // \{
  image2d<value::int_u8> img2a(2, 3);
  image2d<value::int_u8> img2b;

  initialize(img2b, img2a);
  level::fill(img2b, img2a);
  // \}
}
