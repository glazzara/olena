// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_TEXT_MERGING_HDOC_HH
# define SCRIBO_TEXT_MERGING_HDOC_HH

/// \file
///
/// \brief Merge text component in order to reconstruct text lines.


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/util/array.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/data/fill.hh>
#include <mln/data/wrap.hh>

#include <mln/make/box2d.hh>

#include <mln/value/rgb8.hh>
#include <mln/io/ppm/save.hh>

#include <mln/draw/box.hh>
#include <mln/data/stretch.hh>
#include <mln/data/wrap.hh>
#include <mln/util/timer.hh>

#include <scribo/text/look_like_text_lines.hh>


namespace scribo
{

  namespace text
  {

    using namespace mln;


    /// \brief Merge text component in order to reconstruct text lines.
    ///
    /// \param[in] lines A line set.
    ///
    /// \return A new line set.  Line ids are preserved and merged
    /// lines (not valid anymore) are tagged with line::Merged.  The
    /// lines produced with this algorithm (valid lines) are tagged
    /// with line::None. Line type is also set either with line::Text
    /// or line::Punctuation.
    //
    template <typename L>
    line_set<L>
    merging_hdoc(const scribo::line_set<L>& lines);


# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      using namespace mln;
      using value::int_u8;


      template <typename T, typename T2>
      void draw_box(image2d<T>& input, const box2d& b, T2 l)
      {
	const unsigned
	  delta = input.delta_offset(dpoint2d(1,0)),
	  nrows = b.nrows(),
	  ncols = b.ncols();
	T* p_start = & input(b.pmin());
	T* ptr = p_start;
	for (unsigned r = 0; r < nrows; ++r)
	{
	  ptr = p_start;
	  for (unsigned c = 0; c < ncols; ++c)
	    *ptr++ = l;
	  p_start += delta;
	}
      }




      template <typename T, typename T2>
      void draw_box(image2d<T>& input,
		    int pmin_row, int pmin_col,
		    int pmax_row, int pmax_col,
		    T2 l)
      {
	if (pmax_row < pmin_row || pmax_col < pmin_col)
	  return;

	const unsigned
	  input_nrows_1 = input.nrows() - 1,
	  input_ncols_1 = input.ncols() - 1;
	if (pmin_row < 0) pmin_row = 0;
	if (pmin_col < 0) pmin_col = 0;
	if (pmax_row > input_nrows_1) pmax_row = input_nrows_1;
	if (pmax_col > input_ncols_1) pmax_col = input_ncols_1;

	const unsigned
	  delta = input.delta_offset(dpoint2d(1,0)),
	  nrows = pmax_row - pmin_row + 1,
	  ncols = pmax_col - pmin_col + 1;
	T* p_start = & input.at_(pmin_row, pmin_col);
	T* ptr = p_start;
	for (unsigned r = 0; r < nrows; ++r)
	{
	  ptr = p_start;
	  for (unsigned c = 0; c < ncols; ++c)
	    *ptr++ = l;
	  p_start += delta;
	}
      }




      inline
      unsigned my_find_root(mln::util::array<unsigned>& parent, unsigned x)
      {
	if (parent[x] == x)
	  return x;
	return parent[x] = my_find_root(parent, parent[x]);
      }


      inline
      void swap_ordering(unsigned l1, unsigned l2)
      {
	if (l2 > l1)
	  return;
	unsigned l1_ = l1;
	l1 = l2;
	l2 = l1_;
      }



      template <typename L>
      unsigned do_union(scribo::line_set<L>& lines,
			unsigned l1,
			unsigned l2,
			mln::util::array<unsigned>& parent)
      {
	l1 = my_find_root(parent, l1);
	l2 = my_find_root(parent, l2);
	if (l1 == l2)
	  return l1;

	swap_ordering(l1, l2);
	parent[l2] = l1; // The smallest label value is root.

	line_info<L>& l1_info = lines(l1);
	line_info<L>& l2_info = lines(l2);

	if (l2_info.card() > l1_info.card())
	{
          // we transfer data from the largest item to the root one.
	  scribo::line_info<L> tmp = l1_info;
	  std::swap(l1_info, l2_info);
	  l1_info.fast_merge(tmp);

	  // We must set manually the tag for lines(l2) since it is
	  // not used directly in merge process so its tag cannot be
	  // updated automatically.
	  l2_info.update_tag(line::Merged);
	  l2_info.set_hidden(true);
	}
	else
	  l1_info.fast_merge(l2_info);

	// l1's tag is automatically set to line::Needs_Precise_Stats_Update
	// l2's tag is automatically set to line::Merged

	return l1;
      }




