// Copyright (C) 2009, 2010, 2011, 2013 EPITA Research and Development
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

#ifndef SCRIBO_PRIMITIVE_LINK_INTERNAL_COMPUTE_ANCHOR_HH
# define SCRIBO_PRIMITIVE_LINK_INTERNAL_COMPUTE_ANCHOR_HH

/// \file
///
/// Routine providing anchors for neighbor seeking.

# include <mln/math/min.hh>

# include <scribo/core/tag/anchor.hh>
# include <scribo/core/component_set.hh>


namespace scribo
{

  namespace primitive
  {

    namespace link
    {

      namespace internal
      {
	using namespace mln;

	/*! \brief Return the proper anchor used to find a neighbor.

	  \param[in] components     A component set.
	  \param[in] current_object An object id.
	  \param[in] anchor         The expected anchor.

	  Anchor can take one of the values defined in the
	  scribo::anchor::Type enum.


	  Top and bottom anchors are respectively computed from the
	  bbox pmin and the bbox pmax, P.

	  Then the output anchor coordinates are computed as follows:

	  out.col = bbox.center.col

	  if object height, h, is lower than 30.
	   out.row = P.row + min(2, (h + 1) / 2 - 1)
	  else
	   out.row = P.row - min(10, h /10)

	 */
	template <typename L>
	mln_site(L)
	compute_anchor(const component_set<L>& components,
		       unsigned current_object, anchor::Type anchor);


# ifndef MLN_INCLUDE_ONLY

	template <typename L>
	mln_site(L)
	compute_anchor(const component_set<L>& components,
		       unsigned current_object, anchor::Type anchor)
	{
	  typedef mln_site(L) P;

	  unsigned h = components(current_object).bbox().height();
	  unsigned w = components(current_object).bbox().width();

	  mln_site(L) sp = components(current_object).bbox().pcenter();

	  switch (anchor)
	  {
	    // Component masss center
	    case anchor::MassCenter:
	      return components(current_object).mass_center();


	    // Bounding box top center
	    case anchor::Top:
	      if (h < 30)
		sp.row() = components(current_object).bbox().pmin().row()
		  + math::min(2u, (h + 1) / 2 - 1);
	      else
		sp.row() = components(current_object).bbox().pmin().row()
		  + math::min(10u, h /10);
	      break;


	      // Bounding box Strict top center
	    case anchor::StrictTopCenter:
	      sp.row() = components(current_object).bbox().pmin().row();
	      break;


	      // Bounding box top left
	    case anchor::TopStrictLeft:
	      sp.col() = components(current_object).bbox().pmin().col();
	      sp.row() = components(current_object).bbox().pmin().row()
		+ math::min(2u, (h + 1) / 2 - 1);
	      break;


	      // Bounding box bottom right
	    case anchor::BottomStrictRight:
	      sp.col() = components(current_object).bbox().pmax().col();
	      sp.row() = components(current_object).bbox().pmax().row()
		- math::min(2u, (h + 1) / 2 - 1);
	      break;


	    // Bounding box bottom center
	    case anchor::Bottom:
	      if (h < 30)
		sp.row() = components(current_object).bbox().pmax().row()
		  - math::min(2u, (h + 1) / 2 - 1);
	      else
		sp.row() = components(current_object).bbox().pmax().row()
		  - math::min(10u, h /10);
	      break;


	      // Bounding box strict bottom center.
	    case anchor::StrictBottomCenter:
	      sp.row() = components(current_object).bbox().pmax().row();
	      break;


	    // Bounding box center
	    case anchor::Center:
	      return components(current_object).bbox().pcenter();


	    // Bounding box actual left center
	    case anchor::StrictLeft:
	      return P(components(current_object).bbox().pcenter().row(),
		       components(current_object).bbox().pmin().col());


	    // Bounding box left center
	    case anchor::Left:
	      if (w < 30)
		sp.col() = components(current_object).bbox().pmin().col()
		  + math::min(2u, (w + 1) / 2 - 1);
	      else
		sp.col() = components(current_object).bbox().pmin().col()
		  + math::min(10u, w /10);
	      break;


	    // Bounding box actual right center
	    case anchor::StrictRight:
	      return P(components(current_object).bbox().pcenter().row(),
		       components(current_object).bbox().pmax().col());


	    // Bounding box right center
	    case anchor::Right:
	      if (w < 30)
		sp.col() = components(current_object).bbox().pmax().col()
		  - math::min(2u, (w + 1) / 2 - 1);
	      else
		sp.col() = components(current_object).bbox().pmax().col()
		  - math::min(10u, w /10);
	      break;


	    // Bounding box top left
	    case anchor::TopLeft:
	      if (h < 30)
		sp.row() = components(current_object).bbox().pmin().row()
		  + math::min(2u, (h + 1) / 2 - 1);
	      else
		sp.row() = components(current_object).bbox().pmin().row()
		  + math::min(10u, h /10);
	      if (w < 30)
		sp.col() = components(current_object).bbox().pmin().col()
		  + math::min(2u, (w + 1) / 2 - 1);
	      else
		sp.col() = components(current_object).bbox().pmin().col()
		  + math::min(10u, w /10);
	      break;


	    // Bounding box top right
	    case anchor::TopRight:
	      if (h < 30)
		sp.row() = components(current_object).bbox().pmin().row()
		  + math::min(2u, (h + 1) / 2 - 1);
	      else
		sp.row() = components(current_object).bbox().pmin().row()
		  + math::min(10u, h /10);
	      if (w < 30)
		sp.col() = components(current_object).bbox().pmax().col()
		  - math::min(2u, (w + 1) / 2 - 1);
	      else
		sp.col() = components(current_object).bbox().pmax().col()
		  - math::min(10u, w /10);
	      break;


	    // Bounding box bottom left
	    case anchor::BottomLeft:
	      if (h < 30)
		sp.row() = components(current_object).bbox().pmax().row()
		  - math::min(2u, (h + 1) / 2 - 1);
	      else
		sp.row() = components(current_object).bbox().pmax().row()
		  - math::min(10u, h /10);
	      if (w < 30)
		sp.col() = components(current_object).bbox().pmin().col()
		  + math::min(2u, (w + 1) / 2 - 1);
	      else
		sp.col() = components(current_object).bbox().pmin().col()
		  + math::min(10u, w /10);
	      break;

	    // Bounding box bottom right
	    case anchor::BottomRight:
	      if (h < 30)
		sp.row() = components(current_object).bbox().pmax().row()
		  - math::min(2u, (h + 1) / 2 - 1);
	      else
		sp.row() = components(current_object).bbox().pmax().row()
		  - math::min(10u, h /10);
	      if (w < 30)
		sp.col() = components(current_object).bbox().pmax().col()
		  - math::min(2u, (w + 1) / 2 - 1);
	      else
		sp.col() = components(current_object).bbox().pmax().col()
		  - math::min(10u, w /10);
	      break;



	    default:
	      mln_trace_warning("Non handled anchor");
	      mln_assertion(anchor < anchor::Invalid);
	  }

	  return sp;
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::primitive::link::internal

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_LINK_INTERNAL_COMPUTE_ANCHOR_HH
