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

#ifndef SCRIBO_IO_IMG_INTERNAL_FULL_IMG_VISITOR_HH
# define SCRIBO_IO_IMG_INTERNAL_FULL_IMG_VISITOR_HH

/// \file
///
/// Save document image analysis to an image.

# include <fstream>

# include <mln/core/image/image2d.hh>
# include <mln/value/rgb8.hh>
# include <mln/draw/box.hh>

# include <scribo/core/internal/doc_serializer.hh>
# include <scribo/core/document.hh>
# include <scribo/core/paragraph_set.hh>
# include <scribo/core/line_info.hh>

# include <scribo/io/img/internal/draw_edges.hh>

namespace scribo
{

  namespace io
  {

    namespace img
    {

      namespace internal
      {


	class full_img_visitor : public doc_serializer<full_img_visitor>
	{
	public:
	  // Constructor
	  full_img_visitor(mln::image2d<value::rgb8>& out);

	  // Visit overloads
	  template <typename L>
	  void visit(const document<L>& doc) const;

	  template <typename L>
	  void visit(const component_info<L>& info) const;

	  template <typename L>
	  void visit(const paragraph_set<L>& parset) const;

	  template <typename L>
	  void visit(const line_info<L>& line) const;

	private: // Attributes
	  mln::image2d<value::rgb8>& output;

	  mutable image2d<scribo::def::lbl_type> elt_edge;
	};



# ifndef MLN_INCLUDE_ONLY


	inline
	full_img_visitor::full_img_visitor(mln::image2d<value::rgb8>& out)
	  : output(out)
	{
	  mln_assertion(output.is_valid());
	}


	/// Document
	//
	template <typename L>
	void
	full_img_visitor::visit(const document<L>& doc) const
	{
	  // Text
	  if (doc.has_text())
	    doc.paragraphs().accept(*this);

	  // Page elements (Pictures, ...)
	  if (doc.has_elements())
	  {
	    // Prepare element edges
	    elt_edge = morpho::elementary::gradient_external(doc.elements().labeled_image(), c8());

	    const component_set<L>& elts = doc.elements();
	    for_all_comps(e, elts)
	      if (elts(e).is_valid())
		elts(e).accept(*this);
	  }


	  // line seraparators
	  if (doc.has_vline_seps())
	    for_all_comps(c, doc.vline_seps_comps())
	      doc.vline_seps_comps()(c).accept(*this);
	  if (doc.has_hline_seps())
	    for_all_comps(c, doc.hline_seps_comps())
	      doc.hline_seps_comps()(c).accept(*this);

	}


	/// Component_info
	//
	template <typename L>
	void
	full_img_visitor::visit(const component_info<L>& info) const
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

	/// Paragraph Set
	//
	template <typename L>
	void
	full_img_visitor::visit(const paragraph_set<L>& parset) const
	{
	  const line_set<L>& lines = parset.lines();

	  for_all_paragraphs(p, parset)
	  {
	    const mln::util::array<line_id_t>& line_ids = parset(p).line_ids();

	    for_all_paragraph_lines(lid, line_ids)
	    {
	      line_id_t l = line_ids(lid);
	      lines(l).accept(*this);
	    }

	    mln::draw::box(output, parset(p).bbox(), literal::blue);
	  }
	}


	template <typename L>
	void
	full_img_visitor::visit(const line_info<L>& line) const
	{
	  mln::draw::box(output, line.bbox(), literal::red);
	}

#endif // MLN_INCLUDE_ONLY

      } // end of namespace scribo::io::img::internal

    } // end of namespace scribo::io::img

  } // end of namespace scribo::io

} // end of namespace scribo

#endif // SCRIBO_IO_IMG_INTERNAL_FULL_IMG_VISITOR_HH
