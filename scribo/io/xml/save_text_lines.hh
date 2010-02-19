// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_IO_XML_SAVE_TEXT_LINES_HH
# define SCRIBO_IO_XML_SAVE_TEXT_LINES_HH

/// \file
///
/// \brief Save text line information as XML.

#include <fstream>
#include <sstream>


namespace scribo
{

  namespace io
  {

    namespace xml
    {

      /*! \brief Save text line information as XML.

	We use a XML Schema part of the PAGE (Page Analysis and Ground
	truth Elements) image representation framework.

	This schema was used in the Page Segmentation COMPetition
	(PSCOMP) for ICDAR 2009.

	Its XSD file is located here:
	http://schema.primaresearch.org/PAGE/gts/pagecontent/2009-03-16/pagecontent.xsd

       */
      template <typename L>
      void
      save_text_lines(const std::string& input_name,
		      const line_set<L>& lines,
		      const std::string& output_name);


# ifndef MLN_INCLUDE_ONLY


      template <typename L>
      void
      save_text_lines(const std::string& input_name,
		      const line_set<L>& lines,
		      const std::string& output_name)
      {
	trace::entering("scribo::io::xml:save_text_lines");

	std::ofstream file(output_name.c_str());
	if (! file)
	{
	  std::cerr << "error: cannot open file '" << input_name << "'!";
	  abort();
	}

	file << "<?xml version=\"1.0\"?>" << std::endl;
	file << "<pcGts xmlns=\"http://schema.primaresearch.org/PAGE/gts/pagecontent/2009-03-16\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://schema.primaresearch.org/PAGE/gts/pagecontent/2009-03-16 http://schema.primaresearch.org/PAGE/gts/pagecontent/2009-03-16/pagecontent.xsd\" pcGtsId=\"" << input_name << "\">" << std::endl;


	file << "  <pcMetadata>" << std::endl;
	file << "    <pcCreator>LRDE</pcCreator>" << std::endl;
	file << "    <pcCreated/>" << std::endl;
	file << "    <pcLastChange/>" << std::endl;
	file << "    <pcComments/>" << std::endl;
	file << "  </pcMetadata>" << std::endl;

	file << "  <page image_filename=\"" << input_name
	     << "\" image_width=\"" << lines.component_set_().labeled_image().ncols()
	     << "\" image_height=\"" << lines.component_set_().labeled_image().nrows()
	     << "\">" << std::endl;

	for_all_lines(l, lines)
	{
	  file << "    <text_region id=\"" << lines(l).id()
	       << "\" txt_orientation=\"0.000\" "
	       << "txt_reading_orientation=\"0.000\" "
	       << "txt_reading_direction=\"Left_To_Right\" "
	       << "txt_reverse_video=\"No\" "
	       << "txt_indented=\"No\">"
	       << std::endl;

	  file << "      <coords>" << std::endl
	       << "        <point x=\"" << lines(l).bbox().pmin().row()
	       << "\" y=\"" << lines(l).bbox().pmin().col() << "\"/>"
	       << std::endl
	       << "        <point x=\"" << lines(l).bbox().pmin().row()
	       << "\" y=\"" << lines(l).bbox().pmax().col() << "\"/>"
	       << std::endl
	       << "        <point x=\"" << lines(l).bbox().pmax().row()
	       << "\" y=\"" << lines(l).bbox().pmin().col() << "\"/>"
	       << std::endl
	       << "        <point x=\"" << lines(l).bbox().pmax().row()
	       << "\" y=\"" << lines(l).bbox().pmax().col() << "\"/>"
	       << std::endl
	       << "      </coords>" << std::endl;
	}

	file << "    </text_region>" << std::endl;
	file << "  </page>" << std::endl;
	file << "</pcGts>" << std::endl;



	trace::exiting("scribo::io::xml::save_text_lines");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::io::xml

  } // end of namespace scribo::io

} // end of namespace scribo


#endif // ! SCRIBO_IO_XML_SAVE_TEXT_LINES_HH
