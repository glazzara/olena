// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/// Test oln::neighb2d.

#include <oln/core/2d/image2d.hh>
#include <oln/core/2d/neighb2d.hh>


namespace test
{

  template <typename I>
  unsigned run(const oln::Image_with_Nbh<I>& input)
  {
    oln_piter(I) p(input.points());
    oln_niter(I) n(p, input.nbhood());
    unsigned count = 0;

    for_all(p)
    {
      for_all(n)
	if (input.owns_(n))
	  ++count;
    }
    return count;
  }
}


int main()
{
  using namespace oln;

  image2d<bool> ima(3, 3);
  // 2 + 3 + 2 +
  // 3 + 4 + 3 +
  // 2 + 3 + 2 = 24
  std::cout << test::run(ima + c4) << std::endl;
  assert(test::run(ima + c4) == 24);
}
