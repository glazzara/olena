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


  /*-------------------.
  | Dimension switch.  |
  `-------------------*/

  /// Forward declarations.
  /// \{
  class grid1d;
  class grid2d;
  class grid3d;
  /// \}
  
  /// Case tag for the dimension.
  struct grid_dim_tag;
  
  /// Switch on on the grid dimension.
  /// \{
  template <typename grid_type>
  struct case_<grid_dim_tag, grid_type, 1> :
    // Test.
    public mlc::where_< mlc_eq(grid_type, oln::grid1d) >
  {
    // Super class if test succeeds.
    typedef stc::abstraction_as_type<abstract::image1d> ret;
  };

  template <typename grid_type>
  struct case_<grid_dim_tag, grid_type, 2> :
    // Test.
    public mlc::where_< mlc_eq(grid_type, oln::grid2d) >
  {
    // Super class if test succeeds.
    typedef stc::abstraction_as_type<abstract::image2d> ret;
  };

  template <typename grid_type>
  struct case_<grid_dim_tag, grid_type, 3> :
    // Test.
    public mlc::where_< mlc_eq(grid_type, oln::grid3d) >
  {
    // Super class if test succeeds.
    typedef stc::abstraction_as_type<abstract::image3d> ret;
  };

  /// Abort when grid_type is not handled by the previous cases.
  template <typename grid_type>
  struct default_case_<grid_dim_tag, grid_type>
  {
    typedef mlc::abort_<grid_dim_tag> ret;
  };
  /// \}

} // end of namespace oln


// Register the dimension switch for oln::abstract::image_entry.
namespace stc
{
  template <typename I>
  struct set_entry_node<I, oln::abstract::dimension_tag> :
    public oln::switch_< oln::grid_dim_tag, oln_type_of(I, grid) >::ret
      ::template instantiated_with<I>::ret
  {
  };
} // end of namespace stc

namespace oln
{
  /// An external type associated to my::B.
  template <typename I>
  struct ext_vtype< category::image,
		    abstract::image<I>,
		    // FIXME: Get rid of this typedef_:: qualifier.
		    typedef_::image_dimension_type >
  {
    // Use the same switch as the one use in the inheritance-plugging
    // mechanism above.
    typedef typename oln::switch_< oln::grid_dim_tag,
				   oln_type_of(I, grid) >::ret ret;
  };
}

#endif // ! OLENA_CORE_ABSTRACT_IMAGE_DIMENSION_HH
