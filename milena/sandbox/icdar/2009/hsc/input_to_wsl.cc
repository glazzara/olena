#include <set>
#include <vector>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/pw/all.hh>
#include <mln/data/fill.hh>
#include <mln/level/saturate.hh>
#include <mln/level/convert.hh>
#include <mln/arith/revert.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <mln/literal/colors.hh>
#include <mln/debug/colorize.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/world/binary_2d/subsample.hh>

#define MLN_FLOAT double
#include <sandbox/theo/exec/gaussian_directional_2d.hh>

#include <mln/morpho/closing/structural.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/win/rectangle2d.hh>

#include <sandbox/icdar/2009/hsc/ws_to_wsl.hh>





void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm output.pgm [output.ppm]" << std::endl
	    << "  HSC @ ICDAR'2009" << std::endl
	    << "  input.pbm:  input 2D binary image (text is black; background is white)" << std::endl
	    << "  output.pgm: output image where line components are labeled (int_u8)" << std::endl
	    << "              0 is the background label." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::rgb8;

  if (argc != 3 && argc != 4)
    usage(argv);


  // Parameters.

  const unsigned
    subsampling_factor = 4,
    height = 5,
    width = 25,
    n_min_stats = 1000;
  const float
    h_sigma = 31,
    v_sigma = 1.3;

  // end of Parameters.


  trace::entering("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  // Sub-sampling.
  image2d<int_u8>
    small = world::binary_2d::subsample(input, subsampling_factor),
    fuzzy, clo,
    ws,
    spc;


# ifdef LOG
  io::pgm::save(small, "tmp_small.pgm");
# endif


  // Fuzzifying.
  {
    image2d<MLN_FLOAT> temp(small.domain()), out;
    data::fill(temp, small);

    out = linear::gaussian_directional_2d(temp, 1, h_sigma, 0);
    out = linear::gaussian_directional_2d(out,  0, v_sigma, 0);

    fuzzy = level::saturate(int_u8(), out);

# ifdef LOG
    io::pgm::save(fuzzy, "tmp_fuzzy.pgm");
# endif
  }


  clo = morpho::closing::structural(fuzzy, win::rectangle2d(height, width));

# ifdef LOG
    io::pgm::save(clo, "tmp_clo.pgm");
# endif

  int_u8 n_basins;
  ws = morpho::watershed::flooding(clo, c4(), n_basins);

# ifdef LOG
    io::pgm::save(ws, "tmp_ws.pgm");
    io::ppm::save(debug::colorize(rgb8(), ws, n_basins), "tmp_ws.ppm");
# endif


  image2d<int_u8> wsl = ws_to_wslines(input, small, ws, n_basins);

  io::pgm::save(wsl, argv[2]);

  if (argc == 4)
    {
      io::ppm::save(debug::colorize(rgb8(), wsl, n_basins),
		    argv[3]);
    }

  trace::exiting("main");
}
