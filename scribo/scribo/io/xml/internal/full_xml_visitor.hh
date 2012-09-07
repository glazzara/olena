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

#ifndef SCRIBO_IO_XML_INTERNAL_FULL_XML_VISITOR_HH
# define SCRIBO_IO_XML_INTERNAL_FULL_XML_VISITOR_HH

/// \file
///
/// XML serializer Visitor.

# include <iomanip>
# include <fstream>

# include <QByteArray>

# include <scribo/core/internal/doc_serializer.hh>
# include <scribo/core/document.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/paragraph_set.hh>
# include <scribo/core/object_groups.hh>
# include <scribo/core/object_links.hh>
# include <scribo/core/line_links.hh>
# include <scribo/core/line_info.hh>

# include <scribo/io/xml/internal/save_image_to_xml.hh>
# include <scribo/io/xml/internal/print_box_coords.hh>
# include <scribo/io/xml/internal/print_page_preambule.hh>
# include <scribo/io/xml/internal/compute_text_colour.hh>

# include <scribo/util/color_to_hex.hh>

namespace scribo
{

  namespace io
  {

    namespace xml
    {

      namespace internal
      {


	class full_xml_visitor : public doc_serializer<full_xml_visitor>
	{
	public:
	  // Constructor
	  full_xml_visitor(std::ofstream& out);

	  // Visit overloads
	  template <typename L>
	  void visit(const document<L>& doc) const;

	  template <typename L>
	  void visit(const line_links<L>& llinks) const;

	  template <typename L>
	  void visit(const object_groups<L>& groups) const;

	  template <typename L>
	  void visit(const object_links<L>& links) const;

	  template <typename L>
	  void visit(const component_set<L>& comp_set) const;

	  template <typename L>
	  void visit(const component_info<L>& info) const;

	  template <typename L>
	  void visit(const paragraph_set<L>& parset) const;

	  template <typename L>
	  void visit(const line_info<L>& line) const;

	private: // Attributes
	  std::ofstream& output;
	};



# ifndef MLN_INCLUDE_ONLY


	inline
	full_xml_visitor::full_xml_visitor(std::ofstream& out)
	  : output(out)
	{
	  output << std::setiosflags(std::ios::fixed);
	  output.precision(26);
	}



	/// Document
	//
	template <typename L>
	void
	full_xml_visitor::visit(const document<L>& doc) const
	{
	  print_PAGE_preambule(output, doc, false);

	  // Text
	  if (doc.has_text())
	  {
	    const line_set<L>& lines = doc.lines();

	    // Save component/link/group information (Extension)
	    {
	      // Component set
	      lines.components().accept(*this);

	      // Object link
	      lines.links().accept(*this);

	      // Object group
	      lines.groups().accept(*this);
	    }
	    // End of EXTENSIONS

	    const paragraph_set<L>& parset = doc.paragraphs();

	    // Save paragraphs related information (Extension)
	    {
	      // General text information
	      output << "    <TextData nlines=\"" << lines.nelements() << "\" "
		   << " nparagraphs=\"" << parset.nelements() << "\" />" << std::endl;

	      // line_links
	      parset.links().accept(*this);
	    }

	    // Paragraph and lines
	    parset.accept(*this);
	  }


	  // Page elements (Pictures, ...)
	  if (doc.has_elements())
	  {
	    const component_set<L>& elts = doc.elements();

	    output << "     <Elements>" << std::endl;
	    elts.accept(*this);

	    for_all_comps(e, elts)
	      if (elts(e).is_valid())
		elts(e).accept(*this);

	    output << "     </Elements>" << std::endl;
	  }


	  // line seraparators
	  if (doc.has_hline_seps())
	  {
	    output << "     <HlinesSeparators>" << std::endl;
	    doc.hline_seps_comps().accept(*this);

	    for_all_comps(c, doc.hline_seps_comps())
	      doc.hline_seps_comps()(c).accept(*this);

	    save_image_to_xml(output, doc.hline_seps(),
			      "HlinesSeparatorsImage");

	    output << "     </HlinesSeparators>" << std::endl;
	  }
	  if (doc.has_vline_seps())
	  {
	    output << "     <VlinesSeparators>" << std::endl;
	    doc.vline_seps_comps().accept(*this);

	    for_all_comps(c, doc.vline_seps_comps())
	      doc.vline_seps_comps()(c).accept(*this);

	    save_image_to_xml(output, doc.vline_seps(),
			      "VlinesSeparatorsImage");

	    output << "     </VlinesSeparators>" << std::endl;
	  }


	  // Whitespace seraparators
	  if (doc.has_whitespace_seps())
	  {
	    const component_set<L>&
	      whitespace_seps_comps = doc.whitespace_seps_comps();

	    output << "     <WhitespacesDelimitors>" << std::endl;
	    whitespace_seps_comps.accept(*this);

	    for_all_comps(c, whitespace_seps_comps)
	      whitespace_seps_comps(c).accept(*this);

	    save_image_to_xml(output, doc.whitespace_seps(),
			      "WhitespacesDelimitorsImage");

	    output << "     </WhitespacesDelimitors>" << std::endl;
	  }

	  output << "  </Page>" << std::endl;
	  output << "</PcGts>" << std::endl;

	}


