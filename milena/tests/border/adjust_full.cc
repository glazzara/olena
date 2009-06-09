// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

/*! \file
 *
 * \brief Tests on mln::border::adjust.
 */

#include <mln/core/image/image2d.hh>
#include <mln/border/get.hh>
#include <mln/border/adjust.hh>

int main()
{
  using namespace mln;

  typedef image2d<int> I;

  I ima(3, 3, 2);
  border::adjust(ima, 3);
  mln_assertion(border::get(ima) == 3);

  border::adjust(ima, 5);
  mln_assertion(border::get(ima) == 5);

  border::adjust(ima, 1);
  mln_assertion(border::get(ima) == 5);

  border::adjust(ima, 4);
  mln_assertion(border::get(ima) == 5);

  border::adjust(ima, 42);
  mln_assertion(border::get(ima) == 42);

  border::adjust(ima, 51);
  mln_assertion(border::get(ima) == 51);

  border::adjust(ima, 2);
  mln_assertion(border::get(ima) == 51);
}
