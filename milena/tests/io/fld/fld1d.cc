// Copyright (C) 2008, 2009, 2013 EPITA Research and Development
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

///
/// \brief Test AVS field file IO with 1D image.
///
///
///

#include <mln/core/image/image1d.hh>
#include <mln/io/fld/load.hh>
#include <mln/io/fld/save.hh>
#include <mln/value/int_u8.hh>
#include <mln/debug/iota.hh>
#include <mln/data/compare.hh>

int main()
{
  using namespace mln;
  using value::int_u8;


   image1d<int_u8> ori, test;
   box<point1d> b(9);
   ori.init_(b);
   debug::iota(ori);

   io::fld::save(ori, "out1d.fld");
   io::fld::load(test, "out1d.fld");

   // Clean output.
   std::remove("out1d.fld");

   mln_assertion(ori == test);
}
