#include <mln/value/int_u.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/transform/distance_front.hh>
#include <mln/make/w_window2d_int.hh>
#include <mln/win/rectangle2d.hh>

#include <mln/morpho/closing/structural.hh>
#include <mln/morpho/closing/area.hh>
#include <mln/morpho/elementary/dilation.hh>
#include <mln/morpho/watershed/flooding.hh>



namespace mln
{


  image2d<bool> // Fwd decl.
  reduce_negate(const image2d<bool>& input);



  template <typename L>
  image2d<L>
  line_to_words(const image2d<bool>& input, L& n_words)
  {
    // Parameters.

    const unsigned
      height = 5,
      width  = 11,
      l_area = 500;

    // end of Parameters.


    image2d<bool>
      sub = reduce_negate(input),
      clo = morpho::closing::structural(sub,
					win::rectangle2d(height,
							 width));

    int ww[] = { 00, 11,  0, 11,  0,
		 11,  7,  5,  7, 11,
		 00,  5,  0,  5,  0,
		 11,  7,  5,  7, 11,
		 00, 11,  0, 11,  0 };

    typedef value::int_u<12> int_u12;
    image2d<int_u12>
      dmap = transform::distance_front(clo,
				       c4(), make::w_window2d_int(ww),
				       mln_max(int_u12)),
      dmap_ = morpho::closing::area(dmap, c4(), l_area);

    image2d<L> ws = morpho::elementary::dilation(morpho::watershed::flooding(dmap_,
									     c4(),
									     n_words),
						 c4());

    image2d<L> output(input.domain());

    {
      const int
	min_row = input.domain().pmin().row(),
	min_col = input.domain().pmin().col();

      mln_piter_(box2d) p(input.domain());
      for_all(p)
	if (input(p))
	  output(p) = 0;
	else
	  output(p) = ws.at_((p.row() - min_row) / 2,
			     (p.col() - min_col) / 2);
    }

    return output;
  }



  image2d<bool>
  reduce_negate(const image2d<bool>& input)
  {
    const int
      min_row = input.domain().pmin().row(),
      min_col = input.domain().pmin().col(),
      max_row = input.domain().pmax().row(),
      max_col = input.domain().pmax().col();

    image2d<bool> output(input.nrows() / 2,
			 input.ncols() / 2);

    for (int row = min_row; row <= max_row; row += 2)
      for (int col = min_col; col <= max_col; col += 2)
	{
	  unsigned count = 0;
	  if (input.at_(row,     col)     == false) ++count;
	  if (input.at_(row,     col + 1) == false) ++count;
	  if (input.at_(row + 1, col)     == false) ++count;
	  if (input.at_(row + 1, col + 1) == false) ++count;
	  output.at_((row - min_row) / 2,
		     (col - min_col) / 2) = (count >= 2); 
	}
    return output;
  }



} // ! mln
