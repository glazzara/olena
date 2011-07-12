// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

/// \file
/// \brief Exercise the fast (LUT-based) computation of 2D
/// connectivity numbers.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/make/box2d.hh>

#include <mln/topo/connectivity_number_2d.hh>

#include <mln/data/fill.hh>

#include <mln/logical/not.hh>


int
main()
{
  using namespace mln;

  //  Exercise the computation of 2D connectivity numbers using the
  //  look-up table.
  box2d b = make::box2d(-1, -1,
			+1, +1);
  image2d<bool> ima(b);
  data::fill(ima, false);

  point2d p(0, 0);
  mln_niter_(neighb2d) n(c4(), p);
  for_all(n)
    ima(n) = true;
  /*  Test case 1.
      ima = 

        - | -
        | - |
        - | -
  */
  mln_assertion(topo::connectivity_number_2d_c4(ima, p, true) == 4);
  mln_assertion(topo::connectivity_number_2d_c8(ima, p, true) == 1);

  logical::not_inplace(ima);
  ima.at_ (0, 0) = false;
  /*  Test case 2.
      ima = 

        | - |
        - - -
        | - |
  */
  mln_assertion(topo::connectivity_number_2d_c4(ima, p, true) == 0);
  mln_assertion(topo::connectivity_number_2d_c8(ima, p, true) == 4);


  /* FIXME: Also exercise the direct computation of connectivity
     numbers, using the number of connected components around the
     point in the foreground and the background.

     Possibly move this part in another test, especially if the
     corresponding routine is located in another file.  */
  // ...
}
