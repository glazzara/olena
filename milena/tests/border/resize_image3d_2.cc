// Copyright (C) 2007 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/image/image3d.hh>
#include <mln/debug/iota.hh>
#include <mln/value/int_u8.hh>
#include <mln/border/resize.hh>
#include <mln/debug/println_with_border.hh>
#include <mln/border/fill.hh>
#include <mln/data/fill.hh>

using namespace mln;

int
main (void)
{
  unsigned border = 2;
  unsigned new_border = 4;

  std::cout << std::endl
	    << "Test 3d resize"
	    << std::endl
	    << std::endl;
  image3d<int> ima(2, 3, 2, border);
  data::fill (ima, 2);
  border::fill(ima, 8);

  std::cout << "before resize ("
	    << border
	    << ")"
	    << std::endl;
  debug::println_with_border(ima);
  std::cout << std::endl;


  border::resize (ima, new_border);
  std::cout << "after resize ("
	    << new_border
	    << ")"
	    << std::endl;
  debug::println_with_border(ima);
  std::cout << std::endl;
}
