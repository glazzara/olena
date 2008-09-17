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
 * \brief Define some implementation classes to provide methods
 * related to classes having a bounding box.
 */

# include <mln/core/internal/force_exact.hh>


namespace mln
{

  namespace internal
  {

    // box_impl

    /*! Implementation class to equip objects having a bounding
     * box.
     *
     */
    template <unsigned n, typename C, typename E>
    struct box_impl_;

    template <typename C, typename E> // FIXME: Add an extra param to replace 'unsigned'.
    struct box_impl_<3, C, E>
    {
      /// Give the number of slis.
      unsigned nslis() const;

      /// Give the minimum sli.
      C min_sli() const;

      /// Give the minimum sli.
      C max_sli() const;

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

    template <typename C, typename E> // FIXME: Add an extra param to replace 'unsigned'.
    struct box_impl_<2, C, E>
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

    template <typename C, typename E> // FIXME: Add an extra param to replace 'unsigned'.
    struct box_impl_<1, C, E>
    {
      /// Give the number of inds.
      unsigned ninds() const;

      /// Give the minimum ind.
      C min_ind() const;

      /// Give the minimum ind.
      C max_ind() const;
    };


# ifndef MLN_INCLUDE_ONLY

    // box_impl

    // 3

    template <typename C, typename E>
    inline
    unsigned box_impl_<3, C, E>::nslis() const
    {
      return internal::force_exact<E>(*this).bbox().len(0);
    }

    template <typename C, typename E>
    inline
    C box_impl_<3, C, E>::min_sli() const
    {
      return internal::force_exact<E>(*this).bbox().pmin()[0];
    }

    template <typename C, typename E>
    inline
    C box_impl_<3, C, E>::max_sli() const
    {
      return internal::force_exact<E>(*this).bbox().pmax()[0];
    }

    template <typename C, typename E>
    inline
    unsigned box_impl_<3, C, E>::nrows() const
    {
      return internal::force_exact<E>(*this).bbox().len(1);
    }

    template <typename C, typename E>
    inline
    C box_impl_<3, C, E>::min_row() const
    {
      return internal::force_exact<E>(*this).bbox().pmin()[1];
    }

    template <typename C, typename E>
    inline
    C box_impl_<3, C, E>::max_row() const
    {
      return internal::force_exact<E>(*this).bbox().pmax()[1];
    }

    template <typename C, typename E>
    inline
    unsigned box_impl_<3, C, E>::ncols() const
    {
      return internal::force_exact<E>(*this).bbox().len(2);
    }

    template <typename C, typename E>
    inline
    C box_impl_<3, C, E>::min_col() const
    {
      return internal::force_exact<E>(*this).bbox().pmin()[2];
    }

    template <typename C, typename E>
    inline
    C box_impl_<3, C, E>::max_col() const
    {
      return internal::force_exact<E>(*this).bbox().pmax()[2];
    }


    // 2

    template <typename C, typename E>
    inline
    unsigned box_impl_<2, C, E>::nrows() const
    {
      return internal::force_exact<E>(*this).bbox().len(0);
    }

    template <typename C, typename E>
    inline
    C box_impl_<2, C, E>::min_row() const
    {
      return internal::force_exact<E>(*this).bbox().pmin()[0];
    }

    template <typename C, typename E>
    inline
    C box_impl_<2, C, E>::max_row() const
    {
      return internal::force_exact<E>(*this).bbox().pmax()[0];
    }

    template <typename C, typename E>
    inline
    unsigned box_impl_<2, C, E>::ncols() const
    {
      return internal::force_exact<E>(*this).bbox().len(1);
    }

    template <typename C, typename E>
    inline
    C box_impl_<2, C, E>::min_col() const
    {
      return internal::force_exact<E>(*this).bbox().pmin()[1];
    }

    template <typename C, typename E>
    inline
    C box_impl_<2, C, E>::max_col() const
    {
      return internal::force_exact<E>(*this).bbox().pmax()[1];
    }

    // 1

    template <typename C, typename E>
    inline
    unsigned box_impl_<1, C, E>::ninds() const
    {
      return internal::force_exact<E>(*this).bbox().len(0);
    }

    template <typename C, typename E>
    inline
    C box_impl_<1, C, E>::min_ind() const
    {
      return internal::force_exact<E>(*this).bbox().pmin()[0];
    }

    template <typename C, typename E>
    inline
    C box_impl_<1, C, E>::max_ind() const
    {
      return internal::force_exact<E>(*this).bbox().pmax()[0];
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_BOX_IMPL_HH
