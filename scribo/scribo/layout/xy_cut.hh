// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
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

/// \file
///
/// \brief XY-Cut layout analysis algorithm.

#ifndef SCRIBO_LAYOUT_XY_CUT_HH
# define SCRIBO_LAYOUT_XY_CUT_HH

# include <mln/geom/min_row.hh>
# include <mln/geom/min_col.hh>
# include <mln/geom/max_row.hh>
# include <mln/geom/max_col.hh>
# include <mln/core/concept/image.hh>
# include <mln/core/macros.hh>
# include <mln/util/array.hh>
# include <scribo/layout/internal/hist_info.hh>
# include <scribo/layout/internal/node.hh>

namespace scribo
{

  namespace layout
  {
    using namespace mln;

    /// \brief XY-Cut layout analysis algorithm.
    /*!  This algorithm is an implementation made from \cite
      nagy.1992.computer7 : "A prototype document image analysis
      system for technical journals.".

      It recusively subdivides empty spaces in the document until a
      minimum division size is reached. The latter is defined with \p
      min_height and \p min_width.

      \input[in] ima A binary image.
      \input[in] min_height The minimum height of a subdivision.
      \input[in] min_width The minimum width of a subdivision.

      \return An array of component group bounding boxes.
     */
    template <typename I>
    mln::util::array<mln_box(I)>
    xy_cut(const Image<I>& ima, int min_height, int min_width);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {


      //-------------------------
      // Compute horizontal hist info
      //-------------------------

      template <typename I>
      void compute_hist_info_h(const Image<I>& input_,
			       mln_ch_value(I,internal::hist_info)& hist)
      {
	trace::entering("compute_hist_info_h");

	const I& input = exact(input_);

	mln_precondition(input.is_valid());
	mln_precondition(hist.is_valid());

	unsigned accumulator = 0;

	for (unsigned i = geom::min_row(input); i <= geom::max_row(input); ++i)
	{
	  accumulator = 0;
	  for (unsigned j = geom::min_col(input); j <= geom::max_col(input); ++j)
	  {
	    if (input.at_(i, j))
	      ++accumulator;

	    hist.at_(i, j).horizontal = accumulator;
	  }
	}

	trace::exiting("compute_hist_info_h");
      }


      //-------------------------
      // Compute vertical hist info
      //-------------------------

      template <typename I>
      void compute_hist_info_v(const Image<I>& input_,
			       mln_ch_value(I,internal::hist_info)& hist)
      {
	trace::entering("compute_hist_info_v");

	const I& input = exact(input_);

	mln_precondition(input.is_valid());
	mln_precondition(hist.is_valid());

	unsigned accumulator = 0;

	for (unsigned j = geom::min_col(input); j <= geom::max_col(input); ++j)
	{
	  accumulator = 0;
	  for (unsigned i = geom::min_row(input); i <= geom::max_row(input); ++i)
	  {
	    if (input.at_(i, j))
	      ++accumulator;

	    hist.at_(i, j).vertical = accumulator;
	  }
	}

	trace::exiting("compute_hist_info_v");
      }



      template <typename H>
      void horizontal_whitespace(const Image<H>& hist_,
				 const mln_site(H)& pmin,
				 const mln_site(H)& pmax,
				 int& max_height,
				 int& ws_min_row,
				 int& ws_max_row)
      {
	trace::entering("horizontal_whitespace");

	typedef mln_value(H) V;
	mlc_is(V,internal::hist_info)::check();

	const H& hist = exact(hist_);

	mln_precondition(hist.is_valid());

	const int x_min = pmin.col();
	const int x_max = pmax.col();
	const int y_min = pmin.row();
	const int y_max = pmax.row();

	for (int y = y_min; y <= y_max; ++y)
	{
	  const int n_white_pixels = hist.at_(y, x_max).horizontal -
	    hist.at_(y, x_min).horizontal;

	  if (!n_white_pixels)
	  {
	    int r = y + 1; // r : row

	    // While there is a gap in the histogram
	    for (; r <= y_max && !(hist.at_(r, x_max).horizontal -
				   hist.at_(r, x_min).horizontal); ++r);

	    const int count = r - y;

	    if (count > max_height)
	    {
	      max_height = count;
	      ws_min_row = (y == y_min) ? y_min : y - 1;
	      ws_max_row = r;
	    }

	    y = r;
	  }
	}

	trace::exiting("horizontal_whitespace");
      }


      template <typename H>
      void vertical_whitespace(const Image<H>& hist_,
			       const mln_site(H)& pmin,
			       const mln_site(H)& pmax,
			       int& max_width,
			       int& ws_min_col,
			       int& ws_max_col)
      {
	trace::entering("vertical_whitespace");

	typedef mln_value(H) V;
	mlc_is(V,internal::hist_info)::check();

	const H& hist = exact(hist_);

	mln_precondition(hist.is_valid());

	const int x_min = pmin.col();
	const int x_max = pmax.col();
	const int y_min = pmin.row();
	const int y_max = pmax.row();

	for (int x = x_min; x <= x_max; ++x)
	{
	  const int n_white_pixels = hist.at_(y_max, x).vertical -
	    hist.at_(y_min, x).vertical;

	  if (!n_white_pixels)
	  {
	    int c = x + 1; // c : column

	    // While there is a gap in the histogram
	    for (; c <= x_max && !(hist.at_(y_max, c).vertical -
				   hist.at_(y_min, c).vertical); ++c);

	    const int count = c - x;

	    if (count > max_width)
	    {
	      max_width = count;
	      ws_min_col = (x == x_min) ? x_min : x - 1;
	      ws_max_col = c;
	    }

	    x = c;
	  }
	}

	trace::exiting("vertical_whitespace");
      }


      template <typename H>
      void do_xy_cut(const Image<H>& hist_,
		     node<mln_box(H)>* root,
		     const mln_box(H)& domain,
		     const bool horizontal,
		     const bool should_stop,
		     const int min_height,
		     const int min_width)
      {
	trace::entering("do_xy_cut");

	typedef mln_value(H) V;
	mlc_is(V,internal::hist_info)::check();

	const H& hist = exact(hist_);

	mln_precondition(hist.is_valid());
	mln_precondition(root != 0);
	mln_precondition(domain.is_valid());

	const mln_site(H)& pmin = domain.pmin();
	const mln_site(H)& pmax = domain.pmax();
	const int x_min = pmin.col();
	const int x_max = pmax.col();
	const int y_min = pmin.row();
	const int y_max = pmax.row();

	if (horizontal)
	{
	  int max_height = 0;
	  int ws_min_row = 0; // Whitespace min and max
	  int ws_max_row = 0; // rows

	  horizontal_whitespace(hist, pmin, pmax, max_height, ws_min_row, ws_max_row);

	  if (max_height < min_height || max_height == (pmax.row() - pmin.row()))
	  {
	    if (should_stop)
	      return;

	    do_xy_cut(hist, root, domain, false, true, min_height, min_width);
	  }
	  else
	  {
	    typedef mln_box(H) B;
	    node<B>* ls = (ws_min_row == y_min) ? 0 :
	      new node<B>(box2d(pmin, point2d(ws_min_row, pmax.col())));
	    node<B>* rs = (ws_max_row >= y_max) ? 0 :
	      new node<B>(box2d(point2d(ws_max_row, pmin.col()), pmax));

	    root->set_ls(ls);
	    root->set_rs(rs);

	    if (ls)
	      do_xy_cut(hist, ls, ls->get_bbox(), false, false, min_height, min_width);
	    if (rs)
	      do_xy_cut(hist, rs, rs->get_bbox(), false, false, min_height, min_width);
	  }
	}
	else
	{
	  int max_width = 0;
	  int ws_min_col = 0; // Whitespace min and max
	  int ws_max_col = 0; // cols

	  vertical_whitespace(hist, pmin, pmax, max_width, ws_min_col, ws_max_col);

	  if (max_width < min_width || max_width == (pmax.col() - pmin.col()))
	  {
	    if (should_stop)
	      return;

	    do_xy_cut(hist, root, domain, true, true, min_height, min_width);
	  }
	  else
	  {
	    typedef mln_box(H) B;
	    node<B>* ls = (ws_min_col == x_min) ? 0 :
	      new node<B>(box2d(pmin, point2d(pmax.row(), ws_min_col)));
	    node<B>* rs = (ws_max_col >= x_max) ? 0 :
	      new node<B>(box2d(point2d(pmin.row(), ws_max_col), pmax));

	    root->set_ls(ls);
	    root->set_rs(rs);

	    if (ls)
	      do_xy_cut(hist, ls, ls->get_bbox(), true, false, min_height, min_width);
	    if (rs)
	      do_xy_cut(hist, rs, rs->get_bbox(), true, false, min_height, min_width);
	  }
	}

	trace::exiting("do_xy_cut");
      }


      template <typename B>
      void xy_dfs(const node<B>* root, util::array<B>& result)
      {
	if (root->is_leaf())
	{
	  result.append(root->get_bbox());
	  return;
	}

	const node<B>* ls = root->get_ls();
	const node<B>* rs = root->get_rs();

	if (ls)
	  xy_dfs(root->get_ls(), result);
	if (rs)
	  xy_dfs(root->get_rs(), result);
      }

    } // end of namespace scribo::layout::internal



    template <typename I>
    mln::util::array<mln_box(I)>
    xy_cut(const Image<I>& ima_, int min_height, int min_width)
    {
      trace::entering("scribo::layout::xy_cut");

      typedef mln_value(I) V;
      typedef mln_box(I) B;
      mlc_is(V,bool)::check();

      const I& ima = exact(ima_);
      mln_precondition(ima.is_valid());

      // Compute histogram
      mln_ch_value(I,internal::hist_info) hist(ima.domain());
      internal::compute_hist_info_h(ima, hist);
      internal::compute_hist_info_v(ima, hist);

      // XY-Cut
      internal::node<B>* n = new internal::node<B>(ima.domain());
      internal::do_xy_cut(hist, n, ima.domain(), true, false, min_height, min_width);

      // Build output.
      mln::util::array<B> output;
      internal::xy_dfs(n, output);

      // Clear temporary data.
      delete n;

      trace::exiting("scribo::layout::xy_cut");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_XY_CUT_HH
