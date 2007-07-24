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

#ifndef MLN_CORE_TRAIT_IS_LOWQ_HH
# define MLN_CORE_TRAIT_IS_LOWQ_HH

/*! \file mln/core/trait/is_lowq.hh
 *
 * \brief Definition of the is_lowq image trait.
 */

# include <mln/metal/bool.hh>
# include <mln/value/props.hh>


# define mln_is_lowq(T)  typename mln::trait::is_lowq< T >::ret



namespace mln
{

  namespace trait
  {


    // FIXME: Doc!

    template <typename T>
    struct is_lowq
    {
      typedef typename metal::bool_<( mln_card(T) != 0 )>::type ret;
    };


  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_CORE_TRAIT_IS_LOWQ_HH
