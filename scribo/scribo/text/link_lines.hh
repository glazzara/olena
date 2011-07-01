// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_TEXT_LINK_LINES_HH
# define SCRIBO_TEXT_LINK_LINES_HH

/// \file
///
/// \brief Verticaly link text lines to prepare paragraph
/// construction.

#include <mln/value/int_u16.hh>
#include <mln/data/fill.hh>
#include <mln/geom/rotate.hh>
#include <mln/geom/ncols.hh>
#include <mln/draw/box_plain.hh>
#include <mln/util/array.hh>

#include <scribo/core/line_links.hh>


namespace scribo
{

  namespace text
  {

    using namespace mln;


    template <typename L>
    line_links<L>
    link_lines(const line_set<L>& lines);


# ifndef MLN_INCLUDE_ONLY


    template <typename L>
    line_links<L>
    link_lines(const line_set<L>& lines)
    {
      typedef value::int_u16 V;

      const mln_concrete(L)& lbl = lines.components().labeled_image();

      // Rotate the domain in order to browse the image efficiently.
      mln_ch_value(L,V) blocks(geom::rotate(lbl.domain(),
					    -90, lbl.domain().pcenter()));
      mln::data::fill(blocks, literal::zero);

      // Construct a list of rotated bboxes for each lines.
      mln::util::array<mln_box(L)> rbbox(1);
      rbbox.reserve(lines.nelements());

      for_all_lines(l, lines)
      {
	if (! lines(l).is_valid() || lines(l).is_hidden() || lines(l).type() != line::Text)
	{
	  rbbox.resize(rbbox.nelements() + 1);
	  continue;
	}

	mln_box(L) b = mln::geom::rotate(lines(l).bbox(), -90, lbl.domain().pcenter());
	rbbox.append(b);
	mln::draw::box_plain(blocks, b, l);
      }

      // Looking for neighbor lines.
      line_links<L> links(lines);
      links.init();

      for_all_lines(l, lines)
      {
	if (! lines(l).is_valid() || lines(l).is_hidden() || lines(l).type() != line::Text)
	  continue;

	int dmax = 1.5 * lines(l).x_height(); // FIXME: better ratio?
	mln_site(L) c = rbbox(l).pcenter();

	int
	  midcol = (rbbox(l).pmax().col()
		    - rbbox(l).pmin().col()) / 2;

	int
	  nleftima = std::abs(c.col() - blocks.domain().pmin().col()), // abs, useful?
	  nleft = std::min(nleftima, midcol + dmax);

	// Left
	{
	  const V
	    *p = &blocks(c),
	    *pstop = p - nleft;

	  for (; p != pstop; --p)
	  {
	    if (*p != literal::zero  // Not the background
		&& *p != l // Not the current component
		&& links(*p) != l) // No loops
	    {
	      links(l) = *p;
	      break;
	    }
	  }
	}

	// Right
	{
	  int
	    nrightima = geom::ncols(blocks) - c.col() + blocks.domain().pmin().col(),
	    nright = std::min(nrightima, midcol + dmax);

	  const V
	    *p = &blocks(c),
	    *pstop = p + nright - 1;

	  for (; p != pstop; ++p)
	  {
	    if (*p != literal::zero  // Not the background
		&& *p != l // Not the current component
		&& links(l) != *p  // No loops
		&& links(*p) == *p) // Not already connected
	    {
	      links(*p) = l;
	      break;
	    }
	  }
	}
      }

      return links;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::text

} // end of namespace scribo


#endif // ! SCRIBO_TEXT_LINK_LINES_HH
