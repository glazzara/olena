#include <mln/core/image2d_b.hh>
#include <mln/core/sub_image.hh>
#include <mln/core/image_if_value.hh>
#include <mln/core/neighb2d.hh>
#include <mln/core/inplace.hh>

#include <mln/value/int_u8.hh>
#include <mln/level/fill.hh>
#include <mln/level/stretch.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pgm/save.hh>
#include <mln/core/mesh_image.hh>
# include <mln/labeling/base.hh>
# include <mln/debug/println.hh>
# include <mln/core/window2d.hh>
# include <mln/core/w_window2d_int.hh>
# include <mln/core/w_window2d_float.hh>
# include <mln/convert/to_window.hh>
# include <mln/core/concept/dpoint.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/window.hh>
# include <mln/pw/image.hh>
# include <mln/pw/cst.hh>
# include <mln/metal/is_a.hh>

# include <mln/core/image_if_interval.hh>
# include <mln/core/dpoint2d.hh>

namespace mln
{
  namespace win_chamfer
  {

    template<int d10, int d11>
    const w_window2d_int
    mk_chamfer_3x3_int()
    {
      int ws[] = { d11, d10, d11,
	           d10,   0,   0,
		     0,   0,   0 };

      return (make::w_window2d(ws));
    }

    template<int d10, int d11, int d21>
    const w_window2d_int
    mk_chamfer_5x5_int()
    {
      int ws[] = {   0, d21,   0, d21,   0,
         	   d21, d11, d10, d11, d21,
		     0, d10,   0, d10,   0,
	             0,   0,   0,   0,   0,
		     0,   0,   0,   0,   0 };

      return (make::w_window2d(ws));
    }

    const w_window2d_float
    mk_chamfer_3x3_float(float d10, float d11)
    {
      float ws[] = { d11, d10, d11,
      	             d10,   0,   0,
		       0,   0,   0 };

      return (make::w_window2d(ws));
    }

    const w_window2d_float
    mk_chamfer_5x5_float(float d10, float d11, float d21)
    {
      float ws[] = {   0, d21,   0, d21,   0,
         	     d21, d11, d10, d11, d21,
		       0, d10,   0, d10,   0,
	               0,   0,   0,   0,   0,
  		       0,   0,   0,   0,   0 };

      return (make::w_window2d(ws));
    }

  } // end of mln::win_chamfer

  template <typename I, typename W>
  mln_ch_value(I, unsigned)
  chamfer(const Image<I>& input_, const Weighted_Window<W>& w_win_,
	  unsigned max = mln_max(unsigned))
  {
    const I& input = exact(input_);
    const W& w_win = exact(w_win_);

    mln_ch_value(I, unsigned) output;
    initialize(output, input);

    /// Init.
    {
      level::fill(inplace(output | (input | true).domain()),  0);
      level::fill(inplace(output | (input | false).domain()), max);
    }

    /// Fwd pass.
    {
      mln_fwd_piter(I) p(input.domain());
      mln_qiter(W) q(w_win, p);

      for_all(p) if (input(p) == false)
	for_all(q) if (input.has(q))
	  if (output(q) != max
	      && output(q) + q.w() < output(p))
	    output(p) = output(q) + q.w();
    }

    /// Bkd pass.
    {

      W w_win_b = geom::sym(w_win);

      mln_bkd_piter(I) p(input.domain());
      mln_qiter(W) q(w_win_b, p);

      for_all(p) if (input(p) == false)
	for_all(q) if (input.has(q))
	  if (output(q) != max
	      && output(q) + q.w() < output(p))
	    output(p) = output(q) + q.w();
    }

    return output;
  }

} // end of mln
