// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_LEVEL_MEDIAN_HH
# define MLN_LEVEL_MEDIAN_HH

/*! \file mln/level/median.hh
 *
 * \brief Median filtering of an image.
 */

# include <mln/core/concept/image.hh>
# include <mln/geom/size2d.hh>

# include <mln/core/window2d.hh>
# include <mln/core/win/hline2d.hh>
# include <mln/core/t_image.hh>

# include <mln/accu/median.hh>
# include <mln/canvas/sbrowsing.hh>

# include <mln/geom/shift.hh>
# include <mln/set/diff.hh>


namespace mln
{

  namespace level
  {

    /*! Compute in \p output the median filter of image \p input by
     *  the window \p win.
     *
     * \param[in] input The image to be filtered.
     * \param[in] win The window.
     * \param[in,out] output The output image.
     *
     * \pre \p input and \p output have to be initialized.
     */
    template <typename I, typename W, typename O>
    void median(const Image<I>& input, const Window<W>& win,
		Image<O>& output);


# ifndef MLN_INCLUDE_ONLY


    namespace impl
    {


      template <typename I, typename O>
      void median(const Image<I>& input_, const win::hline2d& win, O& output)
      {

	I& input = exact(input_);
	typedef mln_coord(I) coord;
	const coord
	  min_row = geom::min_row(input),
	  max_row = geom::max_row(input),
	  min_col = geom::min_col(input),
	  max_col = geom::max_col(input);
	const coord half = win.length() / 2;

	point2d p;
	coord& row = p.row();
	coord& col = p.col();

	point2d pt;
	coord& ct = pt.col();

	point2d pu;
	coord& cu = pu.col();

	accu::median<mln_vset(I)> med(input.values());

	for (row = min_row; row <= max_row; ++row)
	  {
	    pt.row() = pu.row() = row;

	    // initialization (before first point of the row)
	    med.init();
	    for (ct = min_col; ct < min_col + half; ++ct)
	      med.take(input(pt));

	    // left columns (just take new points)
	    for (col = min_col; col <= min_col + half; ++col, ++ct)
	      {
		med.take(input(pt));
		output(p) = med.to_value();
	      }

	    // middle columns (both take and untake)
	    cu = min_col;
	    for (; col <= max_col - half; ++cu, ++col, ++ct)
	      {
		med.take(input(pt));
		med.untake(input(pu));
		output(p) = med.to_value();
	      }

	    // right columns (now just untake old points)
	    for (; col <= max_col; ++cu, ++col)
	      {
		med.untake(input(pu));
		output(p) = med.to_value();
	      }
	  }
      }

      template <typename I, typename O>
      void median(const Fast_Image<I>& input_, const win::hline2d& win, O& output)
      {

	I& input = exact(input_);
	border::resize(input, win.delta());;
	border::duplicate(input);

	typedef mln_coord(I) coord;
	const coord
	  min_row = geom::min_row(input),
	  max_row = geom::max_row(input),
	  min_col = geom::min_col(input),
	  max_col = geom::max_col(input);
	const coord half = win.length() / 2;

	point2d p;
	coord& row = p.row();
	coord& col = p.col();

	point2d pt;
	coord& ct = pt.col();

	point2d pu;
	coord& cu = pu.col();

	accu::median<mln_vset(I)> med(input.values());

	for (row = min_row; row <= max_row; ++row)
	  {
	    pt.row() = pu.row() = row;

	    // initialization (before first point of the row)
	    med.init();
	    for (ct = min_col - half; ct < min_col + half; ++ct)
	      med.take(input(pt));

	    // middle columns (both take and untake)
	    cu = min_col;
	    for (col = min_col; col <= max_col; ++cu, ++col, ++ct)
	      {
		med.take(input(pt));
		med.untake(input(pu));
		output(p) = med.to_value();
	      }

	  }
      }

      template <typename I, typename O>
      void median(const I& input, const win::vline2d& win, O& output)
      {
	t_image<O> swap_output = swap_coords(output, 0, 1);
	impl::median(swap_coords(input, 0, 1),
		     win::hline2d(win.length()),
		     swap_output);
      }


    } // end of namespace mln::level::impl


    // facade

    template <typename I, typename W, typename O>
    void median(const Image<I>& input, const Window<W>& win,
		Image<O>& output)
    {
      mln_assertion(exact(output).domain() == exact(input).domain());
      impl::median(exact(input), exact(win), exact(output)); 
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_MEDIAN_HH