      inline
      box2d enlarge(const box2d& b, int delta)
      {
	box2d b_(point2d(b.pmin().row(), b.pmin().col() - delta),
		 point2d(b.pmax().row(), b.pmax().col() + delta));
	return b_;
      }


      template <typename L>
      bool between_separators(const scribo::line_set<L>& lines,
			      const line_id_t& l1_,
			      const line_id_t& l2_)
      {
	const scribo::line_info<L>& l1 = lines(l1_);
	const scribo::line_info<L>& l2 = lines(l2_);

        // No separators found in image.
	mln_precondition(lines.components().has_separators());

	const box2d& l1_bbox = l1.bbox();
	const box2d& l2_bbox = l2.bbox();

	const unsigned
	  col1 = l1_bbox.pcenter().col(),
	  col2 = l2_bbox.pcenter().col();
	const mln_ch_value(L, bool)&
	  separators = lines.components().separators();

	// Checking for separators starting from 1 / 4, 3/ 4 and the
	// center of the box
	typedef const bool* sep_ptr_t;
	sep_ptr_t sep_ptr, sep_ptr_top, sep_ptr_bottom, end;

	if (col1 < col2)
	{
	  const unsigned quarter =
	    ((l1_bbox.pcenter().row() - l1_bbox.pmin().row()) >> 1);

	  sep_ptr = &separators(l1_bbox.pcenter());
	  sep_ptr_top = &separators(point2d(l1_bbox.pmin().row() + quarter,
					    l1_bbox.pcenter().col()));
	  sep_ptr_bottom = &separators(point2d(l1_bbox.pmax().row() - quarter,
					    l1_bbox.pcenter().col()));
	  end = sep_ptr + col2 - col1;
	}
	else
	{
	  const unsigned quarter =
	    ((l2_bbox.pcenter().row() - l2_bbox.pmin().row()) >> 1);

	  sep_ptr = &separators(l2_bbox.pcenter());
	  sep_ptr_top = &separators(point2d(l2_bbox.pmin().row() + quarter,
					    l2_bbox.pcenter().col()));
	  sep_ptr_bottom = &separators(point2d(l2_bbox.pmax().row() - quarter,
					    l2_bbox.pcenter().col()));
	  end = sep_ptr + col1 - col2;
	}

	// If sep_ptr is true, then a separator is reached.
	while (!*sep_ptr && !*sep_ptr_top && !*sep_ptr_bottom && sep_ptr != end)
	{
	  ++sep_ptr;
	  ++sep_ptr_top;
	  ++sep_ptr_bottom;
	}

	return (*sep_ptr || *sep_ptr_top || *sep_ptr_bottom);
      }


