#include <algorithm>

#include <mln/core/image/image1d.hh>
#include <mln/core/alias/window1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/core/routine/extend.hh>
#include <mln/core/var.hh>

#include <mln/io/dump/all.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/label_16.hh>

#include <mln/accu/sum.hh>
#include <mln/accu/mean.hh>
#include <mln/accu/image/all.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/convert/from_to.hh>
#include <mln/fun/v2v/fit.hh>
#include <mln/labeling/compute.hh>
#include <mln/labeling/wrap.hh>
#include <mln/data/compute.hh>
#include <mln/data/convert.hh>
#include <mln/data/stretch.hh>
#include <mln/linear/convolve.hh>
#include <mln/make/image2d.hh>
#include <mln/make/w_window1d.hh>
#include <mln/math/diff_abs.hh>
#include <mln/morpho/dilation.hh>
#include <mln/morpho/erosion.hh>
#include <mln/morpho/closing/structural.hh>
#include <mln/morpho/closing/area.hh>
#include <mln/morpho/opening/structural.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/pw/all.hh>
#include <mln/util/array.hh>

#include <mln/world/inter_pixel/display_edge.hh>
#include <mln/world/inter_pixel/compute.hh>
#include <mln/world/inter_pixel/immerse.hh>
#include <mln/world/inter_pixel/neighb2d.hh>
#include <mln/world/inter_pixel/all.hh>

#include "../plot_label.hh"


using namespace mln;
using value::int_u8;
using value::int_u12;
using value::label_16;

#define SATURATION 0.5




// Mean image.
//------------
template <typename V>
inline
image1d<float>
mean_image(image1d<V>& input)
{
  window1d win_1;
  win_1
    .insert(-2)
    .insert(-1)
    .insert(0)
    .insert(1)
    .insert(2);

  image1d<V> closing_ima = morpho::closing::structural(input, win_1);
  image1d<V> opening_ima = morpho::opening::structural(input, win_1);

  image1d<accu::mean<float> > result;

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
  typedef float result;

  template <typename V>
  float operator()(util::array<V> v1, util::array<V> v2) const
  {
    float res = 0.f;

    accu::sum<V> accu_sum;

    image1d<V> tmp_ima;
    convert::from_to(v1, tmp_ima);
    image1d<float> morpho_ima = mean_image(tmp_ima);
    float sum_v1 = data::compute(accu_sum, morpho_ima);

    image1d<V> tmp_ima2;
    convert::from_to(v2, tmp_ima2);
    image1d<float> morpho_ima2 = mean_image(tmp_ima2);
    float sum_v2 = data::compute(accu_sum, morpho_ima2);

    mln_piter(image1d<float>) p(morpho_ima.domain());
    for_all(p)
      res += std::min(morpho_ima(p), morpho_ima2(p));

    res /= std::max(sum_v1, sum_v2);

    return 1 - res;
  }
} dist;








int usage(const char* bin)
{
  std::cout << "Usage: " << bin << " input.dump closing_lambda" << std::endl;
  return 1;
}

int main(int argc, char* argv[])
{
  if (argc != 3)
    return usage(argv[0]);


  // Initialization.
  image3d<int_u12> input;
  io::dump::load(input, argv[1]);
  typedef image2d<util::array<int_u12> > I;
  I ima_arr;
  initialize(ima_arr, slice(input, 0));
  for (unsigned int i = 0; i < input.nslices(); ++i)
  {
    image2d<int_u12> tmp_slice = duplicate(slice(input, i));
    mln_piter_(image2d<int_u12>) p(tmp_slice.domain());
    for_all(p)
      ima_arr(p).append(tmp_slice(p));
  }


  // Edges image creation.
  typedef image_if<I, world::inter_pixel::is_pixel> Ix;
  Ix imax = world::inter_pixel::immerse(ima_arr);

  // Edges distance computation.
  mln_VAR(edges, world::inter_pixel::compute(imax, dist));
  mln_VAR(e, data::transform(edges, fun::v2v::fit<float>(SATURATION)));
  io::dump::save(e.unmorph_(), "edges_int_u12.dump");
  typedef int_u12 E_TYPE;

  {
    // Display.
    mln_VAR(display_ima, world::inter_pixel::display_edge(e.unmorph_(), 0.0, 3));
    io::pgm::save(data::stretch(int_u8(), display_ima), "01_edges.pgm");
  }



  // Closing.
  mln_VAR(clo, morpho::closing::area(e, world::inter_pixel::e2e(), atoi(argv[2])));

  {
    // Display.
    mln_VAR(display_clo, world::inter_pixel::display_edge(clo.unmorph_(), 0.0, 3));
    io::pgm::save(data::stretch(int_u8(), display_clo), "03_closing.pgm");
  }


  // Watershed.
  typedef label_16 L;
  L nbasins;
  mln_VAR(wst, morpho::watershed::flooding(clo, world::inter_pixel::e2e(), nbasins));

  std::cout << "nbasins: " << nbasins << std::endl;


  mln_VAR(w_all, wst.unmorph_());
  io::dump::save(w_all, "watershed_edges.dump");
  //data::fill((w | (!world::inter_pixel::is_separator())).rw(), nbasins.next());
  mln_VAR(w_pixels, w_all | world::inter_pixel::is_pixel());
  data::paste(morpho::dilation(extend(w_pixels, pw::value(w_all)), c4().win()), w_all);
  // edges -> dots
  mln_VAR(w_dots, w_all | world::inter_pixel::dim2::is_dot());
  data::paste(morpho::erosion(extend(w_dots, pw::value(w_all)), c4().win()), w_all);

  //io::ppm::save(labeling::colorize(value::rgb8(), w, nbasins.next()), "result.ppm");
  io::pgm::save(labeling::wrap(int_u8(), w_all), "watershed.pgm");



  // Mean distance.

  // Old code.
  //accu::mean<E_TYPE> accu_mean;
  //util::array<float> means = labeling::compute(accu_mean, e, wst, nbasins);

  // Theo version.
  typedef accu::mean<int_u12,float,int_u12> A;
  util::array<int_u12> means = labeling::compute(A(), e, wst, nbasins);



  // Display.
  {
    typedef image_if<image2d<float>, world::inter_pixel::is_separator> Fsx;
    Fsx ima_means;
    initialize(ima_means, wst);
    data::paste(wst, ima_means);
    for (unsigned i = 1; i < means.nelements(); ++i)
      data::fill((ima_means | pw::value(ima_means) == pw::cst(i)).rw(), means[i]);
    mln_VAR(display_means, world::inter_pixel::display_edge(ima_means.unmorph_(), 0.0, 3));
    io::pgm::save(data::stretch(int_u8(), display_means), "04_means.pgm");
  }




  return 0;
}