	/// Line Links
	//
	template <typename L>
	void
	full_xml_visitor::visit(const line_links<L>& llinks) const
	{
	  output << "      <LineLinks>" << std::endl;
	  for_all_links(l, llinks)
	  {
	    output << "      <LineLink"
		   << " from=\"" << l
		   << "\" to=\"" << llinks(l)
		   << "\"/>" << std::endl;
	  }
	  output << "      </LineLinks>" << std::endl;
	}


	/// Object Groups
        //
	template <typename L>
	void
	full_xml_visitor::visit(const object_groups<L>& groups) const
	{
	  output << "      <ObjectGroups ngroups=\"" << groups.nelements()
		 << "\">" << std::endl;

	  for_all_groups(g, groups)
	  {
	    output << "        <group id=\"" << groups(g).id()
		   << "\" valid=\"" << groups(g).is_valid()
		   << "\" pixelArea=\"" << groups(g).pixel_area()
		   << "\" pmin_x=\"" << groups(g).bbox().pmin().row()
		   << "\" pmin_y=\"" << groups(g).bbox().pmin().col()
		   << "\" pmax_x=\"" << groups(g).bbox().pmax().row()
		   << "\" pmax_y=\"" << groups(g).bbox().pmax().col()
		   << "\">" << std::endl;

	    for_all_elements(e, groups(g).component_ids())
	      output << "          <GroupMember comp_id=\""
		     << groups(g).component_ids()(e)
		     << "\"/>" << std::endl;

	    output << "        </Group>" << std::endl;
	  }
	  output << "      </ObjectGroups>" << std::endl;
	}


	/// Object Links
	//
	template <typename L>
	void
	full_xml_visitor::visit(const object_links<L>& links) const
	{
	  output << "      <ObjectLinks>" << std::endl;
	  for_all_links(l, links)
	  {
	    output << "      <Link"
		   << " from=\"" << l
		   << "\" to=\"" << links(l)
		   << "\"/>" << std::endl;
	  }
	  output << "      </ObjectLinks>" << std::endl;
	}


