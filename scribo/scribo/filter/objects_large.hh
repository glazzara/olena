// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
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

#ifndef SCRIBO_FILTER_COMPONENTS_LARGE_HH
# define SCRIBO_FILTER_COMPONENTS_LARGE_HH

/// \file
///
/// \brief Remove large objects in a binary image.


# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/concept/function.hh>

# include <mln/labeling/compute.hh>
# include <mln/accu/math/count.hh>

# include <mln/util/array.hh>

# include <mln/pw/all.hh>

# include <scribo/core/component_set.hh>
# include <scribo/primitive/extract/components.hh>
# include <scribo/fun/v2b/objects_large_filter.hh>

namespace scribo
{

  namespace filter
  {

    using namespace mln;


    /// \brief Remove large objects in a binary image.
    ///
    /// Set to 'false' all the removed objects.
    ///
    /// \param[in] input_     A binary image.
    /// \param[in] nbh_	      A neighborhood used for labeling \p input_.
    /// \param[in] label_type The label type used for labeling.
    /// \param[in] max_size   The minimum cardinality of an object.
    ///
    /// \return A binary image without large objects.
    ///
    /// \ingroup grpalgofiltercomp
    //
    template <typename I, typename N, typename V>
    mln_concrete(I)
    components_large(const Image<I>& input_,
		  const Neighborhood<N>& nbh_,
		  const V& label_type,
		  unsigned max_size);

    /// \brief Remove too large components.
    ///
    /// \param[in] components    An object image.
    /// \param[in] max_size   The maximum cardinality of an object.
    ///
    /// \return A component set with large components set to
    /// component::Ignored.
    ///
    /// \ingroup grpalgofiltercomp
    //
    template <typename L>
    inline
    component_set<L>
    components_large(const component_set<L>& components,
		     unsigned max_size);


# ifndef MLN_INCLUDE_ONLY



   template <typename I, typename N, typename V>
   inline
   mln_concrete(I)
   components_large(const Image<I>& input_,
		    const Neighborhood<N>& nbh_,
		    V& nlabels,
		    unsigned min_size)
   {
     mln_trace("scribo::filter::components_large");

     const I& input = exact(input_);
     const N& nbh = exact(nbh_);

     mln_precondition(input.is_valid());
     mln_precondition(nbh.is_valid());

     typedef mln_ch_value(I,V) lbl_t;
     component_set<lbl_t>
       lbl = primitive::extract::components(input, nbh, nlabels);

     typedef fun::v2b::components_large_filter<mln_ch_value(I,V)> func_t;
     func_t fv2b(lbl, min_size);
     mln_concrete(I)
       output = mln::data::transform(lbl.labeled_image(), fv2b);
     nlabels = fv2b.nlabels_;

     return output;
   }



    template <typename L>
    inline
    component_set<L>
    components_large(const component_set<L>& components,
		     unsigned max_size)
    {
      mln_trace("scribo::filter::components_large");

      mln_precondition(components.is_valid());

      fun::v2b::components_large_filter<L> f(components, max_size);

      component_set<L> output = components.duplicate();
      output.update_tags(f, component::Ignored);

      return output;
    }



# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo

#endif // ! SCRIBO_FILTER_COMPONENTS_LARGE_HH
