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

#ifndef SCRIBO_IO_XML_INTERNAL_EXTENDED_PAGE_XML_VISITOR_HH
# define SCRIBO_IO_XML_INTERNAL_EXTENDED_PAGE_XML_VISITOR_HH

/// \file
///
/// Extended XML PAGE format serializer Visitor.

# include <fstream>

# include <mln/morpho/elementary/gradient_internal.hh>
# include <mln/pw/all.hh>
# include <mln/core/image/dmorph/image_if.hh>

# include <scribo/core/def/lbl_type.hh>

# include <scribo/core/internal/doc_serializer.hh>
# include <scribo/core/document.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/paragraph_set.hh>
# include <scribo/core/object_groups.hh>
# include <scribo/core/object_links.hh>
# include <scribo/core/line_links.hh>
# include <scribo/core/line_info.hh>

# include <scribo/io/xml/internal/print_box_coords.hh>
# include <scribo/io/xml/internal/print_image_coords.hh>
# include <scribo/io/xml/internal/print_page_preambule.hh>


namespace scribo
{

  namespace io
  {

    namespace xml
    {

      namespace internal
      {


	class extended_page_xml_visitor : public doc_serializer<extended_page_xml_visitor>
	{
	public:
	  // Constructor
	  extended_page_xml_visitor(std::ofstream& out);

	  // Visit overloads
	  template <typename L>
	  void visit(const document<L>& doc) const;

	  template <typename L>
	  void visit(const component_set<L>& comp_set) const;

	  void visit(const component_info& info) const;

	  template <typename L>
	  void visit(const paragraph_set<L>& parset) const;

	  template <typename L>
	  void visit(const line_info<L>& line) const;

	private: // Attributes
	  std::ofstream& output;
	  mutable image2d<scribo::def::lbl_type> elt_edge;
	};



# ifndef MLN_INCLUDE_ONLY


	inline
	extended_page_xml_visitor::extended_page_xml_visitor(std::ofstream& out)
	  : output(out)
	{
	}



	/// Document
	//
	template <typename L>
	void
	extended_page_xml_visitor::visit(const document<L>& doc) const
 	{
	  // Preambule
	  print_PAGE_preambule(output, doc, false);

	  // Text
	  if (doc.has_text())
	    doc.paragraphs().accept(*this);


	  // Page elements (Pictures, ...)
	  if (doc.has_elements())
	  {
	    // Prepare element edges
	    elt_edge = morpho::elementary::gradient_internal(doc.elements().labeled_image(), c4());

	    doc.elements().accept(*this);
	  }

	  // line seraparators
	  if (doc.has_vline_seps())
	    doc.vline_seps_comps().accept(*this);
	  if (doc.has_hline_seps())
	    doc.hline_seps_comps().accept(*this);

	  // Whitespace seraparators
	  if (doc.has_whitespace_seps())
	    doc.whitespace_seps_comps().accept(*this);

	  output << "  </page>" << std::endl;
	  output << "</pcGts>" << std::endl;

	}

	/// Component Set
	//
	template <typename L>
	void
	extended_page_xml_visitor::visit(const component_set<L>& comp_set) const
	{
	  for_all_comps(c, comp_set)
	    if (comp_set(c).is_valid())
	      comp_set(c).accept(*this);
	}


	/// Component_info
	//
	inline
	void
	extended_page_xml_visitor::visit(const component_info& info) const
	{
	  switch (info.type())
	  {
	    case component::WhitespaceSeparator:
	    {
	      output << "    <whitespace_separator_region id=\"wss"
		     << info.id()
		     << "\">" << std::endl;

	      internal::print_box_coords(output, info.bbox(), "      ");

	      output << "    </whitespace_separator_region>" << std::endl;
	      break;
	    }

	    case component::LineSeparator:
	    {
	      output << "    <separator_region id=\"sr" << info.id()
		     << "\" sep_orientation=\"0.000000\" "
		     << " sep_colour=\"Black\" "
		     << " sep_bgcolour=\"White\">" << std::endl;

	      internal::print_box_coords(output, info.bbox(), "      ");

	      output << "    </separator_region>" << std::endl;
	      break;
	    }


	    default:
	    case component::Image:
	    {
	      output << "    <image_region id=\"ir" << info.id()
		     << "\" img_colour_type=\"24_Bit_Colour\""
		     << " img_orientation=\"0.000000\" "
		     << " img_emb_text=\"No\" "
		     << " img_bgcolour=\"White\">" << std::endl;

	      internal::print_image_coords(output,
					   ((elt_edge | info.bbox())
					    | (pw::value(elt_edge) == pw::cst((scribo::def::lbl_type)info.id().to_equiv()))).domain(),
					   "      ");

	      output << "    </image_region>" << std::endl;
	      break;
	    }
	  }
	}


