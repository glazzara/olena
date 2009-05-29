#include <mln/core/image/image2d.hh>
#include <mln/border/fill.hh>
#include <mln/make/image.hh>
#include <mln/debug/println_with_border.hh>
#include <mln/border/thickness.hh>

int main()
{
  using namespace mln;

  // \{
  bool vals[3][3] = { { 0, 1, 1 },
		      { 1, 0, 0 },
		      { 1, 1, 0 } };

  image2d<bool> ima_def = make::image(vals);
  border::fill(ima_def, false);
  debug::println_with_border(ima_def);

  std::cout << "===========" << std::endl << std::endl;

  border::thickness = 0;
  image2d<bool> ima_bt0 = make::image(vals);
  debug::println_with_border(ima_bt0);
  // \}

}