      /*! \brief Check whether two lines can merge.

	Criterions:
	- Height ratio must be <= 1.7
	- Baselines delta must be <= 3
	- Boxes must not overlap too much.

      */
      template <typename L>
      bool lines_can_merge(scribo::line_set<L>& lines,
			   const scribo::line_id_t& l1_,
			   const scribo::line_id_t& l2_)
      {
	scribo::line_info<L>& l1 = lines(l1_);
	scribo::line_info<L>& l2 = lines(l2_);

	// Parameters.
	const float x_ratio_max = 1.7f;
	const float baseline_delta_max =
	  0.5f * std::min(l1.x_height(), l2.x_height());

	const box2d& l1_bbox = l1.bbox();
	const box2d& l2_bbox = l2.bbox();

	const point2d& l1_pmin = l1_bbox.pmin();
	const point2d& l2_pmin = l2_bbox.pmin();
	const point2d& l1_pmax = l1_bbox.pmax();
	const point2d& l2_pmax = l2_bbox.pmax();

	const bool l1_has_separators = lines.components().has_separators();
	const bool l1_l2_between_separators = (l1_has_separators) ?
	  between_separators(lines, l1_, l2_) : false;
	const float l_ted_cw = l2.char_width();

	const float dx = std::max(l1_pmin.col(), l2_pmin.col())
	  - std::min(l1_pmax.col(), l2_pmax.col());
	const float dy = std::max(l1_pmin.row(), l2_pmin.row())
	  - std::min(l1_pmax.row(), l2_pmax.row());

	// Particular case of "
	{
	  if (// Must have 2 characters
	    (l1.card() == 2
	     // The box height must be smaller than the touched line x height
	     && l1_bbox.height() < l2.x_height())
	    // The line must be vertically and horizontally close to
	    // the touched line
	      && (dx < l_ted_cw && dy < 0)
	    // No separator between the two lines
	      && not (l1_l2_between_separators))
	  {
	    // Line is then considered as punctuation
	    l1.update_type(line::Punctuation);
	    return true;
	  }
	}

	// Particular case like merging between a line and [5]
	{
	  const mln::def::coord
	    top_row_l2 = l2_pmin.row(),
	    top_row_l1 = l1_pmin.row(),
	    bot_row = l2_pmax.row();
	  const float x1 = l1.x_height(), x2 = l2.x_height();
	  const float x_ratio = std::max(x1, x2) / std::min(x1, x2);

	  if (// No separator
	    !l1_l2_between_separators
	    // The x height ration must be lower than 2
	      && (x_ratio < 2.0f)
	    // Baseline alignment
	      && (std::abs(bot_row - l1.baseline()) < baseline_delta_max)
	    // The top of the boxes must be aligned
	      && (std::abs(top_row_l2 - top_row_l1) < 5)
            // Distance between the line and the touched line.
	      && dx < 5.0f * l_ted_cw)
	  {
	    return true;
	  }
	}

	// Similarity of x_height.
	{
	  const float x1 = l1.x_height(), x2 = l2.x_height();
	  const float x_ratio = std::max(x1, x2) / std::min(x1, x2);
	  if (x_ratio > x_ratio_max)
	    return false;
	}

	// Same baseline.
	{
	  if (std::abs(l1.baseline() - l2.baseline()) > baseline_delta_max)
	    return false;
	}

	// left / right
	const unsigned
	  col1 = l1_bbox.pcenter().col(),
	  col2 = l2_bbox.pcenter().col();
	if (col1 < col2)
	{
	  if ((col1 + l1_bbox.width() / 4) >= (col2 - l2_bbox.width() / 4))
 	    return false;
	}
	else
	  if ((col2 + l2_bbox.width() / 4) >= (col1 - l1_bbox.width() / 4))
	    return false;

 	// Check that there is no separator in between.
	if (l1_has_separators)
	  return ! l1_l2_between_separators;

	return true;
      }




      inline
      int horizontal_distance(const box2d& l1,
			      const box2d& l2)
      {
	if (l1.pcenter().col() < l2.pcenter().col())
	  return l2.pmin().col() - l1.pmax().col();
	else
	  return l1.pmin().col() - l2.pmax().col();
      }




