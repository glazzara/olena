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
# include <scribo/io/xml/internal/compute_text_colour.hh>

# include <scribo/text/paragraphs_closing.hh>

# include <scribo/util/component_precise_outline.hh>
# include <scribo/util/color_to_hex.hh>


namespace scribo
{

  namespace io
  {

    namespace xml
    {

      namespace internal
      {


	template <typename L>
	class extended_page_xml_visitor
	  : public doc_serializer<extended_page_xml_visitor<L> >
	{
	public:
	  // Constructor
	  extended_page_xml_visitor(std::ofstream& out);

	  // Visit overloads
	  void visit(const document<L>& doc) const;

	  void visit(const component_set<L>& comp_set) const;

	  void visit(const component_info<L>& info) const;

	  void visit(const paragraph_set<L>& parset) const;

	  void visit(const line_info<L>& line) const;

	private: // Attributes
	  std::ofstream& output;
	  mutable L lbl_;
	};



# ifndef MLN_INCLUDE_ONLY


	template <typename L>
	extended_page_xml_visitor<L>::extended_page_xml_visitor(std::ofstream& out)
	  : output(out)
	{
	}



	/// Document
	//
	template <typename L>
	void
	extended_page_xml_visitor<L>::visit(const document<L>& doc) const
 	{
	  // Preambule
	  print_PAGE_preambule(output, doc, false);

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

	  // Whitespace seraparators
	  if (doc.has_whitespace_seps())
	    doc.whitespace_seps_comps().accept(*this);

	  output << "  </Page>" << std::endl;
	  output << "</PcGts>" << std::endl;

	}

