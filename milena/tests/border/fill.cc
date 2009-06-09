// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/border/fill.hh>
#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/debug/println_with_border.hh>
#include <mln/opt/element.hh>

using namespace mln;


void
check(int size, int border)
{
  int w = size + 2 * border;
  int ww = w *w;

  image2d<value::int_u8> ima(size, size, border);
  border::fill (ima, 42);
  for(int i = 0; i < ww; ++i)
    if ((i / w < border) || (i / w > border + size))
      mln_assertion (opt::element(ima, i) == 42u);
    else
      if ((i % w < border) &&
	  (border + size <= i % w))
	mln_assertion (opt::element(ima, i) == 42u);
}

int
main (void)
{
  for (int i = 1; i < 42; ++i)
    for (int j = 1; j < 42; ++j)
      check(i, j);
}
