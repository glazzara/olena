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

// FIXME: We should not include oln/basics2d.hh, but oln/core/2d/image2d.hh.
#include <oln/basics2d.hh>
#include <oln/morpher/identity.hh>
#include <oln/morpher/add_neighborhood.hh>

// FIXME: Fake!
// To be removed as soon as neighborhood2d is written.
namespace oln
{
  struct neighborhood2d {};
}

int
main()
{
  typedef oln::image2d<int> image_t;
  image_t ima (42, 51);

  typedef oln::morpher::add_neighborhood<image_t> image_with_nbh_t;
  // FIXME: Using `oln_type_of_(image_t, neighborhood)' seems
  // mandatory here.  In fact, we cannot use oln::neighborhood2d
  // directly like this
  //
  //   oln::neighborhood2d nbh;
  //
  // since it confuses the compiler (g++ 4.0)!
  typedef oln_type_of_(image_t, neighborhood) neighborhood_t;
  neighborhood_t nbh;
  image_with_nbh_t ima_with_nbh(ima, nbh);

  typedef oln::morpher::identity<image_with_nbh_t> image_with_nbh_id_t;
  image_with_nbh_id_t ima_with_nbh_id(ima_with_nbh);
}
