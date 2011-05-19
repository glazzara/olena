// Copyright (C) 2011 EPITA Research and Development Laboratory
// (LRDE)
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

/// \file
///
/// \brief Find in a document non text which are not text.
///
/// \fixme To be optimized!

#ifndef SCRIBO_PRIMITIVE_EXTRACT_NON_TEXT_HDOC_HH
# define SCRIBO_PRIMITIVE_EXTRACT_NON_TEXT_HDOC_HH

# include <mln/morpho/elementary/dilation.hh>

# include <mln/draw/box_plain.hh>
# include <mln/morpho/closing/structural.hh>
# include <mln/win/rectangle2d.hh>

# include <scribo/make/text_components_image.hh>
# include <scribo/make/text_blocks_image.hh>

# include <scribo/primitive/extract/internal/union.hh>
# include <scribo/debug/logger.hh>

# include <scribo/filter/objects_small.hh>
# include <scribo/filter/objects_on_border.hh>
# include <scribo/filter/objects_v_thin.hh>
# include <scribo/filter/objects_h_thin.hh>

//DEBUG
#include <mln/util/timer.hh>
#include <mln/io/pbm/save.hh>


namespace scribo
{

  namespace primitive
  {

    namespace extract
    {

      using namespace mln;

      /*! \brief Extract non text components.

	Variant adapted for historical documents.
       */
      template <typename L>
      component_set<L>
      non_text_hdoc(const document<L>& doc, unsigned closing_size);


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

      } // end of namespace scribo::primitive::extract::internal



      // FACADE

      template <typename L>
      component_set<L>
      non_text_hdoc(const document<L>& doc, unsigned closing_size)
      {
	trace::entering("scribo::primitive::extract::non_text_hdoc");

	mln_precondition(doc.is_valid());
	mln_precondition(doc.has_text());

	mln_ch_value(L,bool)
	  element_image = duplicate(doc.binary_image_wo_seps());

	for_all_lines(l, doc.lines())
	  if (doc.lines()(l).is_textline())
	    mln::draw::box_plain(element_image, doc.lines()(l).bbox(), false);

	element_image = morpho::closing::structural(element_image,
						    win::rectangle2d(closing_size,
								     closing_size));

	mln_value(L) ncomps;
	component_set<L>
	  elements = primitive::extract::components(element_image,
						    c8(), ncomps);

	elements = scribo::filter::components_small(elements, 200);
	elements = scribo::filter::components_on_border(elements);
	elements = scribo::filter::objects_v_thin(elements, 100);
	elements = scribo::filter::objects_h_thin(elements, 100);

	// Debug
	{
	  debug::logger().log_image(debug::Special,
				    elements.labeled_image(),
				    "non_text_hdoc_components");
	}

	trace::exiting("scribo::primitive::extract::non_text_hdoc");
	return elements;
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_EXTRACT_NON_TEXT_HDOC_HH
