// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/*! \file tests/core/image/translate_image.cc
 *
 * \brief Tests on mln::core::translate_image.hh.
 */

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/debug/iota.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/border/fill.hh>
#include <mln/debug/println_with_border.hh>
#include <mln/debug/println.hh>
#include <mln/core/image/translate_image.hh>


int main ()
{
  using namespace mln;

  typedef image2d<value::int_u8> I;

  I ima (4, 2, 1);
  debug::iota (ima);
  translate_image<I> tmp (ima, dpoint2d (0,2));
  std::cout << "orginal image domain : "
	    << ima.domain ()
	    << std::endl
	    << "translated image domain : "
	    << tmp.domain ()
	    << std::endl;

  std::cout << "original image :"
	    << std::endl;
  debug::println (ima);
  std::cout << std::endl;
  std::cout << "translated image :"
	    << std::endl;
  debug::println (tmp);

  std::cout << std::endl;
  I out (4,4);
  data::paste(ima, out);
  data::paste(tmp, out);
  std::cout << "pasted image :"
	    << std::endl;
  debug::println (out);
  std::cout << std::endl;
}
