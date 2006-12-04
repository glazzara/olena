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

#ifndef OLN_CORE_ABSTRACT_INTERNAL_IMAGE_IMPL_HH
# define OLN_CORE_ABSTRACT_INTERNAL_IMAGE_IMPL_HH

# include <mlc/flags.hh>
# include <oln/core/typedefs.hh>
# include <oln/core/abstract/any.hh>
# include <oln/core/macros.hh>

// FIXME: To be rewtitten using Metalic's switch/case?

namespace oln
{

  namespace abstract
  {

    namespace internal
    {

      /* A summary of the implementation retrieval mechanism.


     any_best_speed<E>                        any_best_speed<E>
            ^                                        ^
            |                                        |
            |(*)                                     |
            |                                        |
    set_image_impl<A, E>                             |
            ^                                        |
            |                                        |
  get_image_impl_helper<A, D, E>  get_image_impl_helper<A, mlc::no_type, E>
            |                                        |
            `--------------.       ,-----------------'
                           |       |
                           o       o
                               o
                               |
                        get_image_impl<E>
                               ^
                               |
                             image<E>
                               ^
                               |
                              ...
                               ^
                               |
                               E
                            (client)


         The D parameter is the delegatee type.  If there is no
	 delegatee type (i.e. D = mlc::none), get_image_impl inherits
	 from oln::any_best_speed (trough get_image_impl_helper);
	 otherwise, get_image_impl is plugged to set_image_impl, a
	 class than can be specialized by the user.

         (*) This relation is not systematic (set_image_impl is to be
         defined by the user), but every specialization of
         set_image_impl *should* inherit from oln::any_best_speed.  */

      template <typename A, typename D, typename E>
      struct get_image_impl_helper;

      // Entry point.
      template <typename A, typename E>
      struct get_image_impl :
	public get_image_impl_helper <A, oln_vtype(E, delegatee), E>
      {
      };

      template <typename A, typename E>
      struct get_image_impl_helper <A, mlc::none, E> :
	public virtual oln::abstract::any_best_speed<E>
      {
	// No implementation.
      };

      // To be specialized.
      template <typename A, typename E>
      struct set_image_impl;

      template <typename A, typename D, typename E>
      struct get_image_impl_helper : public set_image_impl<A, E>
      {
	// Implementation comes from internal::set_image_impl<A, E>.
      };

      /// \brief Abstraction factorization for the delegation mechanism.
      ///
      /// set_image_impl specializations should inherit from this
      /// class (as a public virtual derivation).
      template <typename E>
      struct image_impl	:
	public virtual oln::abstract::any_best_speed<E>
      {
	typedef oln_vtype(E, delegatee) D;
	D& delegate() { return this->exact().impl_delegate(); }
	const D& delegate() const { return this->exact().impl_delegate(); }
      };

    } // end of namespace oln::abstract::internal

  } // end of namespace oln::abstract

} // end of namespace oln


#endif // ! OLN_CORE_ABSTRACT_INTERNAL_IMAGE_IMPL_HH