      /*! \brief Check whether a non text line and a text line can merge.

	Criterions:
	- Small height (c.height < l.x_height)
	- Character width mean in 'c' must be lower than the character
        width median of 'l'. (c.width / c.ncomps < l.char_width)

	OR

	- Small height (c.height < l.x_height)
	- Not so long width (c.width < 5 * l.char_width)
	- Aligned with the 'x' center ((l.baseline + l.meanline / 2) - c.center.row < 7)
	- tiny spacing (horizontal distance < 5)

      */
      template <typename L>
      bool non_text_and_text_can_merge(scribo::line_set<L>& lines,
				       const scribo::line_id_t& l_cur_, // current
				       const scribo::line_id_t l_ted_) // touched
      {
	scribo::line_info<L>& l_cur = lines(l_cur_);
	scribo::line_info<L>& l_ted = lines(l_ted_);

	if (l_cur.type() == line::Text || l_ted.type() != line::Text)
	  return false;
	// the current object is a NON-textline
	// the background (touched) object is a textline


 	// Check that there is no separator in between.
	if (lines.components().has_separators()
	    && between_separators(lines, l_cur_, l_ted_))
	  return false;

	const box2d& l_cur_bbox = l_cur.bbox();
	const box2d& l_ted_bbox = l_ted.bbox();

	const point2d& l_cur_pmin = l_cur_bbox.pmin();
	const point2d& l_ted_pmin = l_ted_bbox.pmin();
	const point2d& l_cur_pmax = l_cur_bbox.pmax();
	const point2d& l_ted_pmax = l_ted_bbox.pmax();

	const float dx = std::max(l_cur_pmin.col(), l_ted_pmin.col())
	  - std::min(l_cur_pmax.col(), l_ted_pmax.col());
	const float l_ted_cw = l_ted.char_width();
	const float l_ted_x_height = l_ted.x_height();

	const unsigned l_cur_height = l_cur_bbox.height();
	const unsigned l_cur_width = l_cur_bbox.width();

	// General case (for tiny components like --> ',:."; <--):
	if (l_cur_height < l_ted_x_height
	    && l_cur_height > 0.05f * l_ted_x_height
	    && float(l_cur_width) / float(l_cur.card()) < l_ted.char_width()
	    && dx < 2 * l_ted_cw
	    && l_cur_pmin.row() < l_ted.baseline())
	{
	  l_cur.update_type(line::Punctuation);
	  return true;
	}

	// Special case for '---':
	if (// small height:
	  l_cur_height < l_ted_x_height
	  // // not so long width:
	  && l_cur_width > 0.8 * l_ted_cw
	  && l_cur_width < 5 * l_ted_cw
	  // align with the 'x' center:
	  && std::abs((l_ted.baseline() + l_ted.meanline()) / 2 - l_cur.bbox().pcenter().row()) < 7
	  // tiny spacing:
	  && unsigned(horizontal_distance(l_cur_bbox, l_ted_bbox)) < 2 * l_ted_cw
	  )
	{
	  return true;
	}


	// Special case

	// Looking for alignement.
	const mln::def::coord
	  top_row = l_cur.bbox().pmin().row(),
	  bot_row = l_cur.bbox().pmax().row();

	const box2d& l_ted_ebbox = l_ted.ebbox();

// 	std::cout << "top_row = " << top_row << " - bot_row = " << bot_row << std::endl;
// 	std::cout << std::abs(bot_row - l_ted.baseline())
// 		  << " - d "
// 		  << std::abs(bot_row - l_ted.descent())
// 		  << " - dd "
// 		  << std::abs(bot_row - l_ted.ebbox().pmax().row())
// 		  << " - "
// 		  << std::abs(top_row - l_ted.meanline())
// 		  << " - a "
// 		  << std::abs(top_row - l_ted.ascent())
// 		  << " - aa "
// 		  << std::abs(top_row - l_ted.ebbox().pmin().row())
// 		  << " - "
// 		  << l_ted.ascent()
// 		  << " - "
// 		  << l_ted.descent()
// 		  << std::endl;

	if ((std::abs(bot_row - l_ted.baseline()) < 5
	     || std::abs(bot_row - l_ted_ebbox.pmax().row()) < 5)
	    &&
	    (std::abs(top_row - l_ted.meanline()) < 5
	     || std::abs(top_row - l_ted_ebbox.pmin().row()) < 5)
	    && dx < 5.0f * l_ted_cw)
	{
	  return true;
	}

	return false;
      }