	/// Paragraph Set
	//
	template <typename L>
	void
	extended_page_xml_visitor::visit(const paragraph_set<L>& parset) const
	{
	  const line_set<L>& lines = parset.lines();

	  for_all_paragraphs(p, parset)
	  {
	    const mln::util::array<line_id_t>& line_ids = parset(p).line_ids();

	    // FIXME: compute that information on the whole paragraph
	    // and use them here.
	    line_id_t fid = line_ids(0);
	    output << "    <text_region id=\"" << p
		   << "\" txt_orientation=\"" << lines(fid).orientation()
		   << "\" txt_reading_orientation=\"" << lines(fid).reading_orientation()
		   << "\" txt_reading_direction=\"" << lines(fid).reading_direction()
		   << "\" txt_text_type=\"" << lines(fid).type()
		   << "\" txt_reverse_video=\"" << (lines(fid).reverse_video() ? "true" : "false")
		   << "\" txt_indented=\"" << (lines(fid).indented() ? "true" : "false")
		   << "\" kerning=\"" << lines(fid).char_space();

	    // EXTENSIONS - Not officially supported
	    // FIXME: add boldness?
	    output << "\" color=\"" << parset(p).color()
		   << "\" color_reliability=\"" << parset(p).color_reliability()
		   << "\" baseline=\"" << lines(fid).baseline()
		   << "\" meanline=\"" << lines(fid).meanline()
		   << "\" x_height=\"" << lines(fid).x_height()
		   << "\" d_height=\"" << lines(fid).d_height()
		   << "\" a_height=\"" << lines(fid).a_height()
		   << "\" char_width=\"" << lines(fid).char_width();
	    // End of EXTENSIONS
	    output << "\">"
		   << std::endl;

	    internal::print_box_coords(output, parset(p).bbox(), "      ");

	    // EXTENSIONS - Not officially supported
	    for_all_paragraph_lines(lid, line_ids)
	    {
	      line_id_t l = line_ids(lid);
	      lines(l).accept(*this);
	    }
	    // End of EXTENSIONS

	    output << "    </text_region>" << std::endl;
	  }
	}


	template <typename L>
	void
	extended_page_xml_visitor::visit(const line_info<L>& line) const
	{
	  if (line.has_text())
	  {
	    output << "        <line text=\"" << line.html_text() << "\" ";
	  }
	  else
	    output << "        <line ";

	  output << "id=\"" << line.id()
		 << "\" boldness=\"" << line.boldness()
		 << "\" boldness_reliability=\"" << line.boldness_reliability()
		 << "\" color=\"" << line.color()
		 << "\" color_reliability=\"" << line.color_reliability()
		 << "\" txt_orientation=\"" << line.orientation()
		 << "\" txt_reading_orientation=\"" << line.reading_orientation()
		 << "\" txt_reading_direction=\"" << line.reading_direction()
		 << "\" txt_text_type=\"" << line.type()
		 << "\" txt_reverse_video=\"" << (line.reverse_video() ? "true" : "false")
		 << "\" txt_indented=\"" << (line.indented() ? "true" : "false")
		 << "\" kerning=\"" << line.char_space()
		 << "\" baseline=\"" << line.baseline()
		 << "\" meanline=\"" << line.meanline()
		 << "\" x_height=\"" << line.x_height()
		 << "\" d_height=\"" << line.d_height()
		 << "\" a_height=\"" << line.a_height()
		 << "\" char_width=\"" << line.char_width()
		 << "\">" << std::endl;

	  internal::print_box_coords(output, line.bbox(), "          ");

	  output << "        </line>" << std::endl;
	}

#endif // MLN_INCLUDE_ONLY

      } // end of namespace scribo::io::xml::internal

    } // end of namespace scribo::io::xml

  } // end of namespace scribo::io

} // end of namespace scribo

#endif // SCRIBO_IO_XML_INTERNAL_EXTENDED_PAGE_XML_VISITOR_HH