	/// Component Set
	//
	template <typename L>
	void
	full_xml_visitor::visit(const component_set<L>& comp_set) const
	{
	  output << "    <ComponentSet nelements=\"" << comp_set.nelements()
		 << "\">" << std::endl;
	  for_all_comps(c, comp_set)
	    if (comp_set(c).is_valid())
	    {
	      output << "      <ComponentInfo"
		     << " id=\"" << comp_set(c).id()
		     << "\" massCenter_x=\"" << comp_set(c).mass_center().col()
		     << "\" massCenter_y=\"" << comp_set(c).mass_center().row()
		     << "\" card=\"" <<  comp_set(c).card()
		     << "\" tag=\"" <<  comp_set(c).tag()
		     << "\" type=\"" <<  comp_set(c).type()
		     << "\" pmin_x=\"" << comp_set(c).bbox().pmin().col()
		     << "\" pmin_y=\"" << comp_set(c).bbox().pmin().row()
		     << "\" pmax_x=\"" << comp_set(c).bbox().pmax().col()
		     << "\" pmax_y=\"" << comp_set(c).bbox().pmax().row();

	      if (comp_set(c).has_features())
	      {
		output << "\">" << std::endl;

		output << "      <ComponentFeatures"
		       << " valid=\"" << comp_set(c).features().valid
		       << "\" color=\"" << comp_set(c).features().color
		       << "\" boldness=\"" << comp_set(c).features().boldness
		       << "\"/>" << std::endl;

		output << "    </ComponentInfo>" << std::endl;
	      }
	      else
		output << "\"/>" << std::endl;
	    }


	  // Save labeled image
	  {
	    const L& lbl = comp_set.labeled_image();
	    save_image_to_xml(output, lbl, "LabeledImage");
	  }

	  // Save separators image
	  if (comp_set.has_separators())
	  {
	    const mln_ch_value(L,bool)& seps = comp_set.separators();
	    save_image_to_xml(output, seps, "SeparatorsImage");
	  }

	  output << "</ComponentSet>" << std::endl;
	}


	/// Component_info
	//
	template <typename L>
	void
	full_xml_visitor::visit(const component_info<L>& info) const
	{
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

	      internal::print_box_coords(output, info.bbox(), "      ");

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

	      internal::print_box_coords(output, info.bbox(), "      ");

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

	      internal::print_box_coords(output, info.bbox(), "      ");

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

	      internal::print_box_coords(output, info.bbox(), "      ");

	      output << "    </ImageRegion>" << std::endl;
	      break;
	    }
	  }
	}

	/// Paragraph Set
	//
	template <typename L>
	void
	full_xml_visitor::visit(const paragraph_set<L>& parset) const
	{
	  const line_set<L>& lines = parset.lines();

	  for_all_paragraphs(p, parset)
	    if (parset(p).is_valid())
	    {
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
	      output << "\" baseline=\"" << lines(fid).baseline()
		     << "\" meanline=\"" << lines(fid).meanline()
		     << "\" xHeight=\"" << lines(fid).x_height()
		     << "\" dHeight=\"" << lines(fid).d_height()
		     << "\" aHeight=\"" << lines(fid).a_height()
		     << "\" charWidth=\"" << lines(fid).char_width()
		     << "\" color=\"" << scribo::util::color_to_hex(parset(p).color())
		     << "\" colorReliability=\"" << parset(p).color_reliability();
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

	      output << "    </TextRegion>" << std::endl;
	    }
	}


	template <typename L>
	void
	full_xml_visitor::visit(const line_info<L>& line) const
	{
	  if (line.has_text())
	  {
	    output << "        <Line text=\"" << line.html_text() << "\" ";
	  }
	  else
	    output << "        <Line " << std::endl;

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
		 << "\" textConfidence=\"" << line.text_confidence()
		 << "\">" << std::endl;

	  internal::print_box_coords(output, line.bbox(), "          ");

	  output << "          <CompidList>" << std::endl;

	  for_all_line_comps(c, line.component_ids())
	    output << "            <Compid value=\""
		   << line.component_ids()(c) << "\" />" << std::endl;

	  output << "          </CompidList>" << std::endl;

	  output << "        </Line>" << std::endl;
	}

#endif // MLN_INCLUDE_ONLY

      } // end of namespace scribo::io::xml::internal

    } // end of namespace scribo::io::xml

  } // end of namespace scribo::io

} // end of namespace scribo


#endif // SCRIBO_IO_XML_INTERNAL_FULL_XML_VISITOR_HH
