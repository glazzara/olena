// Put into ligth the bug of mean_values_rgb

#include <mln/value/rgb.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/label_8.hh>

#include <mln/core/image/image2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/data/transform.hh>
#include <mln/data/compute.hh>

#include <mln/accu/math/sup.hh>
#include <mln/fun/v2v/rgb8_to_rgbn.hh>

#include <mln/trace/quiet.hh>

#include <mln/labeling/mean_values.hh>

#include <mln/img_path.hh>

void test_mean_values_rgb()
{
  typedef mln::value::rgb8       t_rgb8;
  typedef mln::value::rgb<5>     t_rgb5;
  typedef mln::value::label_8    t_lbl8;
  typedef mln::image2d<t_rgb5>   t_image2d_rgb5;
  typedef mln::image2d<t_rgb8>   t_image2d_rgb8;
  typedef mln::image2d<t_lbl8>   t_image2d_lbl8;

  t_image2d_rgb8 input_rgb8;
  t_image2d_rgb5 input_rgb5;
  t_image2d_lbl8 input_lbl8;
  t_image2d_rgb8 output_rgb8;
  t_lbl8         sup;
  unsigned       n_label;

  mln::io::ppm::load(input_rgb8, OLENA_IMG_PATH"/house.ppm");
  input_rgb5=mln::data::transform(input_rgb8, mln::fun::v2v::rgb8_to_rgbn<5>());

  mln::io::pgm::load(input_lbl8, "label.pgm");
  sup     = mln::data::compute(mln::accu::meta::math::sup(), input_lbl8);
  n_label = static_cast<unsigned>(sup) + 1;

  output_rgb8 = mln::labeling::mean_values(input_rgb8, input_lbl8, n_label);
  mln::io::ppm::save(output_rgb8, "output.ppm");
}

int main()
{
  mln::trace::quiet = false;

  test_mean_values_rgb();

  return 0;
}
