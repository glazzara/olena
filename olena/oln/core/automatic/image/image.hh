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

#ifndef OLN_CORE_AUTOMATIC_IMAGE_IMAGE_HH
# define OLN_CORE_AUTOMATIC_IMAGE_IMAGE_HH

# include <oln/core/automatic/impl.hh>
# include <oln/morpher/tags.hh>


namespace oln
{
  // Forward declaration.
  namespace abstract
  {
    template <typename E> class image;

  } // end of namespace oln::abstract


  namespace automatic
  {

    /// Implementation corresponding to the interface
    /// oln::abstract::image for an identity morpher.
    template <typename E>
    class set_impl<abstract::image, morpher::tag::identity, E> :
      public virtual stc::any__simple<E>
    {
    private:

      typedef oln_type_of(E, topo)   topo_t;
      typedef oln_type_of(E, rvalue) rvalue_t;
      typedef oln_type_of(E, psite)  psite_t;

    public:

      /// Delegation.

      const topo_t& impl_topo() const;
      rvalue_t impl_op_read(const psite_t& p) const;

    };


# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    const typename set_impl<abstract::image, morpher::tag::identity, E>::topo_t&
    set_impl<abstract::image, morpher::tag::identity, E>::impl_topo() const
    {
      return this->exact().delegate().topo();
    }

    template <typename E>
    typename set_impl<abstract::image, morpher::tag::identity, E>::rvalue_t
    set_impl<abstract::image, morpher::tag::identity, E>::impl_op_read(const typename set_impl<abstract::image, morpher::tag::identity, E>::psite_t& p) const
    {
      return this->exact().delegate().operator()(p);
    }

# endif

  } // end of namespace oln::automatic
  
} // end of namespace oln

#endif // ! OLN_CORE_AUTOMATIC_IMAGE_IMAGE_HH
