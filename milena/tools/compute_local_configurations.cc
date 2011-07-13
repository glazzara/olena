// Copyright (C) 2008, 2009, 2011 EPITA Research and Development
// Laboratory (LRDE)
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

/// \brief Compute 2D connectivity numbers.

#include <iostream>
#include <iomanip>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>

#include <mln/labeling/blobs.hh>
#include <mln/data/fill.hh>
#include <mln/debug/println.hh>


int main()
{
  using namespace mln;
  using value::int_u8;

  typedef image2d<bool> I;

  box2d b = make::box2d(-1,-1, 1,1);
  image2d<bool> ima(b, 0);
  point2d p(0, 0);

  std::vector<int_u8> x8(256), x4(256);


  for (unsigned i = 0; i < 256; i++)
  {
    /* Create the local i-th configuration around P.

       Note that the value corresponding to P is always `false', to
       prevent the connection of two components through P.  */
    data::fill(ima, false);
    int_u8 tmp = i;
    mln_fwd_niter_(neighb2d) n(c8(), p);
    for_all(n)
    {
      // Inspect the least significant bit.
      if (tmp % 2)
	ima(n) = true;
      tmp = tmp >> 1;
    }

    labeling::blobs(ima, c8(), x8[i]);

    {
      int_u8 nl;
      image2d<int_u8> lab = labeling::blobs(ima, c4(), nl);
      std::set<int_u8> s;
      mln_fwd_niter_(neighb2d) n(c4(), p);
      for_all(n)
	if (lab(n) != 0)
	  s.insert(lab(n));
      x4[i] = s.size();
    }
  }


  // Now printing!

  std::cout << "----- Object in C8 ------" << std::endl;

  for (unsigned i = 0; i < 256; i++)
  {
    std::cout << std::setw(2) << x8[i] << ", ";
    if (! ((i + 1) % 4)) std::cout << " ";
    if (! ((i + 1) % 16)) std::cout << std::endl;
    if (! ((i + 1) % 64)) std::cout <<  std::endl;
  }

  std::cout << "----- Object in C4 ------" << std::endl;

  for (unsigned i = 0; i < 256; i++)
  {
    std::cout <<  std::setw(2) << x4[i] << ", ";
    if (! ((i + 1) % 4)) std::cout << " ";
    if (! ((i + 1) % 16)) std::cout << std::endl;
    if (! ((i + 1) % 64)) std::cout <<  std::endl;
  }

}
