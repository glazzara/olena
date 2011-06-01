// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_IO_XML_SAVE_HH
# define SCRIBO_IO_XML_SAVE_HH

/// \file
///
/// \brief Save document information as XML.

# include <libgen.h>
# include <fstream>
# include <sstream>

# include <map>

# include <scribo/core/document.hh>

# ifdef HAVE_QT
#  include <scribo/io/xml/internal/full_xml_visitor.hh>
# endif // ! HAVE_QT

# include <scribo/io/xml/internal/extended_page_xml_visitor.hh>
# include <scribo/io/xml/internal/page_xml_visitor.hh>


namespace scribo
{

  namespace io
  {

    namespace xml
    {

      /*! \brief Supported XML formats

	Page : PRima PAGE format. Used in ICDAR 2009.

	PageExtended : Enriched PRima PAGE format with scribo data.

	Full : Enriched PRima PAGE format with scribo data. This
	       format can be reloaded in Scribo.
       */
      enum Format
      {
	Page,
	PageExtended,

# ifdef HAVE_QT
	Full
# endif // ! HAVE_QT

	//Hocr
      };


      /*! \brief Save document information as XML.

      */
      template <typename L>
      void
      save(const document<L>& doc, const std::string& output_name,
	   Format format);


# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

	template <typename L>
	void save_page(const document<L>& doc, std::ofstream& output)
	{
	  scribo::io::xml::internal::page_xml_visitor<L> f(output);
	  doc.accept(f);
	}

	template <typename L>
	void save_page_extended(const document<L>& doc, std::ofstream& output)
	{
	  scribo::io::xml::internal::extended_page_xml_visitor f(output);
	  doc.accept(f);
	}

# ifdef HAVE_QT
	template <typename L>
	void save_full(const document<L>& doc, std::ofstream& output)
	{
	  scribo::io::xml::internal::full_xml_visitor f(output);
	  doc.accept(f);
	}
# endif // ! HAVE_QT

      } // end of namespace scribo::io::xml::internal



      // FACADE

      template <typename L>
      void
      save(const document<L>& doc,
	   const std::string& output_name,
	   Format format)
      {
	trace::entering("scribo::io::xml::save");

	mln_precondition(doc.is_open());

	// Open file
	std::ofstream output(output_name.c_str());
	if (! output)
	{
	  std::cerr << "scribo::io::xml::save - ERROR: cannot open file '"
		    << doc.filename() << "'!";
	  return;
	}

	// Choose saving method.
	switch (format)
	{
	  case Page:
	    internal::save_page(doc, output);
	    break;

	  case PageExtended:
	    internal::save_page_extended(doc, output);
	    break;

# ifdef HAVE_QT
	  case Full:
	    internal::save_full(doc, output);
	    break;
# endif // ! HAVE_QT

	  default:
	    trace::warning("scribo::io::xml::save - Invalid XML format! Skip saving...");
	}

	output.close();
	trace::exiting("scribo::io::xml::save");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::io::xml

  } // end of namespace scribo::io

} // end of namespace scribo


#endif // ! SCRIBO_IO_XML_SAVE_HH
