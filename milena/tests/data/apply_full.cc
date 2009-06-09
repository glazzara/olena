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

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/int_s8.hh>
#include <mln/value/int_s16.hh>


#include <mln/core/routine/duplicate.hh>
#include <mln/data/apply.hh>
#include <mln/debug/iota.hh>

#include <mln/debug/println.hh>

struct qrde : mln::Function_v2v<qrde>
{
  typedef unsigned short result;
  result operator()(int c) const
  {
    return result(c % 42);
  }
};


namespace mln
{
  template <typename I>
  void
  chck(I& ref)
  {
    I out = duplicate (ref);

    mln_piter(I) p (ref.domain ());

    {
      data::apply(out, qrde());

      for_all(p)
	mln_assertion((mln_value(I))(ref(p) % 42) == out(p));
    }

  }

  template <typename I>
  void
  chk1d(unsigned cols)
  {
    image1d<I> ima (cols);
    debug::iota (ima);
    chck(ima);
  }

  template <typename I>
  void
  chk2d(unsigned rows,
	unsigned cols)
  {
    image2d<I> ima (rows, cols);
    debug::iota (ima);
    chck(ima);
  }

  template <typename I>
  void
  chk3d(unsigned slis,
	unsigned rows,
	unsigned cols)
  {
    image3d<I> ima (slis, rows, cols);
    debug::iota (ima);
    chck(ima);
  }

}


int main()
{
  using namespace mln;

  unsigned slis_start = 1;
  unsigned slis_end   = 3;

  unsigned rows_start = 1;
  unsigned rows_end   = 8;

  unsigned cols_start = 2;
  unsigned cols_end = 256;


  std::cerr << "Tests data::apply" << std::endl;

  (std::cerr << "in 1d ... ").flush ();
  {
    for (unsigned i = cols_start; i < cols_end; ++i)
      {
	chk1d<int>(i);
	chk1d<unsigned>(i);
	chk1d<value::int_u8>(i);
	chk1d<value::int_u16>(i);
	chk1d<value::int_s8>(i);
	chk1d<value::int_s16>(i);
      }
  }
  std::cerr << "OK" << std::endl;

  (std::cerr << "in 2d ... ").flush ();
  {
    for (unsigned h = rows_start; h < rows_end; ++h)
      for (unsigned i = cols_start; i < cols_end; ++i)
	{
	  chk2d<int>(h, i);
	  chk2d<unsigned>(h, i);
	  chk2d<value::int_u8>(h, i);
	  chk2d<value::int_u16>(h, i);
	  chk2d<value::int_s8>(h, i);
	  chk2d<value::int_s16>(h, i);
	}
  }
  std::cerr << "OK" << std::endl;


  (std::cerr << "in 3d ... ").flush ();
  {
    for (unsigned g = slis_start; g < slis_end; ++g)
      for (unsigned h = rows_start; h < rows_end; ++h)
	for (unsigned i = cols_start; i < cols_end; ++i)
	  {
	    chk3d<int>(g, h, i);
	    chk3d<unsigned>(g, h, i);
	    chk3d<value::int_u8>(g, h, i);
	    chk3d<value::int_u16>(g, h, i);
	    chk3d<value::int_s8>(g, h, i);
	    chk3d<value::int_s16>(g, h, i);
	  }
  }
  std::cerr << "OK" << std::endl;
}
