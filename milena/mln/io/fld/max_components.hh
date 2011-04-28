// Copyright (C) 2008, 2009, 2010, 2011 EPITA Research and Development
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

#ifndef MLN_IO_FLD_MAX_COMPONENTS_HH
# define MLN_IO_FLD_MAX_COMPONENTS_HH

/// \file
///
/// \fixme Don't we want to change the return type 'unsigned' to a
/// larger type or make it templated?

# include <mln/algebra/vec.hh>
# include <mln/value/rgb.hh>
# include <mln/io/fld/header.hh>

namespace mln
{

  namespace io
  {

    namespace fld
    {

      template <typename V>
      inline
      unsigned max_component(const V&);

      template <unsigned n, typename V>
      inline
      unsigned max_component(const algebra::vec<n, V>& v);

      template <unsigned n>
      inline
      unsigned max_component(const value::rgb<n>&);

      inline
      unsigned max_component(const fld::data_type::E& t);

# ifndef MLN_INCLUDE_ONLY

      template <typename V>
      inline
      unsigned max_component(const V&)
      {
	return unsigned(mln_max(V));
      }


      template <unsigned n, typename V>
      inline
      unsigned max_component(const algebra::vec<n, V>& v)
      {
	(void) v;
	return unsigned(mln_max(V));
      }

      template <unsigned n>
      inline
      unsigned max_component(const value::rgb<n>&)
      {
	return mln_max(mln::value::int_u<n>);
      }

      inline
      unsigned max_component(const fld::data_type::E& t)
      {
	switch (t)
	  {
	    case data_type::BYTE: return mln_max(unsigned char);
	    case data_type::SHORT: return mln_max(unsigned short);
	    case data_type::INTEGER: return mln_max(unsigned);
	    case data_type::FLOAT: return unsigned(mln_max(float));
	    case data_type::DOUBLE: return unsigned(mln_max(double));
	    default: return 0;
	  }
      }

# endif // ! MLN_INCLUDE_ONLY
    }

  }

}

#endif // !MLN_IO_FLD_MAX_COMPONENTS_HH
