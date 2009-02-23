// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

# include <mln/core/image/image2d.hh>
# include <mln/extract/green.hh>
# include <mln/value/rgb8.hh>
# include <mln/core/var.hh>

int main()
{
  using namespace mln;
  using value::rgb8;

  typedef image2d<rgb8> I;
  I ima(2,2);
  point2d p(1,1);
  ima(p) = value::rgb8(200, 230, 240);

  fun_image<meta::green<rgb8>,I> ima_green = extract::green(ima);

  mln_assertion(ima(p).green() == ima_green(p));
}

