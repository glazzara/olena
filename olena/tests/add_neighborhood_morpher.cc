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

/// Test the identity morpher.

#include <mlc/assert.hh>
#include <mlc/is_a.hh>

// FIXME: We should not include oln/basics2d.hh, but only
// oln/core/2d/image2d.hh.
#include <oln/basics2d.hh>
#include <oln/morpher/add_neighborhood.hh>

int
main()
{
  /*----------------.
  | image2d<char>.  |
  `----------------*/

  typedef oln::image2d<char> image_t;

  // Sanity check: interfaces realized by oln::image2d.
  mlc::assert_< mlc_is_a_(image_t, oln::abstract::image2d) >::check();
  mlc::assert_< mlc_is_a_(image_t,
			  oln::abstract::grey_level_image) >::check();

  image_t ima(42, 51);


  /*------------------------------------.
  | add_neighborhood< image2d<char> >.  |
  `------------------------------------*/

  typedef oln::neighb2d neighb_t;
  typedef oln::morpher::add_neighborhood<image_t, neighb_t> image_with_nbh_t;

  // Check that the instantiated add_neighborhood morpher realizes the
  // same interfaces as the underlying morphed image.
  mlc::assert_< mlc_is_a_(image_with_nbh_t, oln::abstract::image2d) >::check();
  mlc::assert_< mlc_is_a_(image_with_nbh_t,
			  oln::abstract::grey_level_image) >::check();
  // Check the type of neighborhood.
  mlc::assert_< mlc_eq(oln_type_of_(image_with_nbh_t, neighborhood),
		       oln::neighb2d) >::check();

  // Instantiate a neighborhood for this image type.
  oln_type_of_(image_with_nbh_t, neighborhood) nbh;
  // Instantiate an object, and check its methods.
  image_with_nbh_t ima_with_nbh(ima, nbh);
  oln::neighb2d nbh2 = ima_with_nbh.neighborhood();
}
