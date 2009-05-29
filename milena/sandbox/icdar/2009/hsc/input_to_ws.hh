#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/data/fill.hh>
#include <mln/data/saturate.hh>

#include <mln/value/int_u8.hh>

#define MLN_FLOAT double
#include <sandbox/theo/exec/gaussian_directional_2d.hh>

#include <mln/win/rectangle2d.hh>
#include <mln/morpho/closing/structural.hh>
#include <mln/morpho/watershed/flooding.hh>

#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/literal/colors.hh>
#include <mln/labeling/colorize.hh>
#include <mln/data/convert.hh>
#include <mln/labeling/wrap.hh>
#include <mln/pw/all.hh>
#include <mln/core/image/dmorph/image_if.hh>


namespace mln
{


  template <typename L>
  image2d<L>
  input_to_ws(const image2d<bool>& input,
	      const image2d<value::int_u8>& small,
	      L& n_lines)
  {

    // Parameters.

    const unsigned
      height = 5,
      width = 25;

    const float
      h_sigma = 31,
      v_sigma = 1.3;

    // end of Parameters.


    // Sub-sampling.
    image2d<value::int_u8> fuzzy, clo;


    // Fuzzifying.
    {
      image2d<MLN_FLOAT> temp(small.domain()), out;
      data::fill(temp, small);

      out = linear::gaussian_directional_2d(temp, 1, h_sigma, 0);
      out = linear::gaussian_directional_2d(out,  0, v_sigma, 0);

      fuzzy = data::saturate(value::int_u8(), out);

#ifdef LOG
    io::pgm::save(small, "temp_small.pgm");
    io::pgm::save(fuzzy, "temp_fuzzy.pgm");
#endif // LOG

    }

    // Closing.
    clo = morpho::closing::structural(fuzzy, win::rectangle2d(height, width));

    // Watershed transform.
    image2d<L> ws = morpho::watershed::flooding(clo, c4(), n_lines);


# ifdef LOG
    {
      // Watershed line (red) over small image.
      image2d<value::rgb8> cool = data::convert(value::rgb8(), small);
      data::fill((cool | (pw::value(ws) == pw::cst(0))).rw(),
		 literal::red);
      io::ppm::save(cool, "temp_small_ws.ppm");
    }
    {
      // Watershed line (red) over fuzzy image.
      image2d<value::rgb8> cool = data::convert(value::rgb8(), fuzzy);
      data::fill((cool | (pw::value(ws) == pw::cst(0))).rw(),
		 literal::red);
      io::ppm::save(cool, "temp_fuzzy_ws.ppm");
    }
    {
      // Watershed basins colorize the small image.
      image2d<value::rgb8> cool = labeling::colorize(value::rgb8(), ws, n_lines);
      data::fill((cool | (pw::value(ws) == pw::cst(0))).rw(),
		 literal::white);
      mln_piter(box2d) p(small.domain());
      for_all(p)
      {
	if (small(p) == 255)
	  continue;
	float f = float(small(p)) / 255;
	cool(p).red()   = cool(p).red()   * f;
	cool(p).green() = cool(p).green() * f;
	cool(p).blue()  = cool(p).blue()  * f;
      }
      io::ppm::save(cool, "temp_ws.ppm");
    }
# endif

    return ws;
  }


} // mln
