// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <iostream>

#include <mln/core/image/image2d.hh>

#include <mln/make/image2d.hh>

#include <mln/value/label_8.hh>
#include <mln/value/int_u8.hh>

#include <mln/morpho/watershed/superpose.hh>
#include <mln/data/compare.hh>

#include "tests/data.hh"

#include <mln/debug/println.hh>
#include <mln/debug/iota.hh>

namespace mln
{

  struct ref_data : Function_v2v<ref_data>
  {
    typedef value::rgb8 result;

    value::rgb8 operator()(const point2d& p) const
    {
      if (p.row() == 2 || p.col() == 2)
	return literal::red;
      unsigned val = p.col() + 1 + p.row() * 4;
      return value::rgb8(val, val, val);
    }

  };

} // end of namespace mln

int main()
{
  using namespace mln;

  image2d<value::int_u8> ima(4,4);
  debug::iota(ima);

  value::label_8 data_ws[16] = { 1,  1,  0,  2,
				 1,  1,  0,  2,
				 0,  0,  0,  0,
				 5,  5,  0,  4 };

  image2d<value::label_8> ima_ws = make::image2d(data_ws);

  image2d<value::rgb8> sup = morpho::watershed::superpose(ima, ima_ws);
  mln_assertion(sup == (ref_data() | sup.domain()));
}
