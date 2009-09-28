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

#ifndef SCRIBO_FILTER_OBJECTS_SIZE_RATIO_HH
# define SCRIBO_FILTER_OBJECTS_SIZE_RATIO_HH

/// \file
///
///

# include <mln/core/concept/function.hh>
# include <scribo/core/object_image.hh>



namespace scribo
{

  namespace filter
  {
    using namespace mln;

    template <typename L>
    object_image(L)
    objects_size_ratio(const object_image(L)& objects,
		       float size_ratio);


# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {


      /// Filter Functor.
      /// Return false for all objects which have a bad ratio.
      template <typename L>
      struct objects_size_ratio_filter
	: Function_v2b< objects_size_ratio_filter<L> >
      {
	typedef accu::shape::bbox<mln_psite(L)> box_accu_t;

	objects_size_ratio_filter(const object_image(L)& objects,
				  float ratio)
	  : objects_(objects), ratio_(ratio)
	{
	}

	bool operator()(const mln_value(L)& l) const
	{
	  if (l == literal::zero)
	    return true;
	  return (objects_.bbox(l).nrows() / static_cast<float>(objects_.bbox(l).ncols())) < ratio_;
	}

	/// Component bounding boxes.
	object_image(L) objects_;

	/// The maximum size ratio.
	float ratio_;
      };


    } //  end of namespace scribo::filter::internal



    template <typename L>
    object_image(L)
    objects_size_ratio(const object_image(L)& objects,
		       float size_ratio)
    {

      trace::entering("scribo::primitive::objects_size_ratio");

      mln_precondition(objects.is_valid());

      typedef internal::objects_size_ratio_filter<L> func_t;
      func_t has_bad_ratio(objects, size_ratio);

      object_image(L) output;
      output.init_from_(objects);
      output.relabel(has_bad_ratio);

      trace::exiting("scribo::primitive::objects_size_ratio");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_OBJECTs_SIZE_RATIO_HH
