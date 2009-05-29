#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/value/rgb8.hh>
#include <mln/io/ppm/load.hh>
#include <mln/literal/colors.hh>

#include <tests/data.hh>
#include <doc/tools/sample_utils.hh>

// \{
template <typename I>
void fill(I& ima, mln_value(I) v)
{
  mln_piter(I) p(ima.domain());
  for_all(p)
    ima(p) = v;
}
// \}

int main()
{
  using namespace mln;

  image2d<value::rgb8> ima;
  io::ppm::load(ima, MLN_IMG_DIR "/small.ppm");

  // \{
  box2d b(20,20);
  fill((ima | b).rw(), literal::green);
  // \}
  doc::ppmsave(ima, "fill");

  // \{
  fill(ima, literal::green);
  // \}
  doc::ppmsave(ima, "fill");
}
