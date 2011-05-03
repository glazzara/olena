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

#ifndef SCRIBO_IO_XML_INTERNAL_SAVE_IMAGE_TO_XML_HH
# define SCRIBO_IO_XML_INTERNAL_SAVE_IMAGE_TO_XML_HH

/// \file
///
/// Save an image as XML data.

# include <iostream>

# include <QtXml>

# include <mln/core/concept/image.hh>
# include <mln/border/resize.hh>


// Compression level 0-9. 9 is the best but is slow.
// 5 seems to be a good compromise.
# define COMPRESSION_LEVEL 5

namespace scribo
{

  namespace io
  {

    namespace xml
    {

      namespace internal
      {
	using namespace mln;

	template <typename I>
	void
	save_image_to_xml(std::ostream& output, const Image<I>& ima,
			  const char *qname);

# ifndef MLN_INCLUDE_ONLY

	template <typename I>
	void
	save_image_to_xml(std::ostream& output, const Image<I>& ima_,
			  const char *qname)
	{
	  trace::entering("scribo::io::xml::internal::save_image");

	  mln_precondition(exact(ima_).is_valid());
	  const I& ima = exact(ima_);

	  output << "<" << qname
		 << " height=\"" << ima.domain().height()
		 << "\" width=\"" << ima.domain().width() << "\">"
		 << "<![CDATA[";

	  mln::border::resize(ima, 0);
	  QByteArray
	    seps64 = QByteArray::fromRawData((const char *)ima.buffer(),
					     ima.nelements() * sizeof(mln_value(I)));
	  seps64 = qCompress(seps64, COMPRESSION_LEVEL);
	  seps64 = seps64.toBase64();

	  output.write(seps64.data(), seps64.size());

	  output <<  "]]></" << qname << ">" << std::endl;

	  trace::exiting("scribo::io::xml::internal::save_image");
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::io::xml::internal

    } // end of namespace scribo::io::xml

  } // end of namespace scribo::io

} // end of namespace scribo

# undef COMPRESSION_LEVEL

#endif // ! SCRIBO_IO_XML_INTERNAL_SAVE_IMAGE_TO_XML_HH
