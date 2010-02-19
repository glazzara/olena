// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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
# include <mln/util/array.hh>

# include <scribo/core/object_image.hh>


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

	  \param[in] objects        An object image.
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
	compute_anchor(const object_image(L)& objects,
		       unsigned current_object, anchor::Type anchor);


# ifndef MLN_INCLUDE_ONLY

	template <typename L>
	mln_site(L)
	compute_anchor(const object_image(L)& objects,
		       unsigned current_object, anchor::Type anchor)
	{
	  typedef mln_site(L) P;

	  unsigned h = objects.bbox(current_object).pmax().row()
	               - objects.bbox(current_object).pmin().row();
	  unsigned w = objects.bbox(current_object).pmax().col()
	               - objects.bbox(current_object).pmin().col();

	  mln_site(L) sp = objects.bbox(current_object).center();

	  switch (anchor)
	  {
	    // Component masss center
	    case anchor::MassCenter:
	      return objects.mass_center(current_object);


	    // Bounding box top center
	    case anchor::Top:
	      if (h < 30)
		sp.row() = objects.bbox(current_object).pmin().row()
		  + math::min(2u, (h + 1) / 2 - 1);
	      else
		sp.row() = objects.bbox(current_object).pmin().row()
		  + math::min(10u, h /10);
	      break;


	    // Bounding box bottom center
	    case anchor::Bottom:
	      if (h < 30)
		sp.row() = objects.bbox(current_object).pmax().row()
		  - math::min(2u, (h + 1) / 2 - 1);
	      else
		sp.row() = objects.bbox(current_object).pmax().row()
		  - math::min(10u, h /10);
	      break;


	    // Bounding box center
	    case anchor::Center:
	      return objects.bbox(current_object).center();


	    // Bounding box actual left center
	    case anchor::ActualLeft:
	      return P(objects.bbox(current_object).center().row(),
		       objects.bbox(current_object).pmin().col());


	    // Bounding box left center
	    case anchor::Left:
	      if (w < 30)
		sp.col() = objects.bbox(current_object).pmin().col()
		  + math::min(2u, (w + 1) / 2 - 1);
	      else
		sp.col() = objects.bbox(current_object).pmin().col()
		  + math::min(10u, w /10);
	      break;


	    // Bounding box actual right center
	    case anchor::ActualRight:
	      return P(objects.bbox(current_object).center().row(),
		       objects.bbox(current_object).pmax().col());


	    // Bounding box right center
	    case anchor::Right:
	      if (w < 30)
		sp.col() = objects.bbox(current_object).pmax().col()
		  - math::min(2u, (w + 1) / 2 - 1);
	      else
		sp.col() = objects.bbox(current_object).pmax().col()
		  - math::min(10u, w /10);
	      break;


	    // Bounding box top left
	    case anchor::TopLeft:
	      if (h < 30)
		sp.row() = objects.bbox(current_object).pmin().row()
		  + math::min(2u, (h + 1) / 2 - 1);
	      else
		sp.row() = objects.bbox(current_object).pmin().row()
		  + math::min(10u, h /10);
	      if (w < 30)
		sp.col() = objects.bbox(current_object).pmin().col()
		  + math::min(2u, (w + 1) / 2 - 1);
	      else
		sp.col() = objects.bbox(current_object).pmin().col()
		  + math::min(10u, w /10);
	      break;


	    // Bounding box top right
	    case anchor::TopRight:
	      if (h < 30)
		sp.row() = objects.bbox(current_object).pmin().row()
		  + math::min(2u, (h + 1) / 2 - 1);
	      else
		sp.row() = objects.bbox(current_object).pmin().row()
		  + math::min(10u, h /10);
	      if (w < 30)
		sp.col() = objects.bbox(current_object).pmax().col()
		  - math::min(2u, (w + 1) / 2 - 1);
	      else
		sp.col() = objects.bbox(current_object).pmax().col()
		  - math::min(10u, w /10);
	      break;


	    // Bounding box bottom left
	    case anchor::BottomLeft:
	      if (h < 30)
		sp.row() = objects.bbox(current_object).pmax().row()
		  - math::min(2u, (h + 1) / 2 - 1);
	      else
		sp.row() = objects.bbox(current_object).pmax().row()
		  - math::min(10u, h /10);
	      if (w < 30)
		sp.col() = objects.bbox(current_object).pmin().col()
		  + math::min(2u, (w + 1) / 2 - 1);
	      else
		sp.col() = objects.bbox(current_object).pmin().col()
		  + math::min(10u, w /10);
	      break;

	    // Bounding box bottom right
	    case anchor::BottomRight:
	      if (h < 30)
		sp.row() = objects.bbox(current_object).pmax().row()
		  - math::min(2u, (h + 1) / 2 - 1);
	      else
		sp.row() = objects.bbox(current_object).pmax().row()
		  - math::min(10u, h /10);
	      if (w < 30)
		sp.col() = objects.bbox(current_object).pmax().col()
		  - math::min(2u, (w + 1) / 2 - 1);
	      else
		sp.col() = objects.bbox(current_object).pmax().col()
		  - math::min(10u, w /10);
	      break;



	    default:
	      trace::warning("Non handled anchor");
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
