// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_AUTOMATIC_IMAGE_IMAGE1D_HH
# define OLN_CORE_AUTOMATIC_IMAGE_IMAGE1D_HH

# include <oln/core/automatic/impl.hh>
# include <oln/morpher/tags.hh>
# include <oln/core/1d/point1d.hh>


namespace oln
{
  // Forward declaration.
  namespace abstract
  {
    template <typename E> class image1d;

  } // end of namespace oln::abstract


  namespace automatic
  {


    /// Default implementation corresponding to the interface
    /// oln::abstract::image1d.
    template <typename E, typename tag>
    class set_impl<abstract::image1d, tag, E> :
      public virtual stc::any__simple<E>
    {
    public:

      oln_rvalue(E) impl_at(const oln_coord(E)& index) const;

      // FIXME: Hack.
      oln_lvalue(E)& impl_at(const oln_coord(E)& index);
      bool impl_has_at(const oln_coord(E)& index) const;

    };



    /// Implementation corresponding to the interface
    /// oln::abstract::image1d for an identity morpher.
    template <typename E>
    class set_impl<abstract::image1d, morpher::tag::identity, E> :
      public virtual stc::any__simple<E>
    {
    public:

      oln_rvalue(E) impl_at(const oln_coord(E)& index) const;

      // FIXME: Hack.
      oln_lvalue(E)& impl_at(const oln_coord(E)& index);
      bool impl_has_at(const oln_coord(E)& index) const;

    };



# ifndef OLN_INCLUDE_ONLY


    // Default is: 1. convert (index) -> p then 2. call operator()(p).

    template <typename E, typename tag>
    oln_rvalue(E)
    set_impl<abstract::image1d, tag, E>
    ::impl_at(const oln_coord(E)& index) const
    {
      point1d tmp(index);
      return this->exact().operator()(tmp);
    }

    template <typename E, typename tag>
    oln_lvalue(E)&
    set_impl<abstract::image1d, tag, E>
    ::impl_at(const oln_coord(E)& index)
    {
      point1d tmp(index);
      return this->exact().operator()(tmp);
    }

    template <typename E, typename tag>
    bool
    set_impl<abstract::image1d, tag, E>
    ::impl_has_at(const oln_coord(E)& index) const
    {
      point1d tmp(index);
      return this->exact().has(tmp);
    }


    // For morphers: delegate.

    template <typename E>
    oln_rvalue(E)
    set_impl<abstract::image1d, morpher::tag::identity, E>
    ::impl_at(const oln_coord(E)& index) const
    {
      return this->exact().delegate().at(index);
    }

    template <typename E>
    oln_lvalue(E)&
    set_impl<abstract::image1d, morpher::tag::identity, E>
    ::impl_at(const oln_coord(E)& index)
    {
      return this->exact().delegate().at(index);
    }

    template <typename E>
    bool
    set_impl<abstract::image1d, morpher::tag::identity, E>
    ::impl_has_at(const oln_coord(E)& index) const
    {
      return this->exact().delegate().has_at(index);
    }

# endif

  } // end of namespace oln::automatic
  
} // end of namespace oln

#endif // ! OLN_CORE_AUTOMATIC_IMAGE_IMAGE1D_HH
