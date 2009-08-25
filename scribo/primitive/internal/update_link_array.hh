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

#ifndef SCRIBO_PRIMITIVE_INTERNAL_UPDATE_LINK_ARRAY_HH
# define SCRIBO_PRIMITIVE_INTERNAL_UPDATE_LINK_ARRAY_HH

/// \file
///
/// Update a lookup table if a neighbor is found on the right of
/// the current bbox.


# include <mln/core/concept/image.hh>
# include <mln/util/array.hh>
# include <mln/math/abs.hh>


namespace scribo
{

  namespace primitive
  {

    namespace internal
    {

      using namespace mln;


      /// Update the lookup table \p link_array if a neighbor is found
      /// on the right of the current bbox.
      ///
      /// \param[in] lbl A label image.
      /// \param[in] link_array Links of bounding boxes.
      /// \param[in] p A site of \p lbl.
      /// \param[in] c A site of \p lbl.
      //
      template <typename I>
      void
      update_link_array(const Image<I>& lbl,
			mln::util::array<unsigned>& link_array,
			const mln_site(I)& p, const mln_site(I)& c,
			unsigned i, int dmax);

# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      void
      update_link_array(const Image<I>& lbl_,
			mln::util::array<unsigned>& link_array,
			const mln_site(I)& p, const mln_site(I)& c,
			unsigned i, int dmax)
      {
	const I& lbl = exact(lbl_);

	mlc_is_a(mln_value(I), mln::value::Symbolic)::check();
	mln_assertion(lbl.is_valid());

	if (lbl.domain().has(p) // Not outside image domain
	    && lbl(p) != literal::zero // Not the background
	    && lbl(p) != i // Not the current component
	    && (math::abs(p.col() - c.col())) < dmax // Not too far
	    && link_array[lbl(p)] != i) // Not creating a loop
	  link_array[i] = lbl(p);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::internal

  } // end of namespace scribo::text

} // end of namespace scribo


#endif // ! SCRIBO_PRIMITIVE_INTERNAL_UPDATE_LINK_ARRAY_HH
