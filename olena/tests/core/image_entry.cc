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

/// Test oln::image_entry.


#include <oln/basics2d.hh>
#include <oln/core/image_entry.hh>


namespace my
{
  // Forward declaration.
  struct image;

  // Dummy type.
  struct dummy;
}


namespace oln
{
  // Warning, this sugar might be removed in the future.
  stc_set_super(my::image, oln::image_entry<my::image>);

  /// Virtual types associated to my::image.
  /// \{
  template<>
  struct vtypes<my::image>
  {
    // The switch for image_dimension (above image_entry) plugs the
    // inheritance relation to the right abstract::image_dimension
    // class using the sole grid information (the grid can be seen
    // here as a ``tag'').
    typedef oln::grid2d grid_type;

    // Likewise, for the switch of image_typeness using the type of
    // the value of the image.
    typedef char value_type;

    // FIXME: Dummy values.
    typedef topo2d topo_type;
    typedef point2d point_type;
    typedef my::image concrete_type;
    typedef my::dummy morpher_type;
  };

  template<>
  struct single_vtype<my::image, typedef_::fwd_piter_type>
  {
    typedef fwd_piter2d ret;
  };

  template<>
  struct single_vtype<my::image, typedef_::bkd_piter_type>
  {
    typedef bkd_piter2d ret;
  };
  ///\}

} // end of namespace oln


namespace my
{
  // A very simple 2D image.
  class image : public oln::get_super_types<my::image>::ret
  {
  public:
    typedef image self_type;
    // Internal vtypes.
    typedef oln_type_of_(self_type, grid) grid_t;
    typedef oln_type_of_(self_type, value) value_t;
    // FIXME: Add missing typedefs.
  };
} // end of namespace my


int
main()
{
  // Check its internally defined vtypes.
  mlc::assert_< mlc_eq(my::image::grid_t, oln::grid2d) >::check();
  mlc::assert_< mlc_eq(my::image::value_t, char) >::check();

  // Check its image dimension abstraction.
  mlc::assert_< mlc_is_a_(my::image, oln::abstract::image2d) >::check();

  // Ensure we can instantiate it.
  my::image i;
}
