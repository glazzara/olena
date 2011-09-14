// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

/*! \file tests/neighb/image.cc
 *
 * \brief Tests on mln::neighb::image.
 */

#include <mln/neighb/image.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/metal/has_neighborhood.hh>


int main()
{
  using namespace mln;

  typedef image2d<int> ima_t;
  typedef neighb2d nbh_t;

  typedef neighb::image<ima_t, nbh_t> ima_with_nbh_t;

  metal::has_neighborhood<ima_with_nbh_t>::check();

  ima_t ima (make::box2d(5, 5));

  // Explicit construction.
  ima_with_nbh_t ima1 (ima, c4());
  neighb::get(ima1);

  // Construction using operator+.
  ima_with_nbh_t ima2 = ima + c4();
  neighb::get(ima2);
}
