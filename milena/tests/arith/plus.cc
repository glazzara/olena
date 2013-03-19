// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/image/image2d.hh>
#include <mln/debug/iota.hh>
#include <mln/arith/plus.hh>
#include <mln/data/compare.hh>

int main()
{
  using namespace mln;

  // debug::trace::quiet = false;

  {
    image2d<int> ima(3,3);
    debug::iota(ima);

    int vs[3][3] = {
      { 4,  5,  6},
      { 7,  8,  9},
      {10, 11, 12}
    };

    ima += 2;
    image2d<int> ref(make::image(vs));

    mln_assertion (ima + 1 == ref);
  }

}



// #include <mln/core/image/image2d.hh>
// #include <mln/core/routine/duplicate.hh>
// #include <mln/value/int_u8.hh>

// #include <mln/arith/plus.hh>
// #include <mln/arith/times.hh>
// #include <mln/data/compare.hh>
// #include <mln/fun/v2v/cast.hh>

// #include <mln/debug/iota.hh>
// #include <mln/debug/println.hh>



// int main()
// {
//   using namespace mln;

//   // debug::trace::quiet = false;

//   {
//     image2d<int> ref(3,3);
//     debug::iota(ref);

//     image2d<int> ima_i = duplicate(ref);
//     ima_i += ima_i;
//     mln_assertion(ima_i == 2 * ref);

//     debug::println(ima_i);
//     ima_i += 1;
//     debug::println(ima_i);

//     image2d<float> ima_f(3,3);
//     debug::iota(ima_f);
//     debug::println(ima_i + ima_f);

//     point2d p(0, 0);
//     std::cout << arith::plus<float>(ima_i, ima_i)(p) / 5 << std::endl;
//   }

// }



// Bench:

//   {
//     unsigned size = 5000;
//     image2d<unsigned char> ima_uc(size, size);
//     image2d<unsigned> ima_u;
//     ima_u = 2u + ima_uc;
//   }
//   {
//     unsigned size = 5000;
//     image2d<value::int_u8> ima_uc(size, size);
//     image2d<unsigned> ima_u;
//     ima_u = 2u + ima_uc;
//   }

//   {
//     unsigned size = 5000;
//     image2d<unsigned char> ima_uc(size, size);
//     image2d<unsigned> ima_u;
//     ima_u = ima_uc + ima_uc;
//   }
//   {
//     unsigned size = 5000;
//     image2d<value::int_u8> ima_u8(size, size);
//     image2d<unsigned> ima_u;
//     ima_u = ima_u8 + ima_u8;
//   }
