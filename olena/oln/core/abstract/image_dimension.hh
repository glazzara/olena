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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301 USA
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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_DIMENSION_HH
# define OLENA_CORE_ABSTRACT_IMAGE_DIMENSION_HH

# include <oln/core/abstract/image.hh>

/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {

  /*! \namespace oln::abstract
  ** \brief oln::abstract namespace.
  */
  namespace abstract {

    /*! \class abstract::image1d<E>
    **
    ** Class of 1d images.
    */
    template <typename E>
    struct image1d : public virtual image<E>
    {
    protected:

      /*! \brief Constructor (protected, empty).
      */
      image1d() {}
    };

    /*! \class abstract::image2d<E>
    **
    ** Class of 2d images.
    */
    template <typename E>
    struct image2d : public virtual image<E>
    {
    protected:

      /*! \brief Constructor (protected, empty).
      */
      image2d() {}
    };
 
    /*! \class abstract::image3d<E>
    **
    ** Class of 3d images.
    */
    template <typename E>
    struct image3d : public virtual image<E>
    {
    protected:

      /*! \brief Constructor (protected, empty).
      */
      image3d() {}
    };
 
 
  } // end of namespace oln::abstract

} // end of namespace oln


// Register the dimension switch for oln::abstract::image_entry.
namespace stc
{
  template <typename E>
  struct set_entry_node<E, oln::abstract::dimension_tag> :
    // FIXME: Don't use the abstraction as a property, but the
    // corresponding grid instead.
    public oln_type_of_(E, image_dimension)
      ::template instantiated_with<E>::ret
  {
  };
}


#endif // ! OLENA_CORE_ABSTRACT_IMAGE_DIMENSION_HH
