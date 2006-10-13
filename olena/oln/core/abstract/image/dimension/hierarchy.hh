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

#ifndef OLN_CORE_ABSTRACT_IMAGE_DIMENSION_HIERARCHY_HH
# define OLN_CORE_ABSTRACT_IMAGE_DIMENSION_HIERARCHY_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/image/hierarchies.hh>


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
                         (image `dimension' selector)
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

# include <oln/core/abstract/image/dimension/1d.hh>
# include <oln/core/abstract/image/dimension/2d.hh>
# include <oln/core/abstract/image/dimension/3d.hh>


/*-------------------.
| Dimension switch.  |
`-------------------*/

namespace oln
{

  // Forward declarations.
  template <unsigned D> struct grid_;
  typedef grid_<1> grid1d;
  typedef grid_<2> grid2d;
  typedef grid_<3> grid3d;


  /// 1D case.
  template <typename E>
  struct case_< image_hierarchy_wrt_dimension, E, 1 > :
    where_< mlc::eq_< oln_type_of(E, grid), oln::grid1d > >
  {
    typedef abstract::image1d<E> ret;
  };

  /// 2D case.
  template <typename E>
  struct case_< image_hierarchy_wrt_dimension, E, 2 > :
    where_< mlc::eq_< oln_type_of(E, grid), oln::grid2d > >
  {
    typedef abstract::image2d<E> ret;
  };

  /// 3D case.
  template <typename E>
  struct case_< image_hierarchy_wrt_dimension, E, 3 > :
    where_< mlc::eq_< oln_type_of(E, grid), oln::grid3d > >
  {
    typedef abstract::image3d<E> ret;
  };

}


#endif // ! OLN_CORE_ABSTRACT_IMAGE_DIMENSION_HIERARCHY_HH
