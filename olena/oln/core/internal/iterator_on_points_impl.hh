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


namespace oln
{

  // Forward declarations
  namespace impl
  {
    template <unsigned n, typename Exact, typename P>
    struct iterator_on_points_impl;
  }


  template <unsigned n, typename Exact, typename P>
  struct super_trait_< impl::iterator_on_points_impl<n, Exact, P> >
  {
    typedef Any<P> ret;
  };

  template <unsigned n, typename Exact, typename P>
  struct vtypes< impl::iterator_on_points_impl<n, Exact, P> >
  {
  };


  namespace impl
  {
    template <unsigned n, typename Exact, typename P>
    struct iterator_on_points_impl : public virtual Any<Exact>
    {
    };

    /// point1d interface
    template <typename Exact, typename P>
    struct iterator_on_points_impl<1, Exact, P> : public virtual Any< Exact >
    {
      typename P::coord ind() const;
    };

    /// point2d interface
    template <typename Exact, typename P>
    struct iterator_on_points_impl<2, Exact, P>: public virtual Any< Exact >
    {
      typename P::coord row() const;
      typename P::coord col() const;
    };

    ///  point3d interface
    template <typename Exact, typename P>
    struct iterator_on_points_impl<3, Exact, P>: public virtual Any< Exact >
    {
      typename P::coord row() const;
      typename P::coord col() const;
      typename P::coord slice() const;
    };

# ifndef OLN_INCLUDE_ONLY

    // point1d
    template <typename Exact, typename P>
    typename P::coord
    iterator_on_points_impl<1, Exact, P>::ind() const
    {
      return exact(this)->to_point().ind();
    }

    // point2d
    template <typename Exact, typename P>
    typename P::coord
    iterator_on_points_impl<2, Exact, P>::row() const
    {
      return exact(this)->to_point().row();
    }


    template <typename Exact, typename P>
    typename P::coord
    iterator_on_points_impl<2, Exact, P>::col() const
    {
      return exact(this)->to_point().col();
    }

    // point3d
    template <typename Exact, typename P>
    typename P::coord
    iterator_on_points_impl<3, Exact, P>::row() const
    {
      return exact(this)->to_point().row();
    }

    template <typename Exact, typename P>
    typename P::coord
    iterator_on_points_impl<3, Exact, P>::col() const
    {
      return exact(this)->to_point().col();
    }

    template <typename Exact, typename P>
    typename P::coord
    iterator_on_points_impl<3, Exact, P>::slice() const
    {
      return exact(this)->to_point().slice();
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln

} // end of namespace oln

#endif // ! OLN_CORE_INTERNAL_ITERATOR_ON_POINTS_IMPL_HH