      /*! \brief Merge text lines.

	This algorithm iterates over all the components ordered by size.
	It uses a 2d labeled image, tmp_box, to draw component bounding
	boxes and uses that image to check bounding box collisions.
	Depending on that collisions and whether the lines looks like
	a text line or not, bounding boxes are merged.

	\verbatim
	ALGORITHM:
	for each component 'cur' in decreasing order
	  if already merged
	    continue

	    ///
	    /// x-----------x
	    /// |           |
	    /// x     x     x
	    /// |           |
	    /// x-----------x
	    ///

	    Set labels <- Every labels corresponding to the colliding bounding
	                  boxes (uses only the 7 sites detailled above).

	    If label.card == 1
	      l = label.get(0);
	      If l != background
	        If looks_like_a_text_line(cur)
		  If looks_like_a_text_line(l)
		    // Error case: a line is included in a line.
		  else
		    // Line cur is included in a frame or a drawing.
		    draw_enlarged_box(l)
		  end
		else
		  If looks_like_a_text_line(l)
		  // Component cur is a punctuation overlapping with line l.
		  l_ <- do_union(cur, l)
		  draw_enlarged_box(l_)
		end
	      end
	    else
	      If looks_like_a_text_line(cur)
	        // Component cur is a new line.
		draw_enlarged_box(l)
	      end
	    end
	  else
	    for each label l in labels
	      If l == background
	        continue
	      end

	      If lines_can_merge(cur, l)
	        l_ <- do_union(cur, l)
		draw_enlarged_box(l_)
		continue
	      end

	      If !looks_like_a_text_line(cur) and looks_like_a_text_line(l)
	     	If non_text_and_text_can_merge(cur, l)
		  // A punctuation merge with a line
		  l_ <- do_union(cur, l)
		  draw_enlarged_box(l_)
		  continue
		else
		  // None
		end
	      else
	      // Error case
	    end
	  end

	  \endverbatim
      */
      // FIXME:
      //
      // Important note: after merging two lines, we draw the
      // merged line over the existing one; we have to ensure that we
      // cover the previous rectangle (otherwise we have a label in
      // 'billboard' that is not used anymore! and it can mix up the
      // detection of upcoming merges...)  so this delta has to remain
      // the same during one pass.  Another solution (yet more costly)
      // could be of erasing the previous rectangle before re-drawing...
      //
      template <typename L>
      void
      one_merge_pass(unsigned ith_pass,
		     const box2d& domain,
		     std::vector<scribo::line_id_t>& v, // Ids sorted by bbox size.
		     scribo::line_set<L>& lines, // Tagged Lines (looks_like_a_text_line?)
		     mln::util::array<unsigned>& parent)
      {
	image2d<unsigned> billboard(domain);
	data::fill(billboard, 0);

# ifndef SCRIBO_NDEBUG
	image2d<value::int_u8> log(domain);
	data::fill(log, 0);
# endif // ! SCRIBO_NDEBUG

	const unsigned n = v.size();
	unsigned l_;

	unsigned
	  count_txtline_IN_txtline = 0,
	  count_txtline_IN_junk = 0,
	  count_two_lines_merge = 0,
	  count_new_txtline = 0,
	  count_comp_IN_txtline = 0,
	  count_comp_HITS_txtline = 0,
	  count_WTF = 0;

	for (int i = n - 1; i >= 0; --i)
	{
	  unsigned l = v[i];

	  if (parent[l] != l) // not a root, so has already merged, thus ignore it
	    continue;

	  const box2d& b = lines(l).bbox();

	  // unsigned tl, tr, ml, mc, mr, bl, br;

	  const box2d& b_ = lines(l).ebbox();

	  /*
	    tl             tr
	    x---------------x
	    |               |
	    |       mc      |
         ml x       x       x mr
	    |               |
	    |               |
	    x---------------x
	    bl             br

	  */


	  const unsigned tl = billboard(b_.pmin());
	  const unsigned tr = billboard.at_(b_.pmin().row(), b_.pmax().col());
	  const unsigned ml = billboard.at_(b_.pcenter().row(), b_.pmin().col());
	  const unsigned mc = billboard.at_(b_.pcenter().row(), b_.pcenter().col());
	  const unsigned mr = billboard.at_(b_.pcenter().row(), b_.pmax().col());
	  const unsigned bl = billboard.at_(b_.pmax().row(), b_.pmin().col());
	  const unsigned br = billboard(b_.pmax());

	  typedef std::set<unsigned> set_t;
	  std::set<unsigned> labels;
	  labels.insert(tl);
	  labels.insert(tl);
	  labels.insert(tr);
	  labels.insert(ml);
	  labels.insert(mc);
	  labels.insert(mr);
	  labels.insert(bl);
	  labels.insert(br);


	  for (set_t::const_iterator it = labels.begin();
	       it != labels.end();
	       ++it)
	  {
	    if (*it == 0)
	      continue;

	    if (lines(*it).type() != line::Text)
	      std::cerr << "outch: we have hit, so drawn, a non-text..." << std::endl;
	  }


	  if (labels.size() == 1) // Same behavior for all anchors.
	  {
	    if (mc != 0)
	    {
	      // Main case: it is an "included" box (falling in an already drawn box)

	      const line_info<L>& l_info = lines(l);
	      const line_info<L>& mc_info = lines(mc);

	      if (l_info.type() == line::Text) // the current object IS a text line
	      {
		if (mc_info.type() == line::Text) // included in a text line => weird
		{
		  ++count_txtline_IN_txtline;

		  // Particular case of "
		  // {
		  //   if ((lines(l).card() == 2 &&
		  //        lines(l).bbox().height() < lines(mc).x_height()) &&
		  // 	not (lines(l).holder().components().has_separators()
		  // 	     && between_separators(lines(l),
		  // lines(mc))))

		  const box2d& l_bbox = l_info.bbox();
		  const box2d& mc_bbox = mc_info.bbox();

		  const point2d& l_pmin = l_bbox.pmin();
		  const point2d& mc_pmin = mc_bbox.pmin();
		  const point2d& l_pmax = l_bbox.pmax();
		  const point2d& mc_pmax = mc_bbox.pmax();

		  const float dx = std::max(l_pmin.col(), mc_pmin.col())
		    - std::min(l_pmax.col(), mc_pmax.col());
		  const float dy = std::max(l_pmin.row(), mc_pmin.row())
		    - std::min(l_pmax.row(), mc_pmax.row());
		  const float l_ted_cw = mc_info.char_width();

		  // We accept a line included into another only if it
		  // is horizontally close to the line's bbox and
		  // vertically aligned
		  // Obviously no separators between the two lines
		  if ((l_info.card() <= 5 ||
		       (std::abs(l_info.baseline() - mc_info.baseline())
			< 5 && std::abs(l_info.meanline() -
					mc_info.meanline()) < 5))
		    && dx < l_ted_cw && dy < 0
		    && 	not (lines.components().has_separators()
		  	     && between_separators(lines, l, mc)))
		    l = do_union(lines, l, mc,  parent);
		  // }

//		  std::cout << "weird: inclusion of a txt_line in a txt_line!" << std::endl;

		  /// Merge is perform if the current line is a
		  /// petouille considered as a line.
// 		  if ((std::abs(lines(l).ascent() - lines(mc).ascent()) >= 5)
// 		      || (std::abs(lines(l).descent() - lines(mc).descent()) >= 5))
// 		      continue;

// 		  // FIXME: Is it valid?
// 		  // A text line is included in another text line.
// 		  // They are merged.
// 		  //
// 		  l_ = do_union(lines, mc, l, parent);
// 		  draw_box(billboard, lines(l_).ebbox(), l_);

# ifndef SCRIBO_NDEBUG
 		  // Log:
 		  draw_box(log, b, 126);
# endif // ! SCRIBO_NDEBUG
		}

		else  // FIXME: Remove!  since included in a non-text-line, so not drawn, so inclusion impossible!!!!!!!!!!
		{
		  std::cout << "error: should NOT happen (a text line included in a NON-text-line (so not drawn!!!)" << std::endl;
		  ++count_txtline_IN_junk;

		  // a non-text-line (probably a drawing or a frame) includes a text line
		  draw_box(billboard, lines(l).ebbox(), l);

# ifndef SCRIBO_NDEBUG
		  // Log:
		  draw_box(log, b, 100);
# endif // ! SCRIBO_NDEBUG
		}

	      }
	      else // the current object is NOT a text line
	      {
		if (lines(mc).type() == line::Text) // included in a text line
		{
		  ++count_comp_IN_txtline;

		  // The current object is supposed to be punctuation
		  // since it is fully included in the bbox of a line
		  // of text, so we always want to merge this object
		  // with the line.
		  //
		  // However, in case of a bad quality document, we
		  // may not always want to merge since this object
		  // could be noise or garbage... So adding new
		  // criterions could fix this issue.
		  //
 		  if (!non_text_and_text_can_merge(lines, l, mc))
 		   continue;

		  // Avoid the case when a large title ebbox overlap
		  // with a text column. In that case, the title may
		  // merge with punctuation from the text.
		  // if (lines(l).holder().components().has_separators()
		  //     && between_separators(lines(l), lines(mc)))
		  //   continue;

		  // Mark current line as punctuation.
		  lines(l).update_type(line::Punctuation);

		  // Merge non-text-line #l into text line #mc.
		  l_ = do_union(lines, mc, l, parent);
		  // We have to re-draw the original largest text line since
		  // it may change of label (take the one of the included line).
		  draw_box(billboard, lines(l_).ebbox(), l_);

# ifndef SCRIBO_NDEBUG
		  // Log:
		  draw_box(log, b, 128);
# endif // ! SCRIBO_NDEBUG
		}
	      }
	    }
	    else // mc == 0
	    {
	      // Main case: it is a "new" box, that might be drawn in the background.

	      // we only draw this box if it is a text-line!!!
	      if (lines(l).type() == line::Text)
	      {
		++count_new_txtline;
		draw_box(billboard, lines(l).ebbox(), l);
# ifndef SCRIBO_NDEBUG
		// Log:
		draw_box(log, b, 127);
# endif // ! SCRIBO_NDEBUG
	      }
# ifndef SCRIBO_NDEBUG
	      else
	      	draw_box(log, b, 1);
# endif // ! SCRIBO_NDEBUG
	    }
	  }
	  else
	  {
	    l_ = l; // current label.

	    // Particular cases.
	    for (set_t::const_iterator it = labels.begin();
		 it != labels.end();
		 ++it)
	    {
	      unsigned lcand = *it;

	      if (lcand == 0) // Skip background.
		continue;

	      // if (lines(lcand).type() != line::Text)
	      // 	std::cerr << "again!" << std::endl;


	      if (lines(l_).type() == line::Text)
	      {
		// l_ and lcand look like text line chunks.
		if (lines_can_merge(lines, l_, lcand))
		{
		  ++count_two_lines_merge;
		  l_ = do_union(lines, l_, lcand,  parent);

		  draw_box(billboard, lines(l_).ebbox(), l_);

# ifndef SCRIBO_NDEBUG
		  // Log:
		  draw_box(log, b, 151);
# endif // ! SCRIBO_NDEBUG

		  continue;
		}
		else
		{
		  ++count_WTF;

# ifndef SCRIBO_NDEBUG
		  // Log:
		  draw_box(log, b, 255);
# endif // ! SCRIBO_NDEBUG

		  // (*) SEE BELOW
		  draw_box(billboard, lines(l_).ebbox(), l_);
		}
	      }
	      else
	      {
		// l_ does NOT looks like a text line chunk.
		++count_comp_HITS_txtline;
		if (non_text_and_text_can_merge(lines, l_, lcand))
		  // a petouille merges with a text line?
		{
		  ++count_comp_HITS_txtline;
		  l_ = do_union(lines, l_, lcand,  parent);
		  draw_box(billboard, lines(l_).ebbox(), l_);

# ifndef SCRIBO_NDEBUG
		  // Log:
		  draw_box(log, b, 169);
# endif // ! SCRIBO_NDEBUG

		  continue;
		}
# ifndef SCRIBO_NDEBUG
		else
		{
		  // Log:
		  draw_box(log, b, 254);
		}
# endif // ! SCRIBO_NDEBUG
	      }


		/*  (*)  Text lines verticaly overlap.

		   --------------------------
		   |           l            |
		   |                        |
		   --------------------------
		   |           lcand        |
		   |                        |
		   --------------------------

		   or

		   --------------------------
		   |          l             |
		   |                        |
		   |---------------------------
		   |------------------------- |
		   |         lcand            |
		   ----------------------------

		   or

		   --------------------------
		   |          lcand         |
		   |                        |
 		   |---------------------------
		   |------------------------- |
		   |           l              |
		   ----------------------------

		 */

	    }
	  }

	}


// 	std::cout
// 	  << "   new txtline        = " << count_new_txtline        << std::endl
// 	  << "   comp IN txtline    = " << count_comp_IN_txtline    << std::endl
// 	  << "   2 lines merge      = " << count_two_lines_merge    << std::endl
// 	  << "   comp HITS txtline  = " << count_comp_HITS_txtline  << std::endl
// 	  << "   txtline IN junk    = " << count_txtline_IN_junk    << std::endl
// 	  << "   txtline IN txtline = " << count_txtline_IN_txtline << std::endl
// 	  << "   WTF!               = " << count_WTF << std::endl;


	(void) ith_pass;
// 	if (ith_pass == 1)
// 	{
// 	  mln::io::pgm::save(log, "log_1.pgm");
// 	  mln::io::pgm::save(data::wrap(int_u8(), billboard), "log_1e.pgm");
// 	}
// 	else if (ith_pass == 2)
// 	{
// 	  mln::io::pgm::save(log, "log_2.pgm");
// 	  mln::io::pgm::save(data::wrap(int_u8(), billboard), "log_2e.pgm");
// 	}
// 	else if (ith_pass == 3)
// 	{
// 	  mln::io::pgm::save(log, "log_3.pgm");
// 	  mln::io::pgm::save(data::wrap(int_u8(), billboard), "log_3e.pgm");
// 	}
      }




