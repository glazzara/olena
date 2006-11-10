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

#ifndef OLN_CORE_ABSTRACT_IMAGE_VALUE_WISE_ACCESSIBILITY_HIERARCHY_HH
# define OLN_CORE_ABSTRACT_IMAGE_VALUE_WISE_ACCESSIBILITY_HIERARCHY_HH

# include <mlc/logic.hh>
# include <mlc/cmp.hh>

# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/image/hierarchies.hh>
# include <oln/core/automatic/image/image_being_value_wise_random_accessible.hh>


namespace oln
{
 
  /*----------------------------------------.
  | Value-wise accessibility abstractions.  |
  `----------------------------------------*/

  namespace abstract
  {

    /// Image having a value-wise random accessibility.
    template <typename E>
    struct image_being_value_wise_random_accessible :
      public virtual image<E>,
      public automatic::get_impl< image_being_value_wise_random_accessible, E >
    {
      typedef oln_type_of(E, value) value_t;
      typedef oln_type_of(E, value_proxy) value_proxy_t;

    public:
      /// Get a (read-only) value descriptor from a value.
      ///
      /// This method is not really useful, but we keep it for the
      /// symmetry with
      /// mutable_image_being_value_wise_random_accessible::value().
      value_proxy_t value(const value_t& v) const;

    protected:
      /// Constructor (protected, empty).
      image_being_value_wise_random_accessible();
    };


    /// Mutable image having a value-wise random accessibility.
    template <typename E>
    struct mutable_image_being_value_wise_random_accessible :
      public virtual image_being_value_wise_random_accessible<E>,
      public automatic::get_impl< mutable_image_being_value_wise_random_accessible, E >
    {
      typedef oln_type_of(E, value) value_t;
      typedef oln_type_of(E, mutable_value_proxy) mutable_value_proxy_t;

    public:
      using image_being_value_wise_random_accessible<E>::value;
      /// Get a (read-only) value descriptor from a value.
      mutable_value_proxy_t value(const value_t& v);

    protected:
      /// Constructor (protected, empty).
      mutable_image_being_value_wise_random_accessible();
    };



# ifndef OLN_INCLUDE_ONLY

    // image_being_value_wise_random_accessible.

    template <typename E>
    typename image_being_value_wise_random_accessible<E>::value_proxy_t
    image_being_value_wise_random_accessible<E>::value(const typename image_being_value_wise_random_accessible<E>::value_t& v) const
    {
      return this->exact().impl_value(v);
    }

    template <typename E>
    image_being_value_wise_random_accessible<E>::image_being_value_wise_random_accessible()
    {
    }


    // mutable_image_being_value_wise_random_accessible.

    template <typename E>
    typename mutable_image_being_value_wise_random_accessible<E>::mutable_value_proxy_t
    mutable_image_being_value_wise_random_accessible<E>::value(const typename mutable_image_being_value_wise_random_accessible<E>::value_t& v)
    {
      return this->exact().impl_value(v);
    }

    template <typename E>
    mutable_image_being_value_wise_random_accessible<E>::mutable_image_being_value_wise_random_accessible()
    {
    }

# endif

  } // end of namespace oln::abstract


  /*----------------------------------.
  | Value-wise accessibility switch.  |
  `----------------------------------*/

  /* FIXME: Handle mutability.  This is not easy, as, for instance an
     image with LUT has no lvalue type, but can be perfectly const or
     mutable.  How can we deal with this?  By adding a specific
     vtype?  */

  /// With mutable value-wise random accessibility.
  template <typename E>
  struct case_< image_hierarchy_wrt_value_wise_accessibility, E, 1 > :
    where_< mlc_and( mlc_is_ok(oln_type_of(E, fwd_viter)),
		     mlc_eq( oln_type_of(E, is_value_wise_mutable),
			     mlc::true_) ) >
  {
    typedef abstract::mutable_image_being_value_wise_random_accessible<E> ret;
  };

  /// With (constant) value-wise random accessibility.
  template <typename E>
  struct case_< image_hierarchy_wrt_value_wise_accessibility, E, 2 > :
    where_< mlc_is_ok(oln_type_of(E, fwd_viter)) >
  {
    typedef abstract::image_being_value_wise_random_accessible<E> ret;
  };

} // end of namespace oln


#endif // ! OLN_CORE_ABSTRACT_IMAGE_VALUE_WISE_ACCESSIBILITY_HIERARCHY_HH
