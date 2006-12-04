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

#ifndef OLN_CORE_ABSTRACT_IMAGE_HYBRID_CLASSICAL_HH
# define OLN_CORE_ABSTRACT_IMAGE_HYBRID_CLASSICAL_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/image/hierarchies.hh>

# include <oln/core/abstract/image/dimension/1d.hh>
# include <oln/core/abstract/image/dimension/2d.hh>
# include <oln/core/abstract/image/dimension/3d.hh>
# include <oln/core/abstract/image/bbox/hierarchy.hh>
# include <oln/core/abstract/image/point_wise_accessibility/hierarchy.hh>


/* Image dimension hierarchy (summary).


                                  /image<I>/
                                      ^
                                      |
             ,----------------+-------+--------+----------------.
             |                |                |                |
  /classical_1d_image1d<I>/   |     /classical_3d_image<I>/     |
             ^                |                ^                |
             |     /classical_2d_image1d<I>/   |       /classical_image<I>/
             |                ^                |                ^
             |                |                |                |
             |                |                |                |
  ,---------------------.     |      ,---------------------.    |
  | if type_of(I, grid) |     |      | if type_of(I, grid) |    |
  |     == grid1d       |     |      |      == grid3d      |    |
  `---------------------'     |      `---------------------'    |
             |                |                |                |
             |      ,---------------------.    |                |
             |      | if type_of(I, grid) |    |                |
             |      |      == grid2d      |    |                |
             |      `---------------------'    |                |
             |                |                |                |
             |                |                |                |
 ,------------------------------------------------------------------------.
 | if  (deduce_type_of(I, topo, is_point_wise_random_accessible) == true) |
 | and (deduce_type_of(I, topo, bbox) != not_found)                       |
 `------------------------------------------------------------------------'
             |                |                |                |
             o                o                o                o

                                      o 
                                      | 
                           (image `hybrid' selector)
                                      ^
                                      |
                                image_entry<I>
                                      ^
                                      |
                                      I
                              (a concrete image)


  Default case: If the image I does not meet any of these conditions, the
  entry is directly plugged to abstract::image<I>.  */


namespace oln
{

  namespace abstract
  {

    /*----------------------.
    | Hybrid abstractions.  |
    `----------------------*/

    template <typename E>
    struct classical_image
      : public virtual abstract::image_having_bbox<E>,
	public virtual abstract::image_being_point_wise_random_accessible<E>
    {
    protected:
      classical_image();
    };

    template <typename E>
    struct classical_1d_image
      : public virtual abstract::classical_image<E>,
	public virtual abstract::image1d<E>
    {
    protected:
      classical_1d_image();
    };

    template <typename E>
    struct classical_2d_image
      : public virtual abstract::classical_image<E>,
	public virtual abstract::image2d<E>
    {
    protected:
      classical_2d_image();
    };

    template <typename E>
    struct classical_3d_image
      : public virtual abstract::classical_image<E>,
	public virtual abstract::image3d<E>
    {
    protected:
      classical_3d_image();
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    classical_image<E>::classical_image()
    {
    }

    template <typename E>
    classical_1d_image<E>::classical_1d_image()
    {
    }

    template <typename E>
    classical_2d_image<E>::classical_2d_image()
    {
    }

    template <typename E>
    classical_3d_image<E>::classical_3d_image()
    {
    }

# endif

  } // end of namespace oln::abstract


  /*----------------.
  | Hybrid switch.  |
  `----------------*/

  // Forward declarations.
  template <unsigned D> struct grid_;
  typedef grid_<1> grid1d;
  typedef grid_<2> grid2d;
  typedef grid_<3> grid3d;


  /// 1D case.
  template <typename E>
  struct case_< image_hybrid_hierarchy_wrt_classical, E, 1 > :
    where_< mlc::and_list_< mlc::eq_< oln_vtype(E, grid), oln::grid1d >,
			    mlc::eq_< oln_deduce_vtype(E, topo, is_random_accessible), mlc::true_ >,
			    mlc::neq_< oln_deduce_vtype(E, topo, bbox), mlc::not_found >
                           >
           >
  {
    typedef abstract::classical_1d_image<E> ret;
  };


  /// 2D case.
  template <typename E>
  struct case_< image_hybrid_hierarchy_wrt_classical, E, 2 > :
    where_< mlc::and_list_< mlc::eq_< oln_vtype(E, grid), oln::grid2d >,
			    mlc::eq_< oln_deduce_vtype(E, topo, is_random_accessible), mlc::true_ >,
			    mlc::neq_< oln_deduce_vtype(E, topo, bbox), mlc::not_found >
                           >
           >
  {
    typedef abstract::classical_2d_image<E> ret;
  };


  /// 3D case.
  template <typename E>
  struct case_< image_hybrid_hierarchy_wrt_classical, E, 3 > :
    where_< mlc::and_list_< mlc::eq_< oln_vtype(E, grid), oln::grid3d >,
			    mlc::eq_< oln_deduce_vtype(E, topo, is_random_accessible), mlc::true_ >,
			    mlc::neq_< oln_deduce_vtype(E, topo, bbox), mlc::not_found >
                           >
           >
  {
    typedef abstract::classical_3d_image<E> ret;
  };


  /// General case.
  template <typename E>
  struct case_< image_hybrid_hierarchy_wrt_classical, E, 4 > :
    where_< mlc::and_< mlc::eq_< oln_deduce_vtype(E, topo, is_random_accessible), mlc::true_ >,
		       mlc::neq_< oln_deduce_vtype(E, topo, bbox), mlc::not_found >
                      >
           >
  {
    typedef abstract::classical_image<E> ret;
  };


} // end of namespace oln


#endif // ! OLN_CORE_ABSTRACT_IMAGE_HYBRID_CLASSICAL_HH
