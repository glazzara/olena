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

#ifndef MLN_TRAIT_VALUE__HH
# define MLN_TRAIT_VALUE__HH

/*! \file mln/trait/images.hh
 *
 * \brief Forward declarations of all image types.
 *
 * \todo Split this file into many.
 */

# include <iostream>
# include <string>

# include <mln/trait/undef.hh>

# include <mln/trait/value/nature.hh>
# include <mln/trait/value/kind.hh>


# define mln_trait_value_nature(I) typename mln::trait::value_< V >::nature
# define mln_trait_value_kind(V)   typename mln::trait::value_< V >::kind


namespace mln
{

  namespace trait
  {


    template <typename V>
    struct undefined_value_
    {
      typedef undef nature;
      typedef undef kind;
    };


    template <typename V>
    struct value_ : undefined_value_<V>
    {
    };


  } // end of namespace mln::trait

} // end of namespace mln


# include <mln/trait/value/print.hh>


#endif // ! MLN_TRAIT_VALUE__HH
