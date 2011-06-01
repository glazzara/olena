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

# include <scribo/core/internal/doc_serializer.hh>
# include <scribo/convert/to_base64.hh>

# include <scribo/util/component_precise_outline.hh>

# include <scribo/io/xml/internal/print_box_coords.hh>
# include <scribo/io/xml/internal/print_page_preambule.hh>
# include <scribo/io/xml/internal/compute_text_colour.hh>
# include <scribo/text/paragraphs_closing.hh>


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

	  This schema was used in the Historical Document Layout
	  Analysis COMPetition (HDLAC) for ICDAR 2011.

	  Its XSD file is located here:
	  http://schema.primaresearch.org/PAGE/gts/pagecontent/2010-03-19/pagecontent.xsd
	*/
	template <typename L>
	class page_xml_visitor : public doc_serializer<page_xml_visitor<L> >
	{
	public:
	  // Constructor
	  page_xml_visitor<L>(std::ofstream& out);

	  // Visit overloads
	  void visit(const document<L>& doc) const;

	  void visit(const component_set<L>& comp_set) const;

	  void visit(const component_info<L>& info) const;

	  void visit(const paragraph_set<L>& parset) const;

	private: // Attributes
	  std::ofstream& output;
	  mutable int base_vertical_line_id_;

	  mutable L lbl_;
	};



# ifndef MLN_INCLUDE_ONLY


	template <typename L>
	page_xml_visitor<L>::page_xml_visitor(std::ofstream& out)
	  : output(out)
	{
	}



	/// Document
	//
	template <typename L>
	void
	page_xml_visitor<L>::visit(const document<L>& doc) const
	{
	  // Make sure there are no duplicate ids for line separators.
	  // Vertical and horizontal lines are indexed separately from
	  // 0, so vertical and horizontal lines with the same id
	  // exist.
	  base_vertical_line_id_ = doc.hline_seps_comps().nelements();

	  // Preambule
	  print_PAGE_preambule(output, doc, true);

	  // Text
	  if (doc.has_text())
	    doc.paragraphs().accept(*this);

	  // Page elements (Pictures, ...)
	  if (doc.has_elements())
	  {
	    doc.elements().accept(*this);
	  }

	  // line seraparators
	  if (doc.has_vline_seps())
	    doc.vline_seps_comps().accept(*this);
	  if (doc.has_hline_seps())
	    doc.hline_seps_comps().accept(*this);

	  output << "  </Page>" << std::endl;
	  output << "</PcGts>" << std::endl;
	}


	/// Component Set
	//
	template <typename L>
	void
	page_xml_visitor<L>::visit(const component_set<L>& comp_set) const
	{
	  lbl_ = comp_set.labeled_image();
	  for_all_comps(c, comp_set)
	    if (comp_set(c).is_valid())
	      comp_set(c).accept(*this);
	}


	/// Component_info
	//
	template <typename L>
	void
	page_xml_visitor<L>::visit(const component_info<L>& info) const
	{
	  // Getting component outline
	  scribo::def::lbl_type id = (scribo::def::lbl_type)info.id().to_equiv();
	  //const L& lbl = info.holder().labeled_image();
	  p_array<point2d>
	    par = scribo::util::component_precise_outline(lbl_ | info.bbox(), id);

	  switch (info.type())
	  {
	    case component::VerticalLineSeparator:
	    {
	      output << "    <SeparatorRegion id=\"sr" << info.id() + base_vertical_line_id_
		     << "\" orientation=\"0.000000\" "
		     << " colour=\"black\">" << std::endl;

	      internal::print_image_coords(output, par, "      ");

	      output << "    </SeparatorRegion>" << std::endl;
	      break;
	    }

	    case component::HorizontalLineSeparator:
	    {
	      output << "    <SeparatorRegion id=\"sr" << info.id()
		     << "\" orientation=\"0.000000\" "
		     << " colour=\"black\">" << std::endl;

	      internal::print_image_coords(output, par, "      ");

	      output << "    </SeparatorRegion>" << std::endl;
	      break;
	    }

	    case component::DropCapital:
	    {
	      output << "    <TextRegion id=\"r" << id << "\" "
		     << " Type=\"Drop_Capital\">"
		     << std::endl;

	      internal::print_image_coords(output, par, "      ");

	      output << "    </TextRegion>" << std::endl;
	      break;
	    }

	    default:
	    case component::Image:
	    {
	      output << "    <ImageRegion id=\"ir" << info.id()
		     << "\" colourDepth=\"colour\""
		     << " orientation=\"0.000000\" "
		     << " embText=\"false\" "
		     << " bgColour=\"white\">" << std::endl;

	      internal::print_image_coords(output, par, "      ");

	      output << "    </ImageRegion>" << std::endl;
	      break;
	    }
	  }
	}


	/// Paragraph Set
	//
	template <typename L>
	void
	page_xml_visitor<L>::visit(const paragraph_set<L>& parset) const
	{
	  const line_set<L>& lines = parset.lines();

	  // Prepare paragraph outlines.
	  L par_clo = text::paragraphs_closing(parset);

	  for_all_paragraphs(p, parset)
	    if (parset(p).is_valid())
	    {
	      p_array<mln_site(L)> par = scribo::util::component_precise_outline(par_clo
										 | parset(p).bbox(), p);

	      const mln::util::array<line_id_t>& line_ids = parset(p).line_ids();

	      // FIXME: compute that information on the whole paragraph
	      // and use them here.
	      line_id_t fid = line_ids(0);
	      output << "    <TextRegion id=\"r" << p
		     << "\" orientation=\"" << lines(fid).orientation()
		     << "\" readingOrientation=\"" << lines(fid).reading_orientation()
		     << "\" readingDirection=\"" << lines(fid).reading_direction()
		     << "\" type=\"" << ((lines(fid).type() == line::Text) ? "paragraph" : line::type2str(lines(fid).type()))
		     << "\" reverseVideo=\"" << (lines(fid).reverse_video() ? "true" : "false")
		     << "\" indented=\"" << (lines(fid).indented() ? "true" : "false")
		     << "\" kerning=\"" << lines(fid).char_space()
		     << "\" textColour=\"" << compute_text_colour(lines(fid).color())
//		   << "\" bgColour=\"" << compute_text_color(lines(fid).bgcolor())
//		   << "\" fontSize=\"" << compute_text_color(lines(fid).x_height())
//		   << "\" leading=\"" << compute_text_color(lines(fid).leading())
		     << "\">"
		     << std::endl;

	      // Add support for text recognition
	      // <TextEquiv>
	      //    <PlainText></PlainText>
	      //    <Unicode></Unicode>
	      //    </TextEquiv>

	      internal::print_image_coords(output, par, "      ");

	      output << "    </TextRegion>" << std::endl;
	    }
	}


#endif // MLN_INCLUDE_ONLY

      } // end of namespace scribo::io::xml::internal

    } // end of namespace scribo::io::xml

  } // end of namespace scribo::io

} // end of namespace scribo

#endif // SCRIBO_IO_XML_INTERNAL_PAGE_XML_VISITOR_HH
