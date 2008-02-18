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


#ifndef UNI_SET_HH
# define UNI_SET_HH

/*! \file sandbox/pellegrin/cond_inheritance/internal/uni_set.hh
 *
 * \brief Definition of uni_set class.
 */

# include "concept/point_set.hh"


namespace mln
{
  namespace internal
  {
    /*! \brief Uni set class.
    */
    template <typename E>
    class uni_set: public concept::point_set<E>
    {
    protected:
      uni_set ();
    };

# ifndef MLN_INCLUDE_ONLY

    template <typename E>
    inline
    uni_set<E>::uni_set ()
    {
      std::cout << "      " << __FILE__
		<< ": constructeur mln::internal::uni_set<E>" << std::endl;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! UNI_SET_HH
