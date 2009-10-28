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

#ifndef SCRIBO_DEBUG_ALIGNMENT_DECISION_IMAGE_HH
# define SCRIBO_DEBUG_ALIGNMENT_DECISION_IMAGE_HH

/// \file
///
/// Save a color image showing the difference between to object groups.

# include <mln/core/concept/image.hh>
# include <mln/data/convert.hh>
# include <mln/value/rgb8.hh>
# include <mln/literal/colors.hh>
# include <mln/util/array.hh>
# include <mln/util/couple.hh>

# include <scribo/core/object_groups.hh>
# include <scribo/draw/bounding_boxes.hh>


namespace scribo
{

  namespace debug
  {

    using namespace mln;

    enum Alignment { top, center, bottom };

    /*! \brief Save a color image showing the difference between to
        object links.

	\input[in] input An image. It's value type must be convertible
                         towards rgb8.
	\input[in] links          Object links information.
	\input[in] filtered_links A copy of \p links which have been
	                           filtered.

	\return A color image. Non filtered links are drawn in
	        green. Others are drawn in red.
     */
    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    alignment_decision_image(const Image<I>& input_,
			     const object_links<L>& links,
			     const object_links<L>& filtered_links,
			     const Alignment& alignment);


# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      template <typename L>
      mln::util::couple<mln_site(L),mln_site(L)>
      find_anchors(const object_image(L)& objects,
		   unsigned i,
		   unsigned link_i,
		   const Alignment& alignment)
      {
	mln_site(L)
	  a1 = objects.bbox(i).center(),
	  a2 = objects.bbox(link_i).center();

	switch (alignment)
	{
	  case top:
	    a1.row() = objects.bbox(i).pmin().row();
	    a2.row() = objects.bbox(link_i).pmin().row();
	    break;

	  case center:
	    break;

	  case bottom:
	    a1.row() = objects.bbox(i).pmax().row();
	    a2.row() = objects.bbox(link_i).pmax().row();
	    break;

	}
	return make::couple(a1, a2);
      }

    }

    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    alignment_decision_image(const Image<I>& input_,
			     const object_links<L>& links,
			     const object_links<L>& filtered_links,
			     const Alignment& alignment)
    {
      trace::entering("scribo::debug::alignment_decision_image");
      const I& input = exact(input_);

      const object_image(L)& objects = links.object_image_();

      mln_precondition(input.is_valid());
      mln_precondition(links.is_valid());
      mln_precondition(filtered_links.is_valid());
      mln_precondition(links.size() == filtered_links.size());
      mln_precondition(links.object_image_() != filtered_links.object_image_());
      /// Fixme: check that objects has been computed from input.

      image2d<value::rgb8>
	decision_image = data::convert(value::rgb8(), input);

      for_all_components(i, objects.bboxes())
	mln::draw::box(decision_image, objects.bbox(i), literal::blue);

      typedef mln_site(L) P;

      for (unsigned i = 1; i < links.size(); ++i)
      {

	if (links[i] != i)
	{
	  value::rgb8 value = literal::green;
	  if (links[i] != filtered_links[i])
	    value = literal::red;

	  mln::util::couple<P,P>
	    anchors = internal::find_anchors(objects, i, links[i], alignment);
	  mln::draw::line(decision_image,
			  anchors.first(),
			  anchors.second(),
			  value);
	}
      }

      trace::exiting("scribo::debug::alignment_decision_image");
      return decision_image;
    }



# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo


#endif // ! SCRIBO_DEBUG_ALIGNMENT_DECISION_IMAGE_HH
