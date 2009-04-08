#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/data/fill.hh>
#include <mln/level/saturate.hh>

#include <mln/value/int_u8.hh>

#define MLN_FLOAT double
#include <sandbox/theo/exec/gaussian_directional_2d.hh>

#include <mln/win/rectangle2d.hh>

#include <mln/morpho/closing/structural.hh>
#include <mln/morpho/watershed/flooding.hh>


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

      fuzzy = level::saturate(value::int_u8(), out);
    }

    // Closing.
    clo = morpho::closing::structural(fuzzy, win::rectangle2d(height, width));

    // Watershed transform.
    image2d<L> ws = morpho::watershed::flooding(clo, c4(), n_lines);

    return ws;
  }


} // mln
