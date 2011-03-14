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

#ifndef SCRIBO_IO_XML_INTERNAL_PAGE_XML_VISITOR_HH
# define SCRIBO_IO_XML_INTERNAL_PAGE_XML_VISITOR_HH

/// \file
///
/// PAGE format XML serializer Visitor.

# include <fstream>

# include <scribo/core/internal/doc_xml_serializer.hh>
# include <scribo/convert/to_base64.hh>

# include <scribo/io/xml/internal/print_box_coords.hh>
# include <scribo/io/xml/internal/print_page_preambule.hh>


namespace scribo
{

  namespace io
  {

    namespace xml
    {

      namespace internal
      {

	/*! \brief Save document information as XML.

	  We use a XML Schema part of the PAGE (Page Analysis and Ground
	  truth Elements) image representation framework.

	  This schema was used in the Page Segmentation COMPetition
	  (PSCOMP) for ICDAR 2009.

	  Its XSD file is located here:
	  http://schema.primaresearch.org/PAGE/gts/pagecontent/2009-03-16/pagecontent.xsd

	*/
	class page_xml_visitor : public doc_xml_serializer<page_xml_visitor>
	{
	public:
	  // Constructor
	  page_xml_visitor(std::ofstream& out);

	  // Visit overloads
	  template <typename L>
	  void visit(const document<L>& doc) const;

	  template <typename L>
	  void visit(const component_set<L>& comp_set) const;

	  void visit(const component_info& info) const;

	  template <typename L>
	  void visit(const paragraph_set<L>& parset) const;

	private: // Attributes
	  std::ofstream& output;
	};



# ifndef MLN_INCLUDE_ONLY


	inline
	page_xml_visitor::page_xml_visitor(std::ofstream& out)
	  : output(out)
	{
	}



	/// Document
	//
	template <typename L>
	void
	page_xml_visitor::visit(const document<L>& doc) const
	{
	  // Preambule
	  print_PAGE_preambule(output, doc, true);

	  // Text
	  if (doc.has_text())
	    doc.paragraphs().accept(*this);

	  // Page elements (Pictures, ...)
	  if (doc.has_elements())
	    doc.elements().accept(*this);

	  // line seraparators
	  if (doc.has_vline_seps())
	    doc.vline_seps_comps().accept(*this);
	  if (doc.has_hline_seps())
	    doc.hline_seps_comps().accept(*this);

	  output << "  </page>" << std::endl;
	  output << "</pcGts>" << std::endl;
	}


	/// Component Set
	//
	template <typename L>
	void
	page_xml_visitor::visit(const component_set<L>& comp_set) const
	{
	  for_all_comps(c, comp_set)
	    if (comp_set(c).is_valid())
	      comp_set(c).accept(*this);
	}


	/// Component_info
	//
	inline
	void
	page_xml_visitor::visit(const component_info& info) const
	{
	  switch (info.type())
	  {
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

	      internal::print_box_coords(output, info.bbox(), "      ");

	      output << "    </image_region>" << std::endl;
	      break;
	    }
	  }
	}


	/// Paragraph Set
	//
	template <typename L>
	void
	page_xml_visitor::visit(const paragraph_set<L>& parset) const
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
		   << "\" kerning=\"" << lines(fid).char_space()
		   << "\">"
		   << std::endl;

	    internal::print_box_coords(output, parset(p).bbox(), "      ");

	    output << "    </text_region>" << std::endl;
	  }
	}


#endif // MLN_INCLUDE_ONLY

      } // end of namespace scribo::io::xml::internal

    } // end of namespace scribo::io::xml

  } // end of namespace scribo::io

} // end of namespace scribo

#endif // SCRIBO_IO_XML_INTERNAL_PAGE_XML_VISITOR_HH
