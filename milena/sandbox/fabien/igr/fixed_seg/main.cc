#include <algorithm>

#include <mln/core/image/image1d.hh>
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
#include <mln/value/float01_8.hh>

#include <mln/accu/sum.hh>
#include <mln/accu/mean.hh>
#include <mln/accu/stat/deviation.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/debug/quiet.hh>
#include <mln/convert/from_to.hh>
#include <mln/fun/v2v/fit.hh>
#include <mln/labeling/compute.hh>
#include <mln/labeling/wrap.hh>
#include <mln/level/compute.hh>
#include <mln/level/convert.hh>
#include <mln/level/stretch.hh>
#include <mln/make/image2d.hh>
#include <mln/math/diff_abs.hh>
#include <mln/morpho/dilation.hh>
#include <mln/morpho/erosion.hh>
#include <mln/morpho/closing/area.hh>
#include <mln/morpho/elementary/gradient.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/pw/all.hh>
#include <mln/util/array.hh>

#include <mln/world/inter_pixel/display_edge.hh>
#include <mln/world/inter_pixel/compute.hh>
#include <mln/world/inter_pixel/immerse.hh>
#include <mln/world/inter_pixel/neighb2d.hh>
#include <mln/world/inter_pixel/all.hh>

#include <mln/labeling/colorize.hh>
#include <mln/debug/println.hh>
#include <mln/trace/quiet.hh>

#include "../plot_label.hh"

using namespace mln;
using value::int_u8;
using value::int_u12;
using value::label_16;
using value::float01_8;

#define SATURATION 0.6





// Distance function.
//-------------------

struct dist_t : Function_vv2v<dist_t>
{
  typedef float result;

  template <typename V>
  float operator()(util::array<V> v1, util::array<V> v2) const
  {
    float res = 0.f;

    for (unsigned i = 0; i < v1.nelements(); ++i)
      res += std::min(v1[i], v2[i]);

    image1d<V> tmp_ima;
    image1d<V> tmp_ima2;
    accu::sum<V> accu_sum;

    convert::from_to(v1, tmp_ima);
    float sum_v1 = level::compute(accu_sum, tmp_ima);

    convert::from_to(v2, tmp_ima2);
    float sum_v2 = level::compute(accu_sum, tmp_ima2);

    res /= std::max(sum_v1, sum_v2);

    return 1 - res;
  }
} dist;








int usage(const char* bin)
{
  std::cout << "Usage: " << bin << " input.dump closing" << std::endl;
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
  mln_VAR(e, level::transform(edges, fun::v2v::fit<float>(SATURATION)));
  typedef int_u12 E_TYPE;

  {
    // Display.
    mln_VAR(display_ima, world::inter_pixel::display_edge(e.unmorph_(), 0.0, 3));
    io::pgm::save(level::stretch(int_u8(), display_ima), "01_edges.pgm");
  }



  // Closing.
  mln_VAR(clo, morpho::closing::area(e, world::inter_pixel::e2e(), atoi(argv[2])));

  {
    // Display.
    mln_VAR(display_clo, world::inter_pixel::display_edge(clo.unmorph_(), 0.0, 3));
    io::pgm::save(level::stretch(int_u8(), display_clo), "03_closing.pgm");
  }


  // Watershed.
  typedef label_16 L;
  L nbasins;
  mln_VAR(wst, morpho::watershed::flooding(clo, world::inter_pixel::e2e(), nbasins));

  std::cout << "nbasins: " << nbasins << std::endl;


  mln_VAR(w_all, wst.unmorph_());
  //data::fill((w | (!world::inter_pixel::is_separator())).rw(), nbasins.next());
  mln_VAR(w_pixels, w_all | world::inter_pixel::is_pixel());
  data::paste(morpho::dilation(extend(w_pixels, pw::value(w_all)), c4().win()), w_all);
  // edges -> dots
  mln_VAR(w_dots, w_all | world::inter_pixel::dim2::is_dot());
  data::paste(morpho::erosion(extend(w_dots, pw::value(w_all)), c4().win()), w_all);

  //io::ppm::save(labeling::colorize(value::rgb8(), w, nbasins.next()), "result.ppm");
  io::pgm::save(labeling::wrap(int_u8(), w_all), "watershed.pgm");



  // Mean distance.
  accu::mean<E_TYPE> accu_mean;
  util::array<float> means = labeling::compute(accu_mean, e, wst, nbasins);

  // Display.
  {
    typedef image_if<image2d<float>, world::inter_pixel::is_separator> Fsx;
    Fsx ima_means;
    initialize(ima_means, wst);
    data::paste(wst, ima_means);
    for (unsigned i = 1; i < means.nelements(); ++i)
      data::fill((ima_means | pw::value(ima_means) == pw::cst(i)).rw(), means[i]);
    mln_VAR(display_means, world::inter_pixel::display_edge(ima_means.unmorph_(), 0.0, 3));
    io::pgm::save(level::stretch(int_u8(), display_means), "04_means.pgm");
  }



  // Deviation.
  util::array<accu::stat::deviation<float> > arr_dev;
    for (unsigned i = 0; i < means.nelements(); ++i)
      arr_dev.append(accu::stat::deviation<float> (means[i]));
  util::array<float> deviations = labeling::compute(arr_dev, e, wst, nbasins);

  // Display.
  {
    typedef image_if<image2d<float>, world::inter_pixel::is_separator> Fsx;
    Fsx ima_dev;
    initialize(ima_dev, wst);
    data::paste(wst, ima_dev);
    for (unsigned i = 1; i < deviations.nelements(); ++i)
      data::fill((ima_dev | pw::value(ima_dev) == pw::cst(i)).rw(), deviations[i]);
    mln_VAR(display_dev, world::inter_pixel::display_edge(ima_dev.unmorph_(), 0.0, 3));
    io::pgm::save(level::stretch(int_u8(), display_dev), "05_dev.pgm");
  }



  // Plots labels.
  image2d<L> w_simple = world::inter_pixel::full2image(w_all);
  plot_label(input, w_simple, 191u);
  plot_label(input, w_simple, 171u);
  plot_label(input, w_simple, 188u);

  plot_label(input, w_simple, 16u);

  plot_label(input, w_simple, 187u);









  /*mln_VAR(clo, morpho::closing::volume(edges | world::inter_pixel::dim2::is_edge(), world::inter_pixel::e2e(), atoi(argv[2])));

  // Debug.
  //debug::println("clo", clo);

  // Display.
  image2d<float> display_ima2 = world::inter_pixel::display_edge(clo.unmorph_(), 0.0, 3);
  io::pgm::save(level::stretch(int_u8(), display_ima2), "edges2.pgm");

  // Watershed.
  typedef label_16 L;
  L nbasins;
  mln_VAR(wst, morpho::watershed::flooding(clo, world::inter_pixel::e2e(), nbasins));

  // Debug.
  //debug::println("wst", wst);

  // Extension.
  image2d<L> w_all = wst.unmorph_();
  // edges -> pixel
  mln_VAR(w_pixels, w_all | world::inter_pixel::dim2::is_pixel());
  data::paste(morpho::dilation(extend(w_pixels, pw::value(w_all)), c4().win()), w_all);
  // edges -> dots
  mln_VAR(w_dots, w_all | world::inter_pixel::dim2::is_dot());
  data::paste(morpho::erosion(extend(w_dots, pw::value(w_all)), c4().win()), w_all);


  // Save labels map.
  std::cout << "nbasins: " << nbasins << std::endl;
  io::dump::save(wst.unmorph_(), "watershed_fixed.dump");*/

  return 0;
}
