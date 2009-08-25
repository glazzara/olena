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

#ifndef SCRIBO_PRIMITIVE_EXTRACT_OBJECTS_HH
# define SCRIBO_PRIMITIVE_EXTRACT_OBJECTS_HH

/// \file
///
/// Extract objects in a binary image.


# include <mln/core/concept/neighborhood.hh>
# include <mln/core/site_set/box.hh>

# include <mln/accu/shape/bbox.hh>

# include <mln/labeling/blobs_and_compute.hh>
# include <mln/labeling/compute.hh>

# include <mln/util/array.hh>

# include <mln/debug/println.hh>

# include <scribo/core/object_image.hh>

#include <mln/accu/shape/bbox.hh>
#include <mln/accu/center.hh>


namespace scribo
{

  namespace primitive
  {

    namespace extract
    {

      using namespace mln;

      /// Extract objects in a binary image.
      ///
      /// \param[in]	 input	  A binary image. Objects are must be set
      ///                         to 'true'
      ///			  and background to 'false'.
      /// \param[in]	 nbh	  A neighborhood to be used for labeling.
      /// \param[in,out] nobjects Will store the numbers of objects found.
      ///
      /// \return An image of labeled objects.
      //
      template <typename I, typename N, typename V>
      inline
      object_image(mln_ch_value(I,V))
      objects(const Image<I>& input,
	      const Neighborhood<N>& nbh, V& nobjects);


# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

        template <typename I, typename N, typename V>
        inline
	void
        objects_tests(const Image<I>& input,
		      const Neighborhood<N>& nbh, V& nobjects)
	{
	  mlc_equal(mln_value(I),bool)::check();
	  mlc_is_a(V, mln::value::Symbolic)::check();
	  mln_precondition(exact(input).is_valid());
	  mln_precondition(exact(nbh).is_valid());
	  (void) input;
	  (void) nbh;
	  (void) nobjects;
	}


      } // end of namespace scribo::primitive::extract::internal


      template <typename I, typename N, typename V>
      inline
      object_image(mln_ch_value(I,V))
      objects(const Image<I>& input,
	      const Neighborhood<N>& nbh, V& nobjects)
      {
	trace::entering("scribo::objects");

	internal::objects_tests(input, nbh, nobjects);

	typedef mln_ch_value(I,V) L;
	typedef accu::shape::bbox<mln_psite(I)> accu_bbox;

	util::couple<L, util::array<mln_box(I)> >
	  results = labeling::blobs_and_compute(input, nbh, nobjects,
						accu_bbox());

	object_image(L)
	  output(results.first(), nobjects, results.second());

	trace::exiting("scribo::objects");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // ! SCRIBO_PRIMITIVE_EXTRACT_OBJECTS_HH