      template <typename L>
      struct order_lines_id
      {
	order_lines_id(const scribo::line_set<L>& lines)
	  : lines_(lines)
	{
	}

	bool operator()(const scribo::line_id_t& l1, const scribo::line_id_t& l2) const
	{
	  const unsigned l1_nsites = lines_(l1).bbox().nsites();
	  const unsigned l2_nsites = lines_(l2).bbox().nsites();

	  if (l1_nsites == l2_nsites)
	    return l1 < l2;
	  return l1_nsites < l2_nsites;
	}

	scribo::line_set<L> lines_;
      };


      template <typename L>
      scribo::line_set<L>
      draw_boxes(const box2d& input_domain,
		 const scribo::line_set<L>& lines_)
      {
	scribo::line_set<L> lines = lines_.duplicate();
	const unsigned n = lines.nelements();

	order_lines_id<L> func(lines);
	std::vector<scribo::line_id_t> v;
	v.reserve(n);

	// Union-find parent data, used to merge lines.
	mln::util::array<unsigned> parent(n + 1);

	// Initialize data
	parent(0) = 0;
	for (unsigned l = 1; l < parent.nelements(); ++l)
	{
	  v.push_back(l);
	  parent[l] = l;
	}

	// Sort lines by bbox.nelements() and ids.
	std::sort(v.begin(), v.end(), func);

// 	mln::util::timer t;


	// Setting lines as text lines according to specific criterions.
	for_all_lines(l, lines)
	  if (looks_like_a_text_line(lines(l)))
	    lines(l).update_type(line::Text);


	// First pass
// 	t.start();
	one_merge_pass(1, input_domain, v, lines, parent);
// 	float ts = t.stop();
// 	std::cout << "time " << ts << std::endl;


//	lines.force_stats_update();

	// Sort lines by bbox.nelements() and ids again!
	// line may have grown differently since the first pass.
	std::sort(v.begin(), v.end(), func);


	// Second pass
// 	t.start();
	one_merge_pass(2, input_domain, v, lines, parent); // <- last pass
// 	ts = t.stop();
// 	std::cout << "time " << ts << std::endl;

	lines.force_stats_update();

	return lines;
      }

    } // end of namespace scribo::text::internal



    // Facade

    template <typename L>
    line_set<L>
    merging_hdoc(const scribo::line_set<L>& lines)
    {
      using namespace mln;

//       mln::util::timer t;
//       t.start();

      scribo::line_set<L> output
	= internal::draw_boxes(lines.components().labeled_image().domain(),
			       lines);
//       float ts = t.stop();
//       std::cout << ts << std::endl;

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_MERGING_HDOC_HH
