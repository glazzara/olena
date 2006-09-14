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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_DIMENSION_HH
# define OLENA_CORE_ABSTRACT_IMAGE_DIMENSION_HH

# include <oln/core/abstract/image.hh>


/* Image dimension hierarchy (summary).


                                  /image<I>/
                                      ^
                                      |
             ,------------------------+------------------------.
             |                        |                        |
        /image1d<I>/             /image2d<I>/             /image3d<I>/
             ^                        ^                        ^
             |                        |                        |    
  ,---------------------.  ,---------------------.  ,---------------------.
  | if type_of(I, grid) |  | if type_of(I, grid) |  | if type_of(I, grid) | 
  |     == grid1d       |  |      == grid2d      |  |      == grid3d      |
  `---------------------'  `---------------------'  `---------------------'
             |                        |                        |    
             o                        o                        o

                                      o 
                                      | 
                 /switch_<image_dimension_hierarchy, I>::ret/
                          (image dimension selector)
                                      ^
                                      |
                          /entry<abstract::image, I>/
                                      ^
                                      |
                                image_entry<I>
                                      ^
                                      |
                                      I
                              (a concrete image)


  Default case: If no known grid type is returned by `oln_type_of(I, grid)',
  the entry is directly plugged to abstract::image<I>.  */


/*-------------------------.
| Dimension abstractions.  |
`-------------------------*/

namespace oln
{
 
  namespace abstract
  {

    /// Class of 1-D images.
    template <typename E>
    struct image1d : public virtual image<E>
    {
    protected:
      /// Constructor (protected, empty).
      image1d() {}
    };

    /// Class of 2-D images.
    template <typename E>
    struct image2d : public virtual image<E>
    {
    protected:
      /// Constructor (protected, empty).
      image2d() {}
    };
 
    /// Class of 3-D images.
    template <typename E>
    struct image3d : public virtual image<E>
    {
    protected:
      /// Constructor (protected, empty).
      image3d() {}
    };
 
  } // end of namespace oln::abstract

} // end of namespace oln


/*-------------------.
| Dimension switch.  |
`-------------------*/

namespace oln
{

  // Forward declarations.
  class grid1d;
  class grid2d;
  class grid3d;

  /// 1-D case.
  template <typename E>
  struct case_< image_dimension_hierarchy, E, 1 > :
    where_< mlc::eq_< oln_type_of(E, grid), oln::grid1d > >
  {
    typedef abstract::image1d<E> ret;
  };

  /// 2-D case.
  template <typename E>
  struct case_< image_dimension_hierarchy, E, 2 > :
    where_< mlc::eq_< oln_type_of(E, grid), oln::grid2d > >
  {
    typedef abstract::image2d<E> ret;
  };

  /// 3-D case.
  template <typename E>
  struct case_< image_dimension_hierarchy, E, 3 > :
    where_< mlc::eq_< oln_type_of(E, grid), oln::grid3d > >
  {
    typedef abstract::image3d<E> ret;
  };

}


#endif // ! OLENA_CORE_ABSTRACT_IMAGE_DIMENSION_HH
