#include <mln/core/image/image2d.hh>
#include <mln/make/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/debug/colorize.hh>
#include <doc/tools/sample_utils.hh>

int main()
{
  using namespace mln;
  using namespace mln::value;

  // \{
  int_u8 vals[25] = { 100, 100, 200, 200, 230,
		      100, 100, 200, 230, 230,
		      140, 140, 140,   0,   0,
		       65, 186, 65, 127, 127,
		       65,  65, 65, 127, 127 };

  image2d<int_u8> ima = make::image2d(vals);
  image2d<rgb8> ima_color = debug::colorize(rgb8(), ima, 230);
  // \}

  doc::pgmsave(ima, "colorize");
  doc::ppmsave(ima_color, "colorize");
}
