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

#ifndef OLN_CORE_INTERNAL_ITERATOR_ON_POINTS_IMPL_HH
# define OLN_CORE_INTERNAL_ITERATOR_ON_POINTS_IMPL_HH

# include <oln/core/equipment.hh>


namespace oln
{

  namespace internal
  {

    // base impl

    template <typename Exact>
    struct iterator_on_points_impl_base_
    {
    private:
      stc_typename(point);
      typedef typename point::vec_t vec_t;
    public:
      const vec_t& vec() const;
      vec_t& vec();
    };


    // no-d impl is empty

    template <unsigned n, typename Exact>
    struct iterator_on_points_impl_
    {
    };

    // 1d impl
    template <typename Exact>
    struct iterator_on_points_impl_< 1, Exact >
      :
      public iterator_on_points_impl_base_<Exact>
    {
    private:
      stc_typename(coord);
    public:
      coord ind() const;
    };

    // 2d impl
    template <typename Exact>
    struct iterator_on_points_impl_< 2, Exact >
      :
      public iterator_on_points_impl_base_<Exact>
    {
    private:
      stc_typename(coord);
    public:
      coord row() const;
      coord col() const;
    };

    // 3d impl
    template <typename Exact>
    struct iterator_on_points_impl_< 3, Exact >
      :
      public iterator_on_points_impl_base_<Exact>
    {
    private:
      stc_typename(coord);
    public:
      coord sli() const;
      coord row() const;
      coord col() const;
    };

# ifndef OLN_INCLUDE_ONLY

    // base impl

    template <typename Exact>
    const typename iterator_on_points_impl_base_<Exact>::vec_t&
    iterator_on_points_impl_base_<Exact>::vec() const
    {
      return static_cast<const Exact*>(this)->impl_point_adr()->vec();
    }

    template <typename Exact>
    typename iterator_on_points_impl_base_<Exact>::vec_t&
    iterator_on_points_impl_base_<Exact>::vec()
    {
      return const_cast<vec_t&>(static_cast<Exact*>(this)->impl_point_adr()->vec());
    }

    // 1d impl

    template <typename Exact>
    typename iterator_on_points_impl_<1, Exact>::coord
    iterator_on_points_impl_<1, Exact>::ind() const
    {
      return static_cast<const Exact*>(this)->impl_point_adr()->ind();
    }

    // 2d impl

    template <typename Exact>
    typename iterator_on_points_impl_<2, Exact>::coord
    iterator_on_points_impl_<2, Exact>::row() const
    {
      return static_cast<const Exact*>(this)->impl_point_adr()->row();
    }

    template <typename Exact>
    typename iterator_on_points_impl_<2, Exact>::coord
    iterator_on_points_impl_<2, Exact>::col() const
    {
      return static_cast<const Exact*>(this)->impl_point_adr()->col();
    }

    // 3d impl

    template <typename Exact>
    typename iterator_on_points_impl_<3, Exact>::coord
    iterator_on_points_impl_<3, Exact>::sli() const
    {
      return static_cast<const Exact*>(this)->impl_point_adr()->sli();
    }

    template <typename Exact>
    typename iterator_on_points_impl_<3, Exact>::coord
    iterator_on_points_impl_<3, Exact>::row() const
    {
      return static_cast<const Exact*>(this)->impl_point_adr()->row();
    }

    template <typename Exact>
    typename iterator_on_points_impl_<3, Exact>::coord
    iterator_on_points_impl_<3, Exact>::col() const
    {
      return static_cast<const Exact*>(this)->impl_point_adr()->col();
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::internal

} // end of namespace oln

#endif // ! OLN_CORE_INTERNAL_ITERATOR_ON_POINTS_IMPL_HH
