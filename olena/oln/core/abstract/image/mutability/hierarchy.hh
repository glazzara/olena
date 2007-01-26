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

#ifndef OLN_CORE_ABSTRACT_IMAGE_MUTABILITY_HIERARCHY_HH
# define OLN_CORE_ABSTRACT_IMAGE_MUTABILITY_HIERARCHY_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/image/hierarchies.hh>
# include <oln/core/automatic/image/mutable_image.hh>



namespace oln
{
 
  /*-------------------------.
  | Mutability abstraction.  |
  `-------------------------*/

  namespace abstract
  {

    /// Image being mutable.
    template <typename E>
    struct mutable_image :
      public virtual image<E>,
      public automatic::get_impl<mutable_image, E>
    {
    public:

      struct decl
      {
	oln_virtual_typedef(lvalue);
      };

      /*! \brief Gives writable access to the value stored at \a p in
      ** the current image.
      */

      using image<E>::operator();

      oln_lvalue(E) operator()(const oln_psite(E)& p);

    protected:

      /// Constructor (protected, empty).
      mutable_image();

      /// Destructor (protected).
      ~mutable_image();

    };


# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    oln_lvalue(E)
    mutable_image<E>::operator()(const oln_psite(E)& p)
    {
      return this->exact().impl_op_readwrite(p);
    }

    template <typename E>
    mutable_image<E>::mutable_image()
    {
    }

    template <typename E>
    mutable_image<E>::~mutable_image()
    {
      decl();
    }

# endif

  } // end of namespace oln::abstract


  /*--------------------.
  | Mutability switch.  |
  `--------------------*/

  /// With mutability.
  template <typename E>
  struct case_< image_hierarchy_wrt_mutability, E, 1 > :
# ifdef OLENA_USE_NEW_SCOOP2
    where_< stc::is_found_< oln_find_vtype(E, lvalue) > >
# else
    where_< mlc::is_found_< oln_find_vtype(E, lvalue) > >
# endif // OLENA_USE_NEW_SCOOP2
  {
    typedef abstract::mutable_image<E> ret;
  };

} // end of namespace oln


#endif // ! OLN_CORE_ABSTRACT_IMAGE_MUTABILITY_HIERARCHY_HH
