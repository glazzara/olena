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

/*! \file tests/level/replace.cc
 *
 * \brief Tests on mln::level::replace.
 */

#include <mln/core/image2d.hh>
#include <mln/level/replace.hh>
#include <mln/level/compare.hh>


int main()
{
  using namespace mln;

  int vs[3][3] = {

    {  10, 3, 4  },
    {  5, 10, 7  },
    {  8, 9, 10 },

  };

  image2d<int> rhs = make::image2d(vs);

  level::replace(rhs, 10, 11);

  int vs_ref[3][3] = {

    {  11, 3, 4  },
    {  5, 11, 7  },
    {  8, 9, 11  },
  };

  mln_assertion(make::image2d(vs_ref) == rhs);

}
