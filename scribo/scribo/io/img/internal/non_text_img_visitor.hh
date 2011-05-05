// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_IO_IMG_INTERNAL_NON_TEXT_IMG_VISITOR_HH
# define SCRIBO_IO_IMG_INTERNAL_NON_TEXT_IMG_VISITOR_HH

/// \file
///
/// \brief Save non-text information as an image.

# include <fstream>


# include <mln/core/image/image2d.hh>
# include <mln/value/rgb8.hh>
# include <mln/draw/box.hh>

# include <scribo/core/internal/doc_serializer.hh>
# include <scribo/io/img/internal/draw_edges.hh>

namespace scribo
{

  namespace io
  {

    namespace img
    {

      namespace internal
      {

	/*! \brief Save non-text information as an image.
	*/
	class non_text_img_visitor : public doc_serializer<non_text_img_visitor>
	{
	public:
	  // Constructor
	  non_text_img_visitor(mln::image2d<value::rgb8>& out);

	  // Visit overloads
	  template <typename L>
	  void visit(const document<L>& doc) const;

	  template <typename L>
	  void visit(const component_set<L>& comp_set) const;

	  void visit(const component_info& info) const;

	private: // Attributes
	  mln::image2d<value::rgb8>& output;

	  mutable image2d<scribo::def::lbl_type> elt_edge;
	};



# ifndef MLN_INCLUDE_ONLY


	inline
	non_text_img_visitor::non_text_img_visitor(mln::image2d<value::rgb8>& out)
	  : output(out)
	{
	  mln_assertion(output.is_valid());
	}



	/// Document
	//
	template <typename L>
	void
	non_text_img_visitor::visit(const document<L>& doc) const
	{
	  // Page elements (Pictures, ...)
	  if (doc.has_elements())
	  {
	    // Prepare element edges
	    elt_edge = morpho::elementary::gradient_external(doc.elements().labeled_image(), c8());

	    doc.elements().accept(*this);
	  }

	  // line seraparators
	  if (doc.has_hline_seps())
	    doc.hline_seps_comps().accept(*this);
	  if (doc.has_vline_seps())
	    doc.vline_seps_comps().accept(*this);
	}


	/// Component Set
	//
	template <typename L>
	void
	non_text_img_visitor::visit(const component_set<L>& comp_set) const
	{
	  for_all_comps(c, comp_set)
	    if (comp_set(c).is_valid())
	      comp_set(c).accept(*this);
	}


	/// Component_info
	//
	inline
	void
	non_text_img_visitor::visit(const component_info& info) const
	{
	  switch (info.type())
	  {
	    case component::HorizontalLineSeparator:
	    case component::VerticalLineSeparator:
	    {
	      mln::draw::box(output, info.bbox(), literal::cyan);
	    }
	    break;


	    default:
	    case component::Image:
	    {
	      draw_edges(info, output, literal::orange, elt_edge);
	    }
	    break;
	  }
	}


#endif // MLN_INCLUDE_ONLY

      } // end of namespace scribo::io::img::internal

    } // end of namespace scribo::io::img

  } // end of namespace scribo::io

} // end of namespace scribo

#endif // SCRIBO_IO_IMG_INTERNAL_NON_TEXT_IMG_VISITOR_HH
