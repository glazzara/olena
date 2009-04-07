#ifndef ICDAR2PPM_HH
# define ICDAR2PPM_HH

# include <mln/core/image/image2d.hh>
# include <mln/value/label_8.hh>
# include <mln/value/rgb8.hh>
# include <mln/level/compute.hh>
# include <mln/accu/count_labels.hh>
# include <mln/debug/colorize.hh>
# include <mln/io/ppm/save.hh>
# include <mln/io/pbm/load.hh>

# include "io/icdar/load.hh"

namespace mln
{

  void
  icdar2ppm(const std::string& pbmfile, const std::string& icdarfile,
	    const std::string& ppmfile)
  {
    image2d<bool> ref;
    io::pbm::load(ref, pbmfile);

    image2d<value::label_8> ima;
    io::icdar::load(ima, icdarfile, ref.nrows(), ref.ncols());

    value::label_8 nlines = level::compute(accu::meta::count_labels(), ima);

    io::ppm::save(debug::colorize(value::rgb8(), ima, nlines), ppmfile);
  }

}

#endif // ! ICDAR2PPM_HH
