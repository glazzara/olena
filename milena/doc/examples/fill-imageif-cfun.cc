#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/var.hh>

#include <mln/make/image.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/label_8.hh>
#include <mln/literal/colors.hh>

#include <mln/data/fill.hh>

#include <doc/tools/sample_utils.hh>
// \{
bool row_oddity(mln::point2d p)
{
  return p.row() % 2;
}
// \}
int main()
{
  using namespace mln;
  using value::rgb8;
  using value::label_8;


  // \{
  bool vals[6][5] = {
      {0, 1, 1, 0, 0},
      {0, 1, 1, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 1, 0, 1, 0},
      {1, 0, 1, 1, 1},
      {1, 0, 0, 0, 0}
  };
  image2d<bool> ima = make::image(vals);
  // \}

  // Create a black image from ima.
  // Fill sites being part of component 2 with red.
  // \{
  image2d<rgb8> ima2;
  initialize(ima2, ima);
  data::fill(ima2, literal::black);

  data::fill((ima2 | row_oddity).rw(), literal::red);
  // \}

  doc::ppmsave(ima2, "fill-imageif-cfun");
}
