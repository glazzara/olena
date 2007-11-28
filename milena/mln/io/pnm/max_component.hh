// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007 EPITA
// Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_IO_PNM_MAX_COMPONENT_HH
# define MLN_IO_PNM_MAX_COMPONENT_HH

/*!
 * \file mln/io/pnm/max_component.hh
 *
 * \brief Define a function which give the max of the component of a
 * value type.
 */

namespace mln
{

  namespace io
  {

    namespace pnm
    {
      /// Give the maximum value which can be stored as a component
      /// value type V.
      template <typename V>
      unsigned int max_component(const V&);

# ifndef MLN_INCLUDE_ONLY

      template <typename V>
      inline
      unsigned int max_component(const V&)
      {
	return mln_max(V);
      }

# ifdef MLN_VALUE_RGB_HH

      template <unsigned n>
      inline
      unsigned int max_component(const mln::value::rgb<n>&)
      {
	return mln_max( mln::value::int_u<n> );
      }

# endif

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::pnm

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PNM_LOAD_HH
