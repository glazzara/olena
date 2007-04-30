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

#ifndef OLN_CORE_INTERNAL_POINT_IMPL_HH
# define OLN_CORE_INTERNAL_POINT_IMPL_HH

# include <oln/core/equipment.hh>


namespace oln
{

  namespace internal
  {

    // default

    template <unsigned dim, typename Exact>
    struct point_impl_
    {
      // nothing
    };


    // dim == 1

    template <typename Exact>
    struct point_impl_< 1, Exact >
    {
    private:
      stc_typename(coord);
    public:

      coord  ind() const;
      coord& ind();
    };


    // dim == 2

    template <typename Exact>
    struct point_impl_< 2, Exact >
    {
    private:
      stc_typename(coord);
    public:

      coord  row() const;
      coord& row();

      coord  col() const;
      coord& col();
    };


    // dim == 3

    template <typename Exact>
    struct point_impl_< 3, Exact >
    {
    private:
      stc_typename(coord);
    public:

      coord  sli() const;
      coord& sli();

      coord  row() const;
      coord& row();

      coord  col() const;
      coord& col();      
    };


# ifndef OLN_INCLUDE_ONLY

    // dim == 1

    template <typename Exact>
    typename point_impl_<1, Exact>::coord
    point_impl_<1, Exact>::ind() const
    {
      return static_cast<const Exact&>(*this)[0];
    }

    template <typename Exact>
    typename point_impl_<1, Exact>::coord &
    point_impl_<1, Exact>::ind()
    {
      return static_cast<Exact&>(*this)[0];
    }

    // dim == 2

    template <typename Exact>
    typename point_impl_<2, Exact>::coord
    point_impl_<2, Exact>::row() const
    {
      return static_cast<const Exact&>(*this)[0];
    }

    template <typename Exact>
    typename point_impl_<2, Exact>::coord &
    point_impl_<2, Exact>::row()
    {
      return static_cast<Exact&>(*this)[0];
    }

    template <typename Exact>
    typename point_impl_<2, Exact>::coord
    point_impl_<2, Exact>::col() const
    {
      return static_cast<const Exact&>(*this)[1];
    }

    template <typename Exact>
    typename point_impl_<2, Exact>::coord &
    point_impl_<2, Exact>::col()
    {
      return static_cast<Exact&>(*this)[1];
    }

    // dim == 3

    template <typename Exact>
    typename point_impl_<3, Exact>::coord
    point_impl_<3, Exact>::sli() const
    {
      return static_cast<const Exact&>(*this)[0];
    }

    template <typename Exact>
    typename point_impl_<3, Exact>::coord &
    point_impl_<3, Exact>::sli()
    {
      return static_cast<Exact&>(*this)[0];
    }

    template <typename Exact>
    typename point_impl_<3, Exact>::coord
    point_impl_<3, Exact>::row() const
    {
      return static_cast<const Exact&>(*this)[1];
    }

    template <typename Exact>
    typename point_impl_<3, Exact>::coord &
    point_impl_<3, Exact>::row()
    {
      return static_cast<Exact&>(*this)[1];
    }

    template <typename Exact>
    typename point_impl_<3, Exact>::coord
    point_impl_<3, Exact>::col() const
    {
      return static_cast<const Exact&>(*this)[2];
    }

    template <typename Exact>
    typename point_impl_<3, Exact>::coord &
    point_impl_<3, Exact>::col()
    {
      return static_cast<Exact&>(*this)[2];
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_POINT_IMPL_HH
