// Copyright (C) 2007, 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <cmath>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/sub_image.hh>

#include <mln/core/image/dmorph/image_if.hh>
#include <mln/fun/p2b/chess.hh>

#include <mln/literal/origin.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>

#include <mln/value/int_s8.hh>
#include <mln/value/int_s16.hh>

#include <mln/data/fill.hh>
#include <mln/data/transform.hh>

#include <mln/debug/iota.hh>

#include <mln/arith/plus.hh>




struct mysqrt : mln::Function_v2v<mysqrt>
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
  chck(const Image<I>& ref_)
  {
    const I& ref = exact(ref_);

    mln_ch_value(I, mln_result_(mysqrt)) out;
    {
      out = data::transform(ref, mysqrt());
      mln_piter(I) p (ref.domain ());
      for_all(p)
	mln_assertion ((mln_value(I))(ref(p) % 42) == out(p) );
    }
  }

  template <typename V>
  void
  chk()
  {
    unsigned sli = 4;
    unsigned row = 16;
    unsigned col = 64;


    (std::cerr << "in 1d ... ").flush ();
    {
      typedef image1d<V> I;
      typedef sub_image<I, box1d> J;

      for (unsigned i = 1; i < col; ++i)
	{
	  I ima(i);
	  debug::iota(ima);
	  chck (ima);
	}
    }
    std::cerr << "OK" << std::endl;

    (std::cerr << "in 2d ... ").flush ();
    {
      typedef image2d<V> I;

      for (unsigned i = 1; i < col; ++i)
	for (unsigned j = 1; j < row; ++j)
	  {
	    I ima(j, i);
	    debug::iota(ima);
	    chck (ima);
	  }
    }
    std::cerr << "OK" << std::endl;

    (std::cerr << "in 3d ... ").flush ();
    {
      typedef image3d<V> I;

      for (unsigned i = 1; i < col; ++i)
	for (unsigned j = 1; j < row; ++j)
	  for (unsigned k = 1; k < sli; ++k)
	    {
	      I ima(k, j, i);
	      debug::iota(ima);
	      chck (ima);
	    }
    }
    std::cerr << "OK" << std::endl;
  }

}





int main()
{
  using namespace mln;

  std::cerr << "Tests data::transform:" << std::endl;
  std::cerr << "on int:" << std::endl;
  chk<int>();
  std::cerr << "on unsigned:" << std::endl;
  chk<unsigned>();
  std::cerr << "on int_u8:" << std::endl;
  chk<value::int_u8>();
  std::cerr << "on int_u16:" << std::endl;
  chk<value::int_u16>();
  std::cerr << "on int_s8:" << std::endl;
  chk<value::int_s8>();
  std::cerr << "on int_s16:" << std::endl;
  chk<value::int_s16>();
}
