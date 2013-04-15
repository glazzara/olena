// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_IO_PDF_GET_HEADER_HH
# define MLN_IO_PDF_GET_HEADER_HH

/// \file
///
/// Load PDF header.

# include <iostream>
# include <fstream>

#include <poppler/cpp/poppler-document.h>

# include <mln/core/concept/image.hh>
# include <mln/core/routine/initialize.hh>
# include <mln/core/box_runstart_piter.hh>
# include <mln/core/pixel.hh>
# include <mln/data/memcpy_.hh>
# include <mln/util/array.hh>

namespace mln
{

  namespace io
  {

    namespace pdf
    {

      /*!
	\brief Store pdf file header.
	\ingroup iopdf
      */
      struct pdf_header
      {
	pdf_header(const poppler::document *doc);

	int page_count;
	int pdf_version_major;
	int pdf_version_minor;
	bool is_encrypted;
	bool is_linearized;
	bool is_locked;
	std::string metadata;
      };


      /*!
	\brief Retrieve header in a pdf file.
	\ingroup iopdf
      */
      pdf_header get_header(const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

      // Begin of pdf_header implementation.

      pdf_header::pdf_header(const poppler::document *doc)
      {
	page_count = doc->pages();
	doc->get_pdf_version(&pdf_version_major, 0);
	doc->get_pdf_version(0, &pdf_version_minor);
	is_encrypted = doc->is_encrypted();
	is_linearized = doc->is_linearized();
	is_locked = doc->is_locked();
	metadata = doc->metadata().to_latin1();
      }

      // End of pdf_header implementation.



      inline
      pdf_header get_header(const std::string& filename)
      {
	mln_trace("mln::io::pdf::get_header");

	// Load document
	poppler::document *pdf = poppler::document::load_from_file(filename);
	if (pdf == 0)
	{
	  std::cerr << "Error: Cannot load PDF " << filename << std::endl;
	  abort();
	}

	// Initialize structure with pdf information.
	pdf_header header(pdf);

	// Clear pdf document.
	delete pdf;

	return header;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::pdf

  } // end of namespace mln::io

} // end of namespace mln

#endif // ! MLN_IO_PDF_GET_HEADER_HH

