// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/*! \file tests/histo/compute.cc
 *
 * \brief Tests on mln::accu::histo and mln::histo::data.
 */

#include <iterator>
#include <sstream>

#include <mln/core/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_s.hh>

#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>
#include <mln/accu/histo.hh>
#include <mln/histo/compute.hh>


int main()
{
  using namespace mln;
  using value::int_u8;

//   // Test on 'bool'.
//   {
//     accu::histo< value::set<bool> > h;
    
//     for (unsigned i = 0; i < 5; ++i)
//       h.take(false);
//     for (unsigned i = 0; i < 2; ++i)
//       h.take(true);
//     h.untake(true);
    
//     mln_assertion(h[0] * 10 + h[1] == 51);
//     mln_assertion(h(false) * 10 + h(true) == 51);
//   }

//   // Test on 'int_u8'.
//   {
//     image2d<int_u8> ima(3, 3);
//     debug::iota(ima);
//     ima.at(0,0) = 2;

//     histo::data< value::set<int_u8> > h = histo::compute(ima);
//     std::ostringstream oss;
//     oss << h; 
//     mln_assertion(oss.str() == "2:2 3:1 4:1 5:1 6:1 7:1 8:1 9:1 ");

//     int_u8 i = 2;
//     mln_assertion(h(i) == 2);
//   }

  // Test on 'int_s5'.
  {
    typedef value::int_s<5> int_s5;
    image2d<int_s5> ima(3, 3);
    debug::iota(ima);
    ima.at(0,0) = 2;

    std::cout << "int_s5: "
	      << value::props< int_s5 >::min() << ' '
	      << value::props< int_s5 >::max() << ' '
	      << value::props< int_s5 >::card_ << std::endl;

    debug::println(ima);


    histo::data< value::set<int_s5> > h = histo::compute(ima);
    std::cout << "nvalues = " << h.vset().nvalues() << std::endl;
    std::cout << h << std::endl;

    for (unsigned i = 0; i < h.vset().nvalues(); ++i)
      std::cout << i << ':' << h[i] << ' ';
    std::cout << std::endl;
  }

}
