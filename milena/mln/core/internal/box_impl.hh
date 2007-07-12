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

#ifndef MLN_CORE_INTERNAL_BOX_IMPL_HH
# define MLN_CORE_INTERNAL_BOX_IMPL_HH

/*! \file mln/core/internal/box_impl.hh
 *
 * \internal
 *
 * \brief Define some implementation classes to provide methods
 * related to classes having a bounding box.
 */

# include <mln/core/internal/force_exact.hh>


namespace mln
{

  namespace internal
  {

    template <typename E>
    struct box_impl_base_
    {
      mln_internal_add_force_exact_(box_impl_base_<E>)
    };


    // box_impl

    /*! \brief Implementation class to equip objects having a bounding
     * box with methods.
     *
     * \internal
     */
    template <unsigned n, typename C, typename E>
    struct box_impl_;

    template <typename C, typename E> // FIXME: Add an extra param to replace 'unsigned'.
    struct box_impl_<2, C, E>
      : box_impl_base_<E>
    {
      /// Give the number of rows.
      unsigned nrows() const;

      /// Give the minimum row.
      C min_row() const;

      /// Give the minimum row.
      C max_row() const;

      /// Give the number of cols.
      unsigned ncols() const;

      /// Give the minimum col.
      C min_col() const;

      /// Give the minimum col.
      C max_col() const;
    };


# ifndef MLN_INCLUDE_ONLY
    
    // box_impl

    // 2

    template <typename C, typename E>
    unsigned box_impl_<2, C, E>::nrows() const
    {
      return this->force_exact_().bbox().len(0);
    }

    template <typename C, typename E>
    C box_impl_<2, C, E>::min_row() const
    {
      return this->force_exact_().bbox().pmin()[0];
    }

    template <typename C, typename E>
    C box_impl_<2, C, E>::max_row() const
    {
      return this->force_exact_().bbox().pmax()[0];
    }

    template <typename C, typename E>
    unsigned box_impl_<2, C, E>::ncols() const
    {
      return this->force_exact_().bbox().len(1);
    }

    template <typename C, typename E>
    C box_impl_<2, C, E>::min_col() const
    {
      return this->force_exact_().bbox().pmin()[1];
    }

    template <typename C, typename E>
    C box_impl_<2, C, E>::max_col() const
    {
      return this->force_exact_().bbox().pmax()[1];
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_BOX_IMPL_HH
