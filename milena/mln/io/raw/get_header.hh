// Copyright (C) 2010, 2012 EPITA Research and Development Laboratory
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

#ifndef MLN_IO_RAW_GET_HEADER_HH
# define MLN_IO_RAW_GET_HEADER_HH

/// \file
///
/// Load a raw image header.

# include <iostream>
# include <fstream>

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

    namespace raw
    {

      /*! \brief Store raw file header.
       *
       * \ingroup ioraw
       */
      struct raw_header
      {
	unsigned dim;
	std::string value_type;
	util::array<unsigned> size;
      };


      /*! \brief Retrieve header from a raw file.
       *
       * \ingroup ioraw
       */
      raw_header get_header(const std::string& filename);


# ifndef MLN_INCLUDE_ONLY


      raw_header get_header(const std::string& filename)
      {
	mln_trace("mln::io::raw::get_header");

	raw_header header;

	std::string info_filename = filename + ".info";

	std::ifstream info_file(info_filename.c_str());
	if (! info_file)
	{
	  std::cerr << "io::raw::get_header - Error: cannot open file '"
		    << filename << "'!"
		    << std::endl;
	  abort();
	}

	std::string file_type;
	info_file >> file_type;
	if (file_type != "milena/raw")
	{
	  std::cerr << "io::raw::load - Error: invalid file type. '"
		    << filename
		    << "' is NOT a valid milena/raw info file!"
		    << std::endl;
	  abort();
	}

	char dev_null[30];

	// Dimension ?
	// Reading line title "Dim: "
	info_file.read(dev_null, 5);
	info_file >> header.dim;

	// Size information - Skip it, useless.
	header.size.resize(header.dim);
	for (unsigned i = 0; i < header.dim; ++i)
	  info_file >> header.size[i];
	// Skipping endline.
	char c;
	info_file.get(c);


	// Value type name ?
	// Reading line title "data type: "
	info_file.read(dev_null, 11);
	// WARNING: value type name limited to 255 characters...
	char value_type[255];
	info_file.getline(value_type, 255);
	header.value_type = value_type;

	info_file.close();

	return header;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::raw

  } // end of namespace mln::io

} // end of namespace mln

#endif // ! MLN_IO_RAW_GET_HEADER_HH

