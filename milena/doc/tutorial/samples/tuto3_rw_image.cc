#include <mln/essential/2d.hh>
#include <tests/data.hh>
#include <doc/tutorial/tools/sample_utils.hh>
int main()
{
  using namespace mln;

  // \{
  image2d<value::rgb16> ima(40, 40);
  // \}

  // \{
  data::fill(ima, literal::red);
  // \}

  // \{
  for (def::coord row = 20; row < 30; ++row)
    for (def::coord col = 20; col < 30; ++col)
      ima(point2d(row, col)) = literal::blue;
  // \}

  // \{
  for (def::coord row = 20; row < 30; ++row)
    for (def::coord col = 20; col < 30; ++col)
      opt::at(ima, row, col) = literal::blue;
  // \}
  doc::ppmsave(ima, "tuto3_rw_image");

  image2d<value::rgb16> ima2 = duplicate(ima);

  // \{
  image2d<value::rgb16> lena;
  io::ppm::load(lena, MLN_IMG_DIR "/small.ppm");
  // \}

  // \{
  data::fill(ima, lena);
  // \}
  doc::ppmsave(ima, "tuto3_rw_image");

  ima = ima2;
  // \{
  data::paste(ima, lena);
  // \}
  doc::ppmsave(lena, "tuto3_rw_image");
}
