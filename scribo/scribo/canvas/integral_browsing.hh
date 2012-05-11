// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.


#ifndef SCRIBO_CANVAS_INTEGRAL_BROWSING_HH
# define SCRIBO_CANVAS_INTEGRAL_BROWSING_HH

# include <mln/core/image/image2d.hh>
# include <mln/util/couple.hh>

namespace scribo
{

  namespace canvas
  {
    using namespace mln;


    template <typename F>
    void integral_browsing(const image2d<mln::util::couple<double, double> >& ima,
			   unsigned step,
			   unsigned w, unsigned h,
			   F& functor);


# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      inline
      void compute_stats(// in
	double sum, double sum_2, unsigned n,
	// out
	double& mean, double& stddev)
      {
	mean = sum / n;

	// unbias version:
	double num = (sum_2 - sum * sum / n);
	if (num > 0)
	  stddev = std::sqrt(num / (n - 1));
	else
	  stddev = 0;

      }

    } // end of namespace scribo::canvas::internal




    template <typename F>
    void integral_browsing(const image2d<mln::util::couple<double, double> >& ima,
			   unsigned step,
			   unsigned w, unsigned h,
			   unsigned s,
			   F& functor)
    {
      typedef mln::util::couple<double, double> V;
      typedef const V* Ptr;
      Ptr a_ima, b_ima, c_ima, d_ima;

//       mln_precondition((h/2) < ima.nrows());
//       mln_precondition((w/2) < ima.ncols());

      const int
	nrows = ima.nrows(),
	ncols = ima.ncols(),
	row_0 = step / 2,
	col_0 = step / 2;

      const int
	offset_down  = ima.delta_offset(dpoint2d(step, 0)),
	offset_ante  = ima.delta_offset(dpoint2d(0, -w)),
	offset_below = ima.delta_offset(dpoint2d(+h, 0));

      const int
	max_row_top  = h/2,
	max_row_mid  = nrows - 1 - h/2,
	max_col_left = w/2,
	max_col_mid  = ncols - 1 - w/2,

	step_2   = step * step,

	h_top  = row_0 + h/2 + 1,
	w_left = col_0 + w/2 + 1;


      int row, col;

      for (col = col_0; col <= max_col_mid; col += step) ;
      int w_right = ncols - col + w/2;

      Ptr
	d_tl_start, d_tr_start,
	b_ml_start = 0, d_ml_start = 0, b_mr_start = 0, d_mr_start = 0,
	b_bl_start = 0, d_bl_start = 0, b_br_start = 0, d_br = 0;

      double mean, stddev;

      unsigned s_2 = s * s;

      // -------------------------------
      //           T (top)
      // -------------------------------

      const unsigned
	delta_start_left  = step * w_left,
	delta_start_right = step * w_right,
	step_w = step * w;
      int
	size_tl_start = h_top * w_left,
	size_tl,
	delta_size_tl = h_top * step,
	size_tc = h_top * w,
	delta_size_tr = h_top * step,
	size_tr_start = h_top * w_right,
	size_tr;

      d_tl_start = & ima.at_(row_0 + h/2, col_0 + w/2);
      d_tr_start = & ima.at_(row_0 + h/2, ncols - 1);

      for (row = row_0; row <= max_row_top; row += step)
      {

	// TL (top left)

	d_ima = d_tl_start;
	size_tl = size_tl_start;

	for (col = col_0; col <= max_col_left; col += step)
	{
	  // D
	  internal::compute_stats(d_ima->first(),
				  d_ima->second(),
				  size_tl * s_2,
				  mean, stddev);
	  functor.exec(mean, stddev);
	  d_ima += step;
	  size_tl += delta_size_tl;
	}

	delta_size_tl += step_2;
	size_tl_start += delta_start_left;
	d_tl_start += offset_down;

	// TC (top center)

	c_ima = d_ima + offset_ante;

	for (; col <= max_col_mid; col += step)
	{
	  // D - C
	  internal::compute_stats(d_ima->first()   - c_ima->first(),
				  d_ima->second() - c_ima->second(),
				  size_tc * s_2,
				  mean, stddev);
	  functor.exec(mean, stddev);
	  c_ima += step;
	  d_ima += step;
	}

	size_tc += step_w;

	// TR (top right)

	d_ima = d_tr_start;
	double
	  d_sum   = d_ima->first(),
	  d_sum_2 = d_ima->second();
	size_tr = size_tr_start;

	for (; col < ncols; col += step)
	{
	  // D* - C
	  internal::compute_stats(d_sum   - c_ima->first(),
				  d_sum_2 - c_ima->second(),
				  size_tr * s_2,
				  mean, stddev);
	  functor.exec(mean, stddev);
	  c_ima += step;
	  size_tr -= delta_size_tr;
	}

	delta_size_tr += step_2;
	size_tr_start += delta_start_right;
	d_tr_start += offset_down;

      }


      // -------------------------------
      //          (M) middle
      // -------------------------------


      const unsigned
	size_ml_start = h * w_left,
	h_step = h * step,
	size_mc = w * h;
      unsigned
	size_ml,
	size_mr_start = h * w_right,
	size_mr;

      if (row <= max_row_mid)
      {
	b_ml_start = & ima.at_(row - h/2 - 1, col_0 + w/2);
	d_ml_start = b_ml_start + offset_below;
	b_mr_start = & ima.at_(row - h/2 - 1, ncols - 1);
	d_mr_start = b_mr_start + offset_below;
      }

      for (; row <= max_row_mid; row += step)
      {

	// ML (middle left)

	size_ml = size_ml_start;
	b_ima = b_ml_start;
	d_ima = d_ml_start;

	for (col = col_0; col <= max_col_left; col += step)
	{
	  // D - B
	  internal::compute_stats(d_ima->first()   - b_ima->first(),
				  d_ima->second() - b_ima->second(),
				  size_ml * s_2,
				  mean, stddev);
	  functor.exec(mean, stddev);
	  b_ima += step;
	  d_ima += step;
	  size_ml += h_step;
	}

	b_ml_start += offset_down;
	d_ml_start += offset_down;

	// MC (middle center)

	a_ima = b_ima + offset_ante;
	c_ima = d_ima + offset_ante;

	for (; col <= max_col_mid; col += step)
	{
	  // D + A - B - C

	  internal::compute_stats((d_ima->first()   - b_ima->first())   + (a_ima->first()   - c_ima->first()),
				  (d_ima->second() - b_ima->second()) + (a_ima->second() - c_ima->second()),
				  size_mc * s_2,
				  mean, stddev);

	  functor.exec(mean, stddev);

	  a_ima += step;
	  b_ima += step;
	  c_ima += step;
	  d_ima += step;
	}

	// MR (middle right)

	size_mr = size_mr_start;
	b_ima = b_mr_start;
	d_ima = d_mr_start;
	double
	  d_b_sum   = d_ima->first()   - b_ima->first(),
	  d_b_sum_2 = d_ima->second() - b_ima->second();

	for (; col < ncols; col += step)
	{
	  // D* + A - B* - C
	  internal::compute_stats(d_b_sum   + (a_ima->first()   - c_ima->first()),
				  d_b_sum_2 + (a_ima->second() - c_ima->second()),
				  size_mr * s_2,
				  mean, stddev);
	  functor.exec(mean, stddev);
	  a_ima += step;
	  c_ima += step;
	  size_mr -= h_step;
	}

	b_mr_start += offset_down;
	d_mr_start += offset_down;
      }


      // -------------------------------
      //         B (bottom)
      // -------------------------------

      unsigned
	size_bl_start = (nrows - row + h/2) * w_left,
	size_bl,
	delta_size_bl = (nrows - row + h/2) * step,
	size_bc = (nrows - row + h/2) * w,

	size_br_start = (nrows - row + h/2) * w_right,
	delta_size_br = (nrows - row + h/2) * step,
	size_br;

      if (row < nrows)
      {
	b_bl_start = & ima.at_(row - h/2 - 1, col_0 + w/2);
	d_bl_start = & ima.at_(nrows - 1, col_0 + w/2);
	b_br_start = & ima.at_(row - h/2 - 1, ncols - 1);
	d_br = & ima.at_(nrows - 1, ncols - 1);
      }

      for (; row < nrows; row += step)
      {

	// BL (bottom left)

	size_bl = size_bl_start;
	b_ima = b_bl_start;
	d_ima = d_bl_start;

	for (col = col_0; col <= max_col_left; col += step)
	{
	  // D* - B
	  internal::compute_stats(d_ima->first()   - b_ima->first(),
				  d_ima->second() - b_ima->second(),
				  size_bl * s_2,
				  mean, stddev);
	  functor.exec(mean, stddev);
	  b_ima += step;
	  d_ima += step;
	  size_bl += delta_size_bl;
	}

	delta_size_bl -= step_2;
	size_bl_start -= delta_start_left;
	b_bl_start += offset_down;

	// BC (bottom center)

	a_ima = b_ima + offset_ante;
	c_ima = d_ima + offset_ante;

	for (; col <= max_col_mid; col += step)
	{
	  // D* + A - B - C*
	  internal::compute_stats((d_ima->first()   - b_ima->first())   + (a_ima->first()   - c_ima->first()),
				  (d_ima->second() - b_ima->second()) + (a_ima->second() - c_ima->second()),
				  size_bc * s_2,
				  mean, stddev);
	  functor.exec(mean, stddev);
	  a_ima += step;
	  b_ima += step;
	  c_ima += step;
	  d_ima += step;
	}

	size_bc -= step_w;

	// BR (bottom right)

	size_br = size_br_start;
	b_ima = b_br_start;
	d_ima = d_br;
	double
	  d_b_sum   = d_ima->first()   - b_ima->first(),
	  d_b_sum_2 = d_ima->second() - b_ima->second();

	for (; col < ncols; col += step)
	{
	  // D* + A - B* - C*
	  internal::compute_stats(d_b_sum   + (a_ima->first()  - c_ima->first()),
				  d_b_sum_2 + (a_ima->second() - c_ima->second()),
				  size_br * s_2,
				  mean, stddev);
	  functor.exec(mean, stddev);
	  a_ima += step;
	  c_ima += step;
	  size_br -= delta_size_br;
	}

	delta_size_br -= step_2;
	size_br_start -= delta_start_right;
	b_br_start += offset_down;
      }

      functor.finalize();
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::canvas

} // end of namespace mln


#endif // ! SCRIBO_CANVAS_INTEGRAL_BROWSING_HH
