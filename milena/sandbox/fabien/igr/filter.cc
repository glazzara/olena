#include <algorithm>

#include <mln/core/image/image1d.hh>
#include <mln/core/alias/window1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/slice_image.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/core/routine/extend.hh>
#include <mln/core/var.hh>

#include <mln/io/dump/all.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/label_16.hh>
#include <mln/value/rgb8.hh>

#include <mln/accu/sum.hh>
#include <mln/accu/stat/mean.hh>
#include <mln/accu/image/all.hh>
#include <mln/accu/stat/deviation.hh>
#include <mln/arith/div.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/debug/quiet.hh>
#include <mln/convert/from_to.hh>
#include <mln/fun/v2v/fit.hh>
#include <mln/labeling/compute.hh>
#include <mln/labeling/wrap.hh>
#include <mln/data/compute.hh>
#include <mln/data/convert.hh>
#include <mln/data/stretch.hh>
#include <mln/make/image2d.hh>
#include <mln/make/w_window1d.hh>
#include <mln/math/diff_abs.hh>
#include <mln/morpho/dilation.hh>
#include <mln/morpho/erosion.hh>
#include <mln/morpho/closing/sum.hh>
#include <mln/morpho/closing/structural.hh>
#include <mln/morpho/opening/structural.hh>
#include <mln/morpho/elementary/gradient.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/pw/all.hh>
#include <mln/util/array.hh>
#include <mln/win/segment1d.hh>

#include <mln/world/inter_pixel/display_edge.hh>
#include <mln/world/inter_pixel/compute.hh>
#include <mln/world/inter_pixel/immerse.hh>
#include <mln/world/inter_pixel/neighb2d.hh>
#include <mln/world/inter_pixel/all.hh>

#include <mln/labeling/colorize.hh>
#include <mln/debug/println.hh>
#include <mln/trace/quiet.hh>


using namespace mln;
using value::int_u8;
using value::int_u12;
using value::label_16;




// Mean image.
//------------

template <typename V>
inline
image1d<float>
mean_image(image1d<V>& input, unsigned seg_size)
{
  win::segment1d seg(seg_size);

  image1d<V> closing_ima = morpho::closing::structural(input, seg);
  image1d<V> opening_ima = morpho::opening::structural(input, seg);

  image1d<accu::stat::mean<float> > result;

  initialize(result, input);

  accu::image::init(result);
  accu::image::take(result, closing_ima);
  accu::image::take(result, opening_ima);

  return accu::image::to_result(result);
}



// Distance function.
//-------------------

struct dist_t : Function_vv2v<dist_t>
{
  typedef int_u12 result;

  template <typename V>
  int_u12 operator()(util::array<V> v1, util::array<V> v2) const
  {
    float res = 0.f;

    for (unsigned i = 0; i < v1.nelements(); ++i)
      res += std::min(v1[i], v2[i]);

    image1d<V> tmp_ima;
    image1d<V> tmp_ima2;
    accu::sum<V> accu_sum;

    convert::from_to(v1, tmp_ima);
    float sum_v1 = data::compute(accu_sum, tmp_ima);

    convert::from_to(v2, tmp_ima2);
    float sum_v2 = data::compute(accu_sum, tmp_ima2);

    if (sum_v1 == 0 && sum_v2 == 0)
      return 1;

    res /= std::max(sum_v1, sum_v2);
    res = 1 - res;
    res = res * 4095;
    if (res > 4095)
      return 4095;

    return (int) res;
  }
} dist;


struct dist_morpho_t : Function_vv2v<dist_morpho_t>
{
  typedef int_u12 result;

  template <typename V>
  int_u12 operator()(util::array<V> v1, util::array<V> v2) const
  {
    float res = 0.f;

    accu::sum<V> accu_sum;

    image1d<V> tmp_ima;
    convert::from_to(v1, tmp_ima);
    image1d<float> morpho_ima = mean_image(tmp_ima, 15);
    morpho_ima = mean_image(morpho_ima, 11);
    morpho_ima = mean_image(morpho_ima, 7);
    float sum_v1 = data::compute(accu_sum, morpho_ima);

    image1d<V> tmp_ima2;
    convert::from_to(v2, tmp_ima2);
    image1d<float> morpho_ima2 = mean_image(tmp_ima2, 15);
    morpho_ima2 = mean_image(tmp_ima2, 11);
    morpho_ima2 = mean_image(tmp_ima2, 7);
    float sum_v2 = data::compute(accu_sum, morpho_ima2);

    mln_piter(image1d<float>) p(morpho_ima.domain());
    for_all(p)
      res += std::min(morpho_ima(p), morpho_ima2(p));

    if (sum_v1 == 0 && sum_v2 == 0)
      return 1;

    res /= std::max(sum_v1, sum_v2);
    res = 1 - res;
    res = res * 4095;
    if (res > 4095)
      return 4095;

    return res;
  }
} dist_morpho;






int usage(const char* bin)
{
  std::cout << "Usage: " << bin << " input.dump output.dump" << std::endl;
  return 1;
}

int main(int argc, char* argv[])
{
  if (argc != 3)
    return usage(argv[0]);

  // Initialization.
  typedef int_u12 input_type;
  image3d<input_type> input;
  io::dump::load(input, argv[1]);
  int min_slice = input.bbox().pmin().sli();
  int max_slice = input.bbox().pmax().sli();
  typedef image2d<util::array<input_type> > I;
  I ima_arr;
  initialize(ima_arr, slice(input, 0));
  for (int i = min_slice; i <= max_slice; ++i)
  {
    image2d<input_type> tmp_slice = duplicate(slice(input, i));
    mln_piter_(image2d<input_type>) p(tmp_slice.domain());
    for_all(p)
      ima_arr(p).append(tmp_slice(p));
  }


  image3d<float> output;
  initialize(output, input);

  mln_piter_(I) p(ima_arr.domain());
  for_all(p)
  {
    image1d<input_type> tmp_ima;
    convert::from_to(ima_arr(p), tmp_ima);
    image1d<float> morpho_ima = mean_image(tmp_ima, 15);
    morpho_ima = mean_image(morpho_ima, 11);
    morpho_ima = mean_image(morpho_ima, 7);

    for (int i = min_slice; i <= max_slice; ++i)
      output.at_(i, p.row(), p.col()) = morpho_ima.at_(i - min_slice);
  }


  io::dump::save(output, argv[2]);

  return 0;
}
