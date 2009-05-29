#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/data/fill.hh>

int main()
{
  using namespace mln;

  // \{
  image2d<char> imga(5, 5);
  // \}

  // \{
  data::fill(imga, 'a');
  // \}

  // \{
  data::fill((imga | box2d(1,2)).rw(), 'a');
  // \}

}
