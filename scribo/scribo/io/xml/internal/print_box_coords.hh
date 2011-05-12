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

#ifndef SCRIBO_IO_XML_INTERNAL_PRINT_BOX_COORDS_HH
# define SCRIBO_IO_XML_INTERNAL_PRINT_BOX_COORDS_HH

/// \file
///
/// \brief Prints box2d coordinates to XML data.

# include <fstream>
# include <mln/core/alias/box2d.hh>

namespace scribo
{

  namespace io
  {

    namespace xml
    {

      namespace internal
      {

	using namespace mln;

        /*! \brief Prints box2d coordinates to XML data.
	 */
	void
	print_box_coords(std::ofstream& ostr, const box2d& b,
			 const char *space);


# ifndef MLN_INCLUDE_ONLY


	inline
	void
	print_box_coords(std::ofstream& ostr, const box2d& b,
			 const char *space)
	{
	  std::string sc = space;
	  std::string sp = sc + "  ";
	  ostr << sc << "<Coords>" << std::endl
	       << sp << "<Point x=\"" << b.pmin().col()
	       << "\" y=\"" << b.pmin().row() << "\"/>"
	       << std::endl
	       << sp << "<Point x=\"" << b.pmax().col()
	       << "\" y=\"" << b.pmin().row() << "\"/>"
	       << std::endl
	       << sp << "<Point x=\"" << b.pmax().col()
	       << "\" y=\"" << b.pmax().row() << "\"/>"
	       << std::endl
	       << sp << "<Point x=\"" << b.pmin().col()
	       << "\" y=\"" << b.pmax().row() << "\"/>"
	       << std::endl
	       << sc << "</Coords>" << std::endl;

	}


# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::io::xml::internal

    } // end of namespace scribo::io::xml

  } // end of namespace scribo::io

} // end of namespace scribo

#endif // ! SCRIBO_IO_XML_INTERNAL_PRINT_BOX_COORDS_HH
