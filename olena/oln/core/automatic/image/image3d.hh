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

#ifndef OLN_CORE_AUTOMATIC_IMAGE_IMAGE3D_HH
# define OLN_CORE_AUTOMATIC_IMAGE_IMAGE3D_HH

# include <oln/core/automatic/impl.hh>
# include <oln/morpher/tags.hh>
# include <oln/core/3d/point3d.hh>


namespace oln
{
  // Forward declaration.
  namespace abstract
  {
    template <typename E> class image3d;

  } // end of namespace oln::abstract


  namespace automatic
  {


    /// Default implementation corresponding to the interface
    /// oln::abstract::image3d.
    template <typename E, typename tag>
    class set_impl<abstract::image3d, tag, E> :
      public virtual stc::any__simple<E>
    {
    public:

      oln_rvalue(E) impl_at(const oln_coord(E)& slice,
			    const oln_coord(E)& row,
			    const oln_coord(E)& col) const;

      // FIXME: Hack.
      oln_lvalue(E) impl_at(const oln_coord(E)& slice,
			    const oln_coord(E)& row,
			    const oln_coord(E)& col);
      bool impl_has_at(const oln_coord(E)& slice,
		       const oln_coord(E)& row,
		       const oln_coord(E)& col) const;

    };



    /// Implementation corresponding to the interface
    /// oln::abstract::image3d for an identity morpher.
    template <typename E>
    class set_impl<abstract::image3d, morpher::tag::identity, E> :
      public virtual stc::any__simple<E>
    {
    public:

      oln_rvalue(E) impl_at(const oln_coord(E)& slice,
			    const oln_coord(E)& row,
			    const oln_coord(E)& col) const;

      // FIXME: Hack.
      oln_lvalue(E) impl_at(const oln_coord(E)& slice,
			    const oln_coord(E)& row,
			    const oln_coord(E)& col);
      bool impl_has_at(const oln_coord(E)& slice,
		       const oln_coord(E)& row,
		       const oln_coord(E)& col) const;

    };



# ifndef OLN_INCLUDE_ONLY


    // Default is: 1. convert (slice, row, col) -> p then 2. call operator()(p).

    template <typename E, typename tag>
    oln_rvalue(E)
    set_impl<abstract::image3d, tag, E>
    ::impl_at(const oln_coord(E)& slice,
	      const oln_coord(E)& row,
	      const oln_coord(E)& col) const
    {
      point3d tmp(slice, row, col);
      return this->exact().operator()(tmp);
    }

    template <typename E, typename tag>
    oln_lvalue(E)
    set_impl<abstract::image3d, tag, E>
    ::impl_at(const oln_coord(E)& slice,
	      const oln_coord(E)& row,
	      const oln_coord(E)& col)
    {
      point3d tmp(slice, row, col);
      return this->exact().operator()(tmp);
    }

    template <typename E, typename tag>
    bool
    set_impl<abstract::image3d, tag, E>
    ::impl_has_at(const oln_coord(E)& slice,
		  const oln_coord(E)& row,
		  const oln_coord(E)& col) const
    {
      point3d tmp(slice, row, col);
      return this->exact().has(tmp);
    }


    // For morphers: delegate.

    template <typename E>
    oln_rvalue(E)
    set_impl<abstract::image3d, morpher::tag::identity, E>
    ::impl_at(const oln_coord(E)& slice,
	      const oln_coord(E)& row,
	      const oln_coord(E)& col) const
    {
      return this->exact().delegate().at(slice, row, col);
    }

    template <typename E>
    oln_lvalue(E)
    set_impl<abstract::image3d, morpher::tag::identity, E>
    ::impl_at(const oln_coord(E)& slice,
	      const oln_coord(E)& row,
	      const oln_coord(E)& col)
    {
      return this->exact().delegate().at(slice, row, col);
    }

    template <typename E>
    bool
    set_impl<abstract::image3d, morpher::tag::identity, E>
    ::impl_has_at(const oln_coord(E)& slice,
		  const oln_coord(E)& row,
		  const oln_coord(E)& col) const
    {
      return this->exact().delegate().has_at(slice, row, col);
    }

# endif

  } // end of namespace oln::automatic
  
} // end of namespace oln

#endif // ! OLN_CORE_AUTOMATIC_IMAGE_IMAGE3D_HH
