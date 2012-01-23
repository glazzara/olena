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

#ifndef MLN_IO_FLD_HEADER_HH
# define MLN_IO_FLD_HEADER_HH

namespace mln
{

  namespace io
  {

    namespace fld
    {


      struct data_type { enum E { UNKNOWN, BYTE, SHORT, INTEGER, FLOAT, DOUBLE }; };
      struct field_type { enum E { UNKNOWN, UNIFORM, RECTILINEAR, IRREGULAR }; };

      /*! \brief Define the header structure of an AVS field data file.
       *
       * \ingroup iofld
       */
      struct fld_header
      {
	int		ndim;	  // The number of computational dimensions in the field.
	int*		dim;	  // The dimension size of each axis.
	int		nspace;   // The number of physical coordinates per field element.
	int		veclen;   // The number of data values for each field element.
	data_type::E	data;	  // The primitive data type of all the data values.
	field_type::E	field;	  // The field type.
	float*		min_ext;  // The minimum coordinate value that any member data point occupies in space.
	float*		max_ext;  // The maximum coordinate value that any member data point occupies in space.
	// std::vector<std::string>  label; // Not handled.
	// std::vector<std::string>  unit; // Not handled.
	// void*	min_val; // The minimum data value in the field. (Not used)
	// void*	max_val; // The maximum data value in the field. (Not used)
	// struct {...}	variable; // Not handled.
	// struct {...}	coord;	  // Not handled.

	fld_header();
	~fld_header();
      };


# ifndef MLN_INCLUDE_ONLY

      inline
      fld_header::fld_header()
	: ndim (-1),
	  dim (0),
	  nspace (-1),
	  veclen (-1),
	  data (data_type::UNKNOWN),
	  field (field_type::UNKNOWN)
      {
      }

      inline
      fld_header::~fld_header()
      {
	delete [] dim;
	delete [] max_ext;
	delete [] min_ext;
      }

# endif // ! MLN_INCLUDE_ONLY

    }  // end of namespace mln::io::fld

  } // end of namespace mln::io

} // end of namespace mln

#endif // !MLN_IO_FLD_HEADER_HH
