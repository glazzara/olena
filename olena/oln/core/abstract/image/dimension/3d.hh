// Copyright (C) 2005, 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_ABSTRACT_IMAGE_DIMENSION_3D_HH
# define OLN_CORE_ABSTRACT_IMAGE_DIMENSION_3D_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/automatic/image/image3d.hh>


namespace oln
{
 
  namespace abstract
  {
 
    /// Class of 3D images.
    template <typename E>
    struct image3d :
      public virtual image<E>,
      public automatic::get_impl<image3d, E>
    {
    public:

      oln_rvalue(E) at(const oln_coord(E)& slice,
		       const oln_coord(E)& row,
		       const oln_coord(E)& col) const;

      // FIXME: Hack (should be elsewhere)!
      oln_lvalue(E) at(const oln_coord(E)& slice,
		       const oln_coord(E)& row,
		       const oln_coord(E)& col);
      bool has_at(const oln_coord(E)& slice,
		  const oln_coord(E)& row,
		  const oln_coord(E)& col) const;

    protected:
      /// Constructor (protected, empty).
      image3d();
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    image3d<E>::image3d()
    {
    }

    template <typename E>
    oln_rvalue(E)
    image3d<E>::at(const oln_coord(E)& slice,
		   const oln_coord(E)& row,
		   const oln_coord(E)& col) const
    {
      return this->exact().impl_at(slice, row, col);
    }

    template <typename E>
    oln_lvalue(E)
    image3d<E>::at(const oln_coord(E)& slice,
		   const oln_coord(E)& row,
		   const oln_coord(E)& col)
    {
      return this->exact().impl_at(slice, row, col);
    }

    template <typename E>
    bool
    image3d<E>::has_at(const oln_coord(E)& slice,
		       const oln_coord(E)& row,
		       const oln_coord(E)& col) const
    {
      return this->exact().impl_has_at(slice, row, col);
    }

# endif
 
  } // end of namespace oln::abstract

} // end of namespace oln


#endif // ! OLN_CORE_ABSTRACT_IMAGE_DIMENSION_3D_HH
