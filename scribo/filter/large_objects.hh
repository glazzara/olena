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

#ifndef SCRIBO_FILTER_LARGE_OBJECTS_HH
# define SCRIBO_FILTER_LARGE_OBJECTS_HH

/// \file
///
/// Remove large objects in a binary image.


# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/concept/function.hh>

# include <mln/labeling/compute.hh>
# include <mln/accu/math/count.hh>

# include <mln/util/array.hh>

# include <mln/pw/all.hh>

# include <scribo/core/object_image.hh>
# include <scribo/extract/primitive/objects.hh>

namespace scribo
{

  namespace filter
  {

    using namespace mln;


    /// Remove large objects in a binary image.
    /// Set to 'false' all the removed objects.
    ///
    /// \param[in] input_     A binary image.
    /// \param[in] nbh_	      A neighborhood used for labeling \p input_.
    /// \param[in] label_type The label type used for labeling.
    /// \param[in] max_size   The minimum cardinality of an object.
    ///
    /// \return A binary image without large objects.
    template <typename I, typename N, typename V>
    mln_concrete(I)
    large_objects(const Image<I>& input_,
		  const Neighborhood<N>& nbh_,
		  const V& label_type,
		  unsigned max_size);

    /// Remove too large text objects.
    ///
    /// \param[in] objects    An object image.
    /// \param[in] max_size   The minimum cardinality of an object.
    ///
    /// \return updated text data.
    template <typename L>
    object_image(L)
    large_objects(const object_image(L)& objects,
		  unsigned max_size);



# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {


      /// Filter Functor.
      /// Return false for all objects which are too large.
      template <typename L>
      struct large_objects_filter
	: Function_v2b< large_objects_filter<L> >
      {

	typedef accu::math::count<mln_psite(L)> card_t;

	/// Constructor
	///
	/// \param[in] compbboxes Component bounding boxes.
	/// \param[in] max_size Maximum object size.
	large_objects_filter(const object_image(L)& objects,
			     unsigned max_size)
	{
	  card_ = labeling::compute(card_t(), objects, objects.nlabels());
	  max_size_ = max_size;
	}


	/// Check if the object is large enough.
	///
	/// \param l A label.
	///
	/// \return false if the object area is strictly inferior to
	/// \p max_size_.
	bool operator()(const mln_value(L)& l) const
	{
	  return card_[l] <= max_size_;
	}


	/// The object bounding boxes.
	mln::util::array<mln_result(card_t)> card_;
	/// The maximum area.
	unsigned max_size_;
      };


    } //  end of namespace scribo::filter::internal



    template <typename I, typename N, typename V>
    inline
    mln_concrete(I)
    large_objects(const Image<I>& input_,
		  const Neighborhood<N>& nbh_,
		  const V& label_type,
		  unsigned max_size)
    {
      trace::entering("scribo::filter::large_objects");

      const I& input = exact(input_);
      const N& nbh = exact(nbh_);

      mln_precondition(input.is_valid());
      mln_precondition(nbh.is_valid());

      V nlabels;
      typedef object_image(mln_ch_value(I,V)) lbl_t;
      lbl_t lbl = extract::primitive::objects(input, nbh, nlabels);

      typedef internal::large_objects_filter<lbl_t> func_t;
      func_t fv2b(lbl, max_size);
      labeling::relabel_inplace(lbl, nlabels, fv2b);

      mln_concrete(I) output = duplicate(input);
      data::fill((output | pw::value(lbl) == literal::zero).rw(), false);

      trace::exiting("scribo::filter::large_objects");
      return output;
    }


    template <typename L>
    inline
    object_image(L)
    large_objects(const object_image(L)& objects,
		  unsigned max_size)
    {
      trace::entering("scribo::filter::large_objects");

      mln_precondition(objects.is_valid());

      internal::large_objects_filter<L> f(objects, max_size);

      object_image(L) output;
      output.init_from_(objects);
      output.relabel(f);

      trace::exiting("scribo::filter::large_objects");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo

#endif // ! SCRIBO_FILTER_LARGE_OBJECTS_HH