	/// Component Set
	//
	template <typename L>
	void
	extended_page_xml_visitor<L>::visit(const component_set<L>& comp_set) const
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
	extended_page_xml_visitor<L>::visit(const component_info<L>& info) const
	{
	  // Getting component outline
	  scribo::def::lbl_type id = (scribo::def::lbl_type)info.id().to_equiv();
	  //const L& lbl = info.holder().labeled_image();
	  p_array<point2d>
	    par = scribo::util::component_precise_outline(lbl_ | info.bbox(), id);

	  switch (info.type())
	  {
	    case component::WhitespaceSeparator:
	    {
	      output << "    <WhitespaceSeparatorRegion id=\"wss"
		     << info.id()
		     << "\""
		     << " x_min=\"" << info.bbox().pmin().col() << "\""
		     << " y_min=\"" << info.bbox().pmin().row() << "\""
		     << " x_max=\"" << info.bbox().pmax().col() << "\""
		     << " y_max=\"" << info.bbox().pmax().row() << "\""
		     << ">" << std::endl;

	      internal::print_image_coords(output, par, "      ");

	      output << "    </WhitespaceSeparatorRegion>" << std::endl;
	      break;
	    }

	    case component::VerticalLineSeparator:
	    {
	      output << "    <VerticalSeparatorRegion id=\"vlsr" << info.id()
		     << "\" orientation=\"0.000000\" "
		     << " colour=\"Black\" "
		     << " bgColour=\"White\""
		     << " x_min=\"" << info.bbox().pmin().col() << "\""
		     << " y_min=\"" << info.bbox().pmin().row() << "\""
		     << " x_max=\"" << info.bbox().pmax().col() << "\""
		     << " y_max=\"" << info.bbox().pmax().row() << "\""
		     << ">" << std::endl;

	      internal::print_image_coords(output, par, "      ");

	      output << "    </VerticalSeparatorRegion>" << std::endl;
	      break;
	    }

	    case component::HorizontalLineSeparator:
	    {
	      output << "    <HorizontalSeparatorRegion id=\"hlsr" << info.id()
		     << "\" orientation=\"0.000000\" "
		     << " colour=\"Black\" "
		     << " bgColour=\"White\""
		     << " x_min=\"" << info.bbox().pmin().col() << "\""
		     << " y_min=\"" << info.bbox().pmin().row() << "\""
		     << " x_max=\"" << info.bbox().pmax().col() << "\""
		     << " y_max=\"" << info.bbox().pmax().row() << "\""
		     << ">" << std::endl;

	      internal::print_image_coords(output, par, "      ");

	      output << "    </HorizontalSeparatorRegion>" << std::endl;
	      break;
	    }


	    default:
	    case component::Image:
	    {
	      output << "    <ImageRegion id=\"ir" << info.id()
		     << "\" colourDepth=\"colour\""
		     << " orientation=\"0.000000\" "
		     << " embText=\"No\" "
		     << " bgColour=\"White\""
		     << " x_min=\"" << info.bbox().pmin().col() << "\""
		     << " y_min=\"" << info.bbox().pmin().row() << "\""
		     << " x_max=\"" << info.bbox().pmax().col() << "\""
		     << " y_max=\"" << info.bbox().pmax().row() << "\""
		     << ">" << std::endl;

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
	extended_page_xml_visitor<L>::visit(const paragraph_set<L>& parset) const
	{
	  const line_set<L>& lines = parset.lines();

	  // Prepare paragraph outlines.
	  L par_clo = text::paragraphs_closing(parset);

	  for_all_paragraphs(p, parset)
	    if (parset(p).is_valid())
	    {
	      const box2d& b = parset(p).bbox();
	      p_array<mln_site(L)>
		par = scribo::util::component_precise_outline(par_clo | b, p);

	      const mln::util::array<line_id_t>& line_ids = parset(p).line_ids();

	      // FIXME: compute that information on the whole paragraph
	      // and use them here.
	      line_id_t fid = line_ids(0);
	      output << "    <TextRegion id=\"" << p
		     << "\" orientation=\"" << lines(fid).orientation()
		     << "\" readingOrientation=\"" << lines(fid).reading_orientation()
		     << "\" readingDirection=\"" << lines(fid).reading_direction()
		     << "\" type=\"" << lines(fid).type()
		     << "\" reverseVideo=\"" << (lines(fid).reverse_video() ? "true" : "false")
		     << "\" indented=\"" << (lines(fid).indented() ? "true" : "false")
		     << "\" textColour=\"" << internal::compute_text_colour(parset(p).color())
		     << "\" kerning=\"" << lines(fid).char_space();

	      // EXTENSIONS - Not officially supported
	      // FIXME: add boldness?
	      output << "\" color=\"" << scribo::util::color_to_hex(parset(p).color())
		     << "\" colorReliability=\"" << parset(p).color_reliability()
		     << "\" baseline=\"" << lines(fid).baseline()
		     << "\" meanline=\"" << lines(fid).meanline()
		     << "\" xHeight=\"" << lines(fid).x_height()
		     << "\" dHeight=\"" << lines(fid).d_height()
		     << "\" aHeight=\"" << lines(fid).a_height()
		     << "\" charWidth=\"" << lines(fid).char_width();
	      // End of EXTENSIONS
	      output << "\">"
		     << std::endl;

	      internal::print_image_coords(output, par, "      ");

	      // EXTENSIONS - Not officially supported
	      for_all_paragraph_lines(lid, line_ids)
	      {
		line_id_t l = line_ids(lid);
		lines(l).accept(*this);
	      }
	      // End of EXTENSIONS

	      output << "    </TextRegion>" << std::endl;
	    }
	}


	template <typename L>
	void
	extended_page_xml_visitor<L>::visit(const line_info<L>& line) const
	{
	  if (line.has_text())
	  {
	    output << "        <Line text=\"" << line.html_text() << "\" ";
	  }
	  else
	    output << "        <Line ";

	  output << "id=\"" << line.id()
		 << "\" boldness=\"" << line.boldness()
		 << "\" boldnessReliability=\"" << line.boldness_reliability()
		 << "\" color=\"" << scribo::util::color_to_hex(line.color())
		 << "\" colorReliability=\"" << line.color_reliability()
		 << "\" orientation=\"" << line.orientation()
		 << "\" readingOrientation=\"" << line.reading_orientation()
		 << "\" readingDirection=\"" << line.reading_direction()
		 << "\" type=\"" << line.type()
		 << "\" reverseVideo=\"" << (line.reverse_video() ? "true" : "false")
		 << "\" indented=\"" << (line.indented() ? "true" : "false")
		 << "\" textColour=\"" << internal::compute_text_colour(line.color())
		 << "\" kerning=\"" << line.char_space()
		 << "\" baseline=\"" << line.baseline()
		 << "\" meanline=\"" << line.meanline()
		 << "\" xHeight=\"" << line.x_height()
		 << "\" dHeight=\"" << line.d_height()
		 << "\" aHeight=\"" << line.a_height()
		 << "\" charWidth=\"" << line.char_width()
		 << "\">" << std::endl;

	  internal::print_box_coords(output, line.bbox(), "          ");

	  output << "        </Line>" << std::endl;
	}

#endif // MLN_INCLUDE_ONLY

      } // end of namespace scribo::io::xml::internal

    } // end of namespace scribo::io::xml

  } // end of namespace scribo::io

} // end of namespace scribo

#endif // SCRIBO_IO_XML_INTERNAL_EXTENDED_PAGE_XML_VISITOR_HH
