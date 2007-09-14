// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_METAL_TEMPLATED_BY_HH
# define MLN_METAL_TEMPLATED_BY_HH

/*! \file mln/metal/templated_by.hh
 *
 * \brief check if a type is templated by a given template.
 *
 * \todo : make it more generic,
 *         make it work on g++-2.95
 *         (where templated_by< X<i>, Y >::check() compile)
 */

# include <mln/metal/bool.hh>


# define mln_templated_by(T1, T2) mln::metal::templated_by< T1, T2 >


namespace mln
{

  namespace metal
  {

    template <typename T1, template <unsigned int X> class T2 >
    struct templated_by : false_
    {};

    template < template <unsigned int> class T, unsigned int V>
    struct templated_by< T<V>, T > : true_
    {};



  } // end of namespace mln::metal

} // end of namespace mln


#endif // ! MLN_METAL_TEMPLATED_BY_HH
