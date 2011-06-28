// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_SRC_AFP_LINK_HH
# define SCRIBO_SRC_AFP_LINK_HH

/// \file
///
/// Specific implementation for AFP use case.

# include <mln/geom/ncols.hh>
# include <mln/geom/nrows.hh>
# include <mln/util/couple.hh>
# include <mln/math/max.hh>

# include <scribo/core/object_links.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/macros.hh>

namespace scribo
{

  namespace primitive
  {

    namespace link
    {

      using namespace mln;


      template <typename L>
      mln::util::couple<object_links<L>, object_links<L> >
      left_right(const component_set<L>& components)
      {
	object_links<L>
	  right(components, static_cast<unsigned>(components.nelements()) + 1);
	right.init();

	object_links<L>
	  left(components, static_cast<unsigned>(components.nelements()) + 1);
	left.init();

	const L& lbl_ima = components.labeled_image();

	for_all_comps(i, components)
	{
	  float
	    w = components(i).bbox().width(),
	    h = components(i).bbox().height();
	  unsigned dmax = (w / 2.0f) + (3 * math::max(w, h));


	  const mln_site(L) c = components(i).mass_center();

	  int
	    midcol = (components(i).bbox().pmax().col()
		      - components(i).bbox().pmin().col()) / 2;
	  int
	    nrightima = geom::ncols(lbl_ima) - c.col(),
	    nleftima = c.col(),
	    nright = std::min(static_cast<unsigned>(nrightima), midcol + dmax),
	    nleft = std::min(static_cast<unsigned>(nleftima), midcol + dmax);

	  // Right
	  {
	    const mln_value(L)
	      *p = &lbl_ima(c),
	      *pstop = p + nright + 1;

	    for (; p != pstop; ++p)
	    {
	      if (*p != literal::zero  // Not the background
		  && *p != i // Not the current component
		  && right(*p) != i) // No loops
	      {
		right.update(i, *p);
		break;
	      }
	    }
	  }


	  // Left
	  {
	    const mln_value(L)
	      *p = &lbl_ima(c),
	      *pstop = p - nleft - 1;

	    for (; p != pstop; --p)
	    {
	      if (*p != literal::zero  // Not the background
		  && *p != i // Not the current component
		  && left(*p) != i) // No loops
	      {
		left.update(i, *p);
		break;
	      }
	    }
	  }
	}

	return mln::make::couple(left, right);
      }


      template <typename L>
      object_links<L>
      left(const component_set<L>& components, unsigned dmax)
      {
	object_links<L>
	  left(components, static_cast<unsigned>(components.nelements()) + 1);
	left.init();

	const L& lbl_ima = components.labeled_image();

	for_all_comps(i, components)
	{
	  const mln_site(L) c = components(i).mass_center();

	  int
	    midcol = (components(i).bbox().pmax().col()
		      - components(i).bbox().pmin().col()) / 2;
	  int
	    nleftima = c.col(),
	    nleft = std::min(static_cast<unsigned>(nleftima), midcol + dmax);

	  // Left
	  {
	    const mln_value(L)
	      *p = &lbl_ima(c),
	      *pstop = p - nleft - 1;

	    for (; p != pstop; --p)
	    {
	      if (*p != literal::zero  // Not the background
		  && *p != i // Not the current component
		  && left(*p) != i) // No loops
	      {
		left(i) = *p;
		break;
	      }
	    }
	  }
	}

	return left;
      }

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_SRC_AFP_LINK_HH
