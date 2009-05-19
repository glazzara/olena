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
#include <mln/value/float01_8.hh>

#include <mln/accu/sum.hh>
#include <mln/accu/mean.hh>
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
#include <mln/level/compute.hh>
#include <mln/level/convert.hh>
#include <mln/level/stretch.hh>
#include <mln/make/image2d.hh>
#include <mln/make/w_window1d.hh>
#include <mln/math/diff_abs.hh>
#include <mln/morpho/dilation.hh>
#include <mln/morpho/erosion.hh>
#include <mln/morpho/closing/area.hh>
#include <mln/morpho/closing/structural.hh>
#include <mln/morpho/opening/structural.hh>
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

const float saturation = 1.0;



namespace mln
{

  struct int_u12_from_float : Function_v2v< int_u12_from_float >
  {
    typedef value::int_u12 result;
    result operator()(float f) const
    {
      mln_precondition(f >= 0.f && f <= 1.f);
      unsigned i = f / saturation * 4095;
      return i > 4095 ? 4095 : i;
    }
  };


  template <typename I>
  void io_save_edges_int_u12(const I& input,
		     value::int_u8 bg,
		     const std::string& filename)
  {
    mlc_equal(mln_value(I), value::int_u12)::check();
    mln_ch_value(I, value::int_u8) output;
    initialize(output, input);
    arith::div_cst(input, 16, output);
    io::pgm::save(world::inter_pixel::display_edge(output.unmorph_(),
						   bg,
						   3),
		  filename);
  }

} // end of namespace mln



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
    float sum_v1 = level::compute(accu_sum, tmp_ima);

    convert::from_to(v2, tmp_ima2);
    float sum_v2 = level::compute(accu_sum, tmp_ima2);

    if (sum_v1 == 0 && sum_v2 == 0)
      return 1;

    res /= std::max(sum_v1, sum_v2);
    res = 1 - res;
    res = res * 4095;

    return (int) res;
  }
} dist;


/*struct dist_morpho_t : Function_vv2v<dist_morpho_t>
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
    float sum_v1 = level::compute(accu_sum, morpho_ima);

    image1d<V> tmp_ima2;
    convert::from_to(v2, tmp_ima2);
    image1d<float> morpho_ima2 = mean_image(tmp_ima2);
    float sum_v2 = level::compute(accu_sum, morpho_ima2);

    mln_piter(image1d<float>) p(morpho_ima.domain());
    for_all(p)
      res += std::min(morpho_ima(p), morpho_ima2(p));

    if (sum_v1 == 0 && sum_v2 == 0)
      return 1;

    res /= std::max(sum_v1, sum_v2);

    return 1 - res;
  }
} dist_morpho;*/






int usage(const char* bin)
{
  std::cout << "Usage: " << bin << " input.dump min smooth lambda" << std::endl;
  return 1;
}

int main(int argc, char* argv[])
{
  if (argc != 5)
    return usage(argv[0]);

  float min = atof(argv[2]);
  unsigned is_smooth = atoi(argv[3]);
  unsigned lambda = atoi(argv[4]);


  // Initialization.
  typedef float input_type;
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
      ima_arr(p).append(tmp_slice(p) - min); // We set the minimum value to 0.
  }


  // Edges image creation.
  typedef image_if<I, world::inter_pixel::is_pixel> Ix;
  Ix imax = world::inter_pixel::immerse(ima_arr);

  // Edges distance computation.
  image_if<image2d<int_u12>, world::inter_pixel::is_separator> edges;
  //if (!is_smooth)
    edges = world::inter_pixel::compute(imax, dist);
  //else
  //  edges = world::inter_pixel::compute(imax, dist_morpho);
  //io::dump::save(edges.unmorph_(), "edges.dump");
  //mln_VAR(d, level::transform(edges, int_u12_from_float()));
  io_save_edges_int_u12(edges, 0, "dist.pgm");


  // Closing.
  /*mln_VAR(d_clo, morpho::closing::area(d, world::inter_pixel::e2e(), lambda));
  //io_save_edges_int_u12(d_clo, 0, "d_clo.pgm");


  // Watershed.
  typedef label_16 L;
  L nbasins;
  mln_VAR(wst, morpho::watershed::flooding(d_clo, world::inter_pixel::e2e(), nbasins));

  std::cout << "nbasins: " << nbasins << std::endl;*/


  /*mln_VAR(w_all, wst.unmorph_());
  io::dump::save(w_all, "watershed_edges.dump");
  //data::fill((w | (!world::inter_pixel::is_separator())).rw(), nbasins.next());
  mln_VAR(w_pixels, w_all | world::inter_pixel::is_pixel());
  data::paste(morpho::dilation(extend(w_pixels, pw::value(w_all)), c4().win()), w_all);
  // edges -> dots
  mln_VAR(w_dots, w_all | world::inter_pixel::dim2::is_dot());
  data::paste(morpho::erosion(extend(w_dots, pw::value(w_all)), c4().win()), w_all);

  //io::ppm::save(labeling::colorize(value::rgb8(), w, nbasins.next()), "result.ppm");
  io::pgm::save(labeling::wrap(int_u8(), w_all), "watershed.pgm");*/



  // Mean distance.

  /*accu::mean<E_TYPE> accu_mean;
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
  }*/

  /*typedef accu::mean<int_u12,float,int_u12> A;
  util::array<int_u12> m = labeling::compute(A(), d, wst, nbasins);

  {
    util::array<int_u8> m_(nbasins.next());
    m_[0] = 1; // watershed line <=> 1
    for (unsigned l = 1; l <= nbasins; ++l)
    {
      m_[l] = m[l] / 16;
      if (m_[l] < 2) m_[l] == 2;
      // basin <=> 2..255
    }
    mln_VAR(d_m, level::transform(wst, m_));
    mln_VAR(out, world::inter_pixel::display_edge(d_m.unmorph_(),
	  0, // background <=> 0
	  3));
    io::pgm::save(out, "dist_mean.pgm");
  }*/



  // Deviation.
  /*util::array<accu::stat::deviation<float> > arr_dev;
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
  }*/



  // Plots labels.
  /*image2d<L> w_simple = world::inter_pixel::full2image(w_all);
  plot_label(input, w_simple, 191u);
  plot_label(input, w_simple, 171u);
  plot_label(input, w_simple, 188u);

  plot_label(input, w_simple, 16u);

  plot_label(input, w_simple, 187u);*/









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
