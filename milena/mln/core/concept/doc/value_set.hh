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

/*! \file mln/core/concept/doc/value_set.hh
 * \brief This file documents the concept of mln::Value_Set.
 */

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Value_Set.
     *
     * \see mln::Value_Set
     */
    template <typename E>
    struct Value_Set : public Object<E>
    {
      /*! \brief Value associated type.
       */
      typedef void value;

      /*! \brief Forward Value_Iterator associated type.
       */
      typedef void fwd_viter;

      /*! \brief Backward Value_Iterator associated type.
       */
      typedef void bkd_viter;

      /*! \brief Test if \p v belongs to this set of values.
       *
       * \param[in] v A value.
       * \return True if \p v is an element of the set of values.
       */
      bool has(const value& v) const;

      /*! \brief Give the number of values in this set.
       */
      std::size_t nvalues() const;

      /*! \brief Give the \p i-th value of this set.
       */
      value operator[](std::size_t i) const;

      /*! \brief Give the index of value \p v in this set.
       */
      std::size_t index_of(const value& v) const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
