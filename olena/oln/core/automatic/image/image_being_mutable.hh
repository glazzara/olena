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

#ifndef OLN_CORE_AUTOMATIC_IMAGE_IMAGE_BEING_MUTABLE_HH
# define OLN_CORE_AUTOMATIC_IMAGE_IMAGE_BEING_MUTABLE_HH

# include <oln/core/automatic/impl.hh>
# include <oln/morpher/tags.hh>


namespace oln
{
  // Forward declaration.
  namespace abstract
  {
    template <typename E> class image_being_mutable;

  } // end of namespace oln::abstract


  namespace automatic
  {
    /// Implementation corresponding to the interface
    /// oln::abstract::image_being_mutable for an identity morpher.
    template <typename E>
    class set_impl<abstract::image_being_mutable, morpher::tag::identity, E> :
      public virtual stc::any__simple<E>
    {
    public:
      /// Accessor delegation.
      oln_lvalue(E) impl_op_readwrite(const oln_psite(E)& p);
    };

# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    oln_lvalue(E)
    set_impl<abstract::image_being_mutable, morpher::tag::identity, E>
    ::impl_op_readwrite(const oln_psite(E)& p)
    {
      return this->exact().delegate().operator()(p);
    }

# endif

  } // end of namespace oln::automatic
  
} // end of namespace oln

#endif // ! OLN_CORE_AUTOMATIC_IMAGE_IMAGE_BEING_MUTABLE_HH
