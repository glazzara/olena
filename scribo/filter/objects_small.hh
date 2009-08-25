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

#ifndef SCRIBO_FILTER_OBJECTS_SMALL_HH
# define SCRIBO_FILTER_OBJECTS_SMALL_HH

/// \file
///
/// Remove small objects in a binary image.


# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/concept/function.hh>

# include <mln/labeling/blobs.hh>
# include <mln/labeling/relabel.hh>

# include <mln/make/relabelfun.hh>

# include <mln/util/array.hh>

# include <mln/pw/all.hh>

# include <mln/accu/math/count.hh>

# include <mln/set/compute.hh>


# include <scribo/core/object_image.hh>
# include <scribo/fun/v2b/objects_small_filter.hh>
# include <scribo/primitive/extract/objects.hh>


namespace scribo
{

  namespace filter
  {

    using namespace mln;


    /// Remove small objects in a binary image.
    /// Set to 'false' all the removed objects.
    ///
    /// \param[in] input_     A binary image.
    /// \param[in] nbh_	      A neighborhood used for labeling \p input_.
    /// \param[in] label_type The label type used for labeling.
    /// \param[in] min_size   The minimum cardinality of an object.
    ///
    /// \return A binary image without small objects.
    template <typename I, typename N, typename V>
    mln_concrete(I)
    objects_small(const Image<I>& input_,
		  const Neighborhood<N>& nbh_,
		  const V& label_type,
		  unsigned min_size);


    /// Remove too small objects.
    ///
    /// \param[in] objects    An object image.
    /// \param[in] min_size   The minimum cardinality of an object.
    ///
    /// \return An object image without small objects.
    template <typename L>
    object_image(L)
    objects_small(const object_image(L)& objects,
		  unsigned min_size);


# ifndef MLN_INCLUDE_ONLY


   template <typename I, typename N, typename V>
   inline
   mln_concrete(I)
   objects_small(const Image<I>& input_,
		 const Neighborhood<N>& nbh_,
		 const V& label_type,
		 unsigned min_size)
   {
     trace::entering("scribo::filter::objects_small");

     const I& input = exact(input_);
     const N& nbh = exact(nbh_);

     mln_precondition(input.is_valid());
     mln_precondition(nbh.is_valid());

     V nlabels;
     typedef mln_ch_value(I,V) lbl_t;
     object_image(lbl_t) lbl = primitive::extract::objects(input, nbh, nlabels);

     typedef fun::v2b::objects_small_filter<mln_ch_value(I,V)> func_t;
     func_t fv2b(lbl, min_size);
     lbl.relabel(fv2b);

     mln_concrete(I) output = duplicate(input);
     data::fill((output | pw::value(lbl) == pw::cst(literal::zero)).rw(),
		false);

     trace::exiting("scribo::filter::objects_small");
     return output;
   }


    template <typename L>
    inline
    object_image(L)
    objects_small(const object_image(L)& objects,
		  unsigned min_size)
    {
      trace::entering("scribo::filter::objects_small");

      mln_precondition(objects.is_valid());

      fun::v2b::objects_small_filter<L> f(objects, min_size);

      object_image(L) output;
      output.init_from_(objects);
      output.relabel(f);

      trace::exiting("scribo::filter::objects_small");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo

#endif // ! SCRIBO_FILTER_OBJECTS_SMALL_HH
