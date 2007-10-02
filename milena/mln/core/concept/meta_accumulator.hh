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

#ifndef MLN_CORE_CONCEPT_META_ACCUMULATOR_HH
# define MLN_CORE_CONCEPT_META_ACCUMULATOR_HH

/*! \file mln/core/concept/meta_accumulator.hh
 *
 * \brief Definition of the concept of mln::Meta_Accumulator.
 */

# include <mln/core/concept/object.hh>
# include <mln/core/concept/accumulator.hh>


# define mln_accu_with(A, T) \
typename A::template with< T >::ret


# define mln_accu_result(A, T) \
typename A::template with< T >::ret::result



namespace mln
{


  /*! \brief Base class for implementation of meta accumulators.
   *
   * The parameter \a E is the exact type.
   *
   * \see mln::doc::Meta_Accumulator for a complete documentation of
   * this class contents.
   */
  template <typename E>
  struct Meta_Accumulator : public Object<E>
  {
    typedef Meta_Accumulator<void> category;
  protected:
    Meta_Accumulator();
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  Meta_Accumulator<E>::Meta_Accumulator()
  {
    // FIXME: Check "with" on E.
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_META_ACCUMULATOR_HH
