// Copyright (C) 2008, 2009, 2011, 2012 EPITA Research and Development
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

#ifndef MLN_IO_FLD_LOAD_HEADER_HH
# define MLN_IO_FLD_LOAD_HEADER_HH

///
/// \brief Read AVS header from a file.
///
///

# include <mln/io/fld/header.hh>
# include <cstdlib>
# include <locale>
# include <iostream>
# include <sstream>
# include <string>

namespace mln
{

  namespace io
  {

    namespace fld
    {

      /*! \brief Read the header form an AVS field file.
       *
       * \param ins The file to read.
       *
       * \return The header.
       *
       * \ingroup iofld
       */
      fld_header read_header(std::istream& ins);

# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	inline
	void
	abort_fld_reader(const char* msg, unsigned line = 0)
	{
	  std::cerr << "AVS field file reader: " << msg << " on line " << line << std::endl;
	  abort();
	}

      }

      inline
      fld_header
      read_header(std::istream& file)
      {
	std::stringstream ins;
	std::string line_str, lhs, rhs;
	fld_header header;
	unsigned line;

	std::getline(file, line_str);
	line = 1;
	if (line_str.compare(0, 5, "# AVS"))
	  internal::abort_fld_reader("Invalid format", line);

	while (file.good() && file.peek() != '\f')
	  {
	    std::getline(file, line_str);
	    ++line;

	    ins.clear();
	    ins.str(line_str);
	    rhs.clear();
	    lhs.clear();

	    { // Parse the line
	      char c = ins.get();
	      while (isspace(c))
		ins.get(c);
	      if (c == '#') // Comments
		continue;
	      while (isalnum(c) || c == '_')
		{
		  lhs.push_back(c);
		  ins.get(c);
		}
	      while (isspace(c))
		ins.get(c);
	      if (c != '=')
		internal::abort_fld_reader("Parse error", line);
	      while (isspace(ins.peek()))
		ins.ignore();
	    }

	    if (lhs == "ndim")
	      {
		ins >> header.ndim;
		if (header.ndim < 1)
		  internal::abort_fld_reader("Invalid dimension", line);
		header.dim = new int[header.ndim];
		std::fill(header.dim, header.dim + header.ndim, -1);
	      }
	    else if (lhs.compare(0, 3, "dim") == 0)
	      {
		std::stringstream ss(lhs.substr(3));
		int dim;
		ss >> dim;
		if (dim < 1 || dim > header.ndim)
		  internal::abort_fld_reader("Invalid dimension", line);
		if (!ss.eof())
		  internal::abort_fld_reader("Parse error", line);
		ins >> header.dim[dim - 1];
		if (header.dim[dim - 1] < 1)
		  internal::abort_fld_reader("Invalid dimension", line);
	      }
	    else if (lhs == "nspace")
	      {
		ins >> header.nspace;
		if (header.nspace < 1)
		  internal::abort_fld_reader("Invalid space dimension", line);
		header.min_ext = new float[header.nspace];
		header.max_ext = new float[header.nspace];
	      }
	    else if (lhs == "veclen")
	      {
		ins >> header.veclen;
		if (header.veclen == -1)
		  internal::abort_fld_reader("Invalid vector length", line);
	      }
	    else if (lhs == "data")
	      {
		ins >> rhs;
		if (rhs == "byte")
		  header.data = data_type::BYTE;
		else if (rhs == "short")
		  header.data = data_type::SHORT;
		else if (rhs == "integer")
		  header.data = data_type::INTEGER;
		else if (rhs == "float")
		  header.data = data_type::FLOAT;
		else if (rhs == "double")
		  header.data = data_type::DOUBLE;
		else
		  internal::abort_fld_reader("Invalid data type", line);
	      }
	    else if (lhs == "field")
	      {
		ins >> rhs;
		if (rhs != "uniform")
		  internal::abort_fld_reader("Unhandled field type", line);
		header.field = field_type::UNIFORM;
	      }
	    else if (lhs == "min_ext")
	      {
		for (int i = 0; i < header.ndim; ++i)
		  {
		    ins >> header.min_ext[i];
		    if (ins.peek() == ',')
		      ins.ignore();
		  }
	      }
	    else if (lhs == "max_ext")
	      {
		for (int i = 0; i < header.ndim; ++i)
		  {
		    ins >> header.max_ext[i];
		    if (ins.peek() == ',')
		      ins.ignore();
		  }
	      }
	    else
	      internal::abort_fld_reader("Parse error", line);

	    rhs.clear();
	    ins >> rhs;
	    if (!rhs.empty() && rhs[0] != '#')
	      internal::abort_fld_reader("Parse error", line);
	  }

	file.ignore();
	if (file.get() != '\f')
	  internal::abort_fld_reader("Parse error", line);

	if (header.ndim == -1 || header.nspace == -1 || header.veclen == -1 ||
	    header.data == data_type::UNKNOWN || header.field == field_type::UNKNOWN)
	  internal::abort_fld_reader("Invalid format", line);
	for (int i = 0; i < header.ndim; ++i)
	  if (header.dim[i] == -1)
	    internal::abort_fld_reader("Invalid format", line);
	return header;
      }

# endif // ! MLN_INCLUDE_ONLY

    }  // end of namespace mln::io::fld

  } // end of namespace mln::io

} // end of namespace mln

#endif // !MLN_IO_FLD_LOAD_HEADER_HH
