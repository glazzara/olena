#include <sandbox/icdar/2009/hsc/clean_input.hh>
#include <sandbox/icdar/2009/hsc/input_to_ws.hh>
#include <sandbox/icdar/2009/hsc/ws_to_wsl.hh>
#include <sandbox/icdar/2009/hsc/clean_lines.hh>

#include <mln/world/binary_2d/subsample.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/label.hh>
#include <mln/labeling/pack.hh>
#include <mln/core/image/vmorph/violent_cast_image.hh>

#include <mln/io/pgm/save.hh>


namespace mln
{

  template <typename L>
  image2d<L>
  input_to_lines(const image2d<bool>& raw_input, L& n_lines,
		 float tau)
  {
    // First clean.
    image2d<bool> input = clean_input(raw_input);

    // Sub-sample.
    image2d<value::int_u8> small = world::binary_2d::subsample(input, 4);
    
    typedef value::int_u<12> L_reg;
    L_reg n_reg;
    image2d<L_reg> ws, wsl;

    // Get text regions.
    ws  = input_to_ws(input, small, n_reg);

    // Merge regions.
    wsl = ws_to_wslines(input, small, ws, n_reg);
    
    // Clean lines
    wsl = clean_lines(input, wsl, tau);
    
    return labeling::pack(violent_cast_image_<L>(wsl),
			  n_lines);
  }

} // mln
