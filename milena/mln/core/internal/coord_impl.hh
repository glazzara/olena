// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
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

#ifndef MLN_CORE_INTERNAL_COORD_IMPL_HH
# define MLN_CORE_INTERNAL_COORD_IMPL_HH

/*! \file
 *
 * \brief Define some implementation classes for generalized points.
 */

# include <mln/core/internal/force_exact.hh>


namespace mln
{

  namespace internal
  {


    // coord_impl


    /// \brief Implementation class to equip generalized points with
    /// explicit access to their coordinates.
    template <unsigned n, typename C, typename E>
    struct coord_impl_;

    /// \brief Specialization for 1D points.
    template <typename C, typename E>
    struct coord_impl_<1, C, E>
    {
      /// Index.
      const C& ind() const;
    };

    /// \brief Specialization for 2D points.
    template <typename C, typename E>
    struct coord_impl_<2, C, E>
    {
      /// Row index.
      const C&row() const;
      /// Column index.
      const C&col() const;
    };

    /// \brief Specialization for 3D points.
    template <typename C, typename E>
    struct coord_impl_<3, C, E>
    {
      /// Slice index.
      const C&sli() const;
      /// Row index.
      const C&row() const;
      /// Col index.
      const C&col() const;
    };


    // mutable_coord_impl

    /// \brief Implementation class to equip generalized points with
    /// explicit access to their coordinates.
    template <unsigned n, typename C, typename E>
    struct mutable_coord_impl_;

    /// \brief Specialization for 1D points.
    template <typename C, typename E>
    struct mutable_coord_impl_<1, C, E>
    {
      /// Index.
      const C& ind() const;
      /// Index.
      C& ind();
    };

    /// \brief Specialization for 2D points.
    template <typename C, typename E>
    struct mutable_coord_impl_<2, C, E>
    {
      /// Row index.
      const C& row() const;
      /// Row index.
      C& row();
      /// Column index.
      const C& col() const;
      /// Column index.
      C& col();
    };

    /// \brief Specialization for 3D points.
    template <typename C, typename E>
    struct mutable_coord_impl_<3, C, E>
    {
      /// Slice index.
      const C& sli() const;
      /// Slice index.
      C& sli();
      /// Row index.
      const C& row() const;
      /// Row index.
      C& row();
      /// Column index.
      const C& col() const;
      /// Column index.
      C& col();
    };


# ifndef MLN_INCLUDE_ONLY

    // coord_impl

    // 1

    template <typename C, typename E>
    inline
    const C& coord_impl_<1, C, E>::ind() const
    {
      return internal::force_exact<E>(*this)[0];
    }

    // 2

    template <typename C, typename E>
    inline
    const C& coord_impl_<2, C, E>::row() const
    {
      return internal::force_exact<E>(*this)[0];
    }

    template <typename C, typename E>
    inline
    const C& coord_impl_<2, C, E>::col() const
    {
      return internal::force_exact<E>(*this)[1];
    }

    // 3

    template <typename C, typename E>
    inline
    const C& coord_impl_<3, C, E>::sli() const
    {
      return internal::force_exact<E>(*this)[0];
    }

    template <typename C, typename E>
    inline
    const C& coord_impl_<3, C, E>::row() const
    {
      return internal::force_exact<E>(*this)[1];
    }

    template <typename C, typename E>
    inline
    const C& coord_impl_<3, C, E>::col() const
    {
      return internal::force_exact<E>(*this)[2];
    }


    // mutable_coord_impl

    // 1

    template <typename C, typename E>
    inline
    const C& mutable_coord_impl_<1, C, E>::ind() const
    {
      return internal::force_exact<E>(*this)[0];
    }

    template <typename C, typename E>
    inline
    C& mutable_coord_impl_<1, C, E>::ind()
    {
      return internal::force_exact<E>(*this)[0];
    }

    // 2

    template <typename C, typename E>
    inline
    const C& mutable_coord_impl_<2, C, E>::row() const
    {
      return internal::force_exact<E>(*this)[0];
    }

    template <typename C, typename E>
    inline
    C& mutable_coord_impl_<2, C, E>::row()
    {
      return internal::force_exact<E>(*this)[0];
    }

    template <typename C, typename E>
    inline
    const C& mutable_coord_impl_<2, C, E>::col() const
    {
      return internal::force_exact<E>(*this)[1];
    }

    template <typename C, typename E>
    inline
    C& mutable_coord_impl_<2, C, E>::col()
    {
      return internal::force_exact<E>(*this)[1];
    }

    // 3

    template <typename C, typename E>
    inline
    const C& mutable_coord_impl_<3, C, E>::sli() const
    {
      return internal::force_exact<E>(*this)[0];
    }

    template <typename C, typename E>
    inline
    C& mutable_coord_impl_<3, C, E>::sli()
    {
      return internal::force_exact<E>(*this)[0];
    }

    template <typename C, typename E>
    inline
    const C& mutable_coord_impl_<3, C, E>::row() const
    {
      return internal::force_exact<E>(*this)[1];
    }

    template <typename C, typename E>
    inline
    C& mutable_coord_impl_<3, C, E>::row()
    {
      return internal::force_exact<E>(*this)[1];
    }

    template <typename C, typename E>
    inline
    const C& mutable_coord_impl_<3, C, E>::col() const
    {
      return internal::force_exact<E>(*this)[2];
    }

    template <typename C, typename E>
    inline
    C& mutable_coord_impl_<3, C, E>::col()
    {
      return internal::force_exact<E>(*this)[2];
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_COORD_IMPL_HH
