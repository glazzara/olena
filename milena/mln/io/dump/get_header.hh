// Copyright (C) 2010, 2011, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_IO_DUMP_GET_HEADER_HH
# define MLN_IO_DUMP_GET_HEADER_HH

/// \file
///
/// Load a Milena image dumped into a file.

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

    namespace dump
    {

      /// Store dump file header.
      struct dump_header
      {
	unsigned dim;
	std::string value_type;
	util::array<unsigned> size;
      };


      /*! \brief Retrieve header from a dump file.
       *
       * \ingroup iodump
       */
      dump_header get_header(const std::string& filename);


# ifndef MLN_INCLUDE_ONLY


      inline
      dump_header get_header(const std::string& filename)
      {
	mln_trace("mln::io::dump::get_header");

	dump_header header;

	std::ifstream file(filename.c_str());
	if (! file)
	{
	  std::cerr << "io::dump::get_header - Error: cannot open file '"
		    << filename << "'!"
		    << std::endl;
	  abort();
	}


	// Milena's file type ?
	std::string file_type;
	file >> file_type;
	if (file_type != "milena/dump")
	{
	  std::cerr << "io::dump::load - Error: invalid file type. '"
		    << filename
		    << "' is NOT a valid milena/dump file!"
		    << std::endl;
	  abort();
	}

	// Dimension
	file >> header.dim;

	// Size information - Skip it, useless.
	header.size.resize(header.dim);
	for (unsigned i = 0; i < header.dim; ++i)
	  file >> header.size[i];

	// Skipping endline.
	char c;
	file.get(c);

	// Value type name ?
	// WARNING: value type name limited to 255 characters...
	char value_type[255];
	file.getline(value_type, 255);
	header.value_type = value_type;

	return header;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::dump

  } // end of namespace mln::io

} // end of namespace mln

#endif // ! MLN_IO_DUMP_GET_HEADER_HH

