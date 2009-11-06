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
#include <mln/value/rgb8.hh>

#include <mln/accu/math/sum.hh>
#include <mln/accu/stat/mean.hh>
#include <mln/accu/image/all.hh>
#include <mln/accu/stat/deviation.hh>
#include <mln/arith/div.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/debug/quiet.hh>
#include <mln/convert/from_to.hh>
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
#include <mln/morpho/closing/volume.hh>
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

#include "plot_points/int2rgb.hh"


using namespace mln;
using value::int_u8;
using value::int_u12;
using value::label_16;

const float saturation = 1.0;



namespace mln
{

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

  template <typename I>
  void io_save_edges_float(const I& input,
			   value::int_u8 bg,
			   const std::string& filename)
  {
    mln_ch_value(I, value::int_u8) output = data::stretch(int_u8(), input);
    io::pgm::save(world::inter_pixel::display_edge(output.unmorph_(),
						   bg,
						   3),
		  filename);
  }

} // end of namespace mln







int usage(const char* bin)
{
  std::cout << "Usage: " << bin << " dist_ima wst_ima nbasins" << std::endl;
  std::cout << "  dist_ima: distance 2D image in dump format encoded in int_u12" << std::endl;
  std::cout << "  wst_ima:  watershed 2D image in dump format encoded in label_16" << std::endl;
  std::cout << "  nbasins:  number of labels in wst_ima" << std::endl;
  return 1;
}

int main(int argc, char* argv[])
{
  if (argc != 4)
    return usage(argv[0]);

  label_16 nbasins = atoi(argv[3]);

  // Initialization.
  typedef float input_type;

  image2d<int_u12> dist_input;
  io::dump::load(dist_input, argv[1]);
  mln_VAR(dist, dist_input | world::inter_pixel::is_separator());

  image2d<label_16> wst_input;
  io::dump::load(wst_input, argv[2]);
  mln_VAR(wst, wst_input | world::inter_pixel::is_separator());



  // Mean distance.

  accu::stat::mean<input_type> accu_mean;
  util::array<float> means = labeling::compute(accu_mean, dist, wst, nbasins);

  // Display.
  {
    typedef image_if<image2d<float>, world::inter_pixel::is_separator> Fsx;
    Fsx ima_means;
    initialize(ima_means, wst);
    data::paste(wst, ima_means);
    for (unsigned i = 1; i < means.nelements(); ++i)
      data::fill((ima_means | pw::value(ima_means) == pw::cst(i)).rw(), (unsigned) means[i]);
    io_save_edges_float(ima_means, 0u, "means.pgm");
  }

  typedef accu::stat::mean<input_type> A;
  util::array<input_type> m = labeling::compute(A(), dist, wst, nbasins);

  {
    util::array<int_u8> m_(nbasins.next());
    m_[0] = 1; // watershed line <=> 1
    for (unsigned l = 1; l <= nbasins; ++l)
    {
      m_[l] = m[l] / 16;
      if (m_[l] < 2u)
	m_[l] = 2u;
      // basin <=> 2..255
    }
    mln_VAR(d_m, data::transform(wst, m_));
    io_save_edges_float(d_m, 0u, "dist_mean.pgm");
  }



  // Deviation.
  util::array<accu::stat::deviation<float> > arr_dev;
  for (unsigned i = 0; i < means.nelements(); ++i)
    arr_dev.append(accu::stat::deviation<float> (means[i]));
  util::array<float> deviations = labeling::compute(arr_dev, dist, wst, nbasins);

  // Display.
  {
    typedef image_if<image2d<float>, world::inter_pixel::is_separator> Fsx;
    Fsx ima_dev;
    initialize(ima_dev, wst);
    data::paste(wst, ima_dev);
    for (unsigned i = 1; i < deviations.nelements(); ++i)
      data::fill((ima_dev | pw::value(ima_dev) == pw::cst(i)).rw(), deviations[i]);
    io_save_edges_float(ima_dev, 0u, "dev.pgm");
  }


  return 0;
}
