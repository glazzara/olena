#include <mln/essential/2d.hh>
int main()
{
  using namespace mln;

  // \{
  image2d<unsigned char> imgb(make::box2d(5,5, 7,8));
  // Initialize imga with the same domain as imgb.
  image2d<unsigned char> imga(imgb.domain());

  // Initialize the image values.
  data::fill(imgb, 'b');

  // Paste the content of imgb in imga.
  data::paste(imgb, imga);

  debug::println(imga);
  // \}
}
