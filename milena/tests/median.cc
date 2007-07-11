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

/*! \file tests/median.cc
 *
 * \brief Tests on mln::value::median<S>.
 */

#include <cmath>

#include <mln/core/image2d_b.hh>
#include <mln/value/int_u.hh>

#include <mln/level/fill.hh>
#include <mln/level/median.hh>
#include <mln/debug/println.hh>

#include <mln/core/rectangle2d.hh>

#include <mln/io/load_pgm.hh>
#include <mln/io/save_pgm.hh>


using namespace mln;
using namespace mln::value;


int_u8 f(const point2d& p)
{
  return unsigned((2
		   + std::cos(float(p.row()))
		   + std::sin(float(p.col()))) * 63.9);
}


int main()
{


//   typedef set_<int_u8> S;

//   median<S> m(S::the());

//   {

//     unsigned vals[] = { 42, 69, 51, 12, 51, 12, 42 };
//     unsigned n = sizeof(vals)/sizeof(unsigned);

//     for (unsigned i = 0; i < n; ++i)
//       {
// 	std::cout << "taking " << vals[i] << ':' << std::endl;
// 	m.take(vals[i]);
// 	std::cout << m << std::endl;
//       }

//     for (int i = int(n) - 1; i >= 0; --i)
//       {
// 	std::cout << "untaking " << vals[i] << ':' << std::endl;
// 	m.untake(vals[i]);
// 	std::cout << m << std::endl;
//       }

//   }

  image2d_b<int_u8>
    lena = io::load_pgm("lena.pgm"),
    out(lena.domain());

  rectangle2d rec(64, 64);
  level::median(lena, rec, out);

  io::save_pgm(out, "out.pgm");
}




//   {

//     unsigned vals[] = { 42, 69, 51, 12, 51, 12, 42 };
//     unsigned n = sizeof(vals)/sizeof(unsigned);

//     for (unsigned i = 0; i < n; ++i)
//       {
// 	std::cout << "taking " << vals[i] << ':' << std::endl;
// 	m.take(vals[i]);
// 	std::cout << m << std::endl;
//       }

//     for (unsigned i = 0; i < n; ++i)
//       {
// 	std::cout << "untaking " << vals[i] << ':' << std::endl;
// 	m.untake(vals[i]);
// 	std::cout << m << std::endl;
//       }

//   }



//   {

//     unsigned vals[] = { 42, 42, 69, 69, 51, 51, 12, 12, 51, 51, 12, 12, 42, 42 };
//     unsigned n = sizeof(vals)/sizeof(unsigned);

//     for (unsigned i = 0; i < n; ++i)
//       {
// 	std::cout << "taking " << vals[i] << ':' << std::endl;
// 	m.take(vals[i]);
// 	std::cout << m << std::endl;
//       }

//     for (int i = int(n) - 1; i >= 0; --i)
//       {
// 	std::cout << "untaking " << vals[i] << ':' << std::endl;
// 	m.untake(vals[i]);
// 	std::cout << m << std::endl;
//       }

//   }


//   {

//     unsigned vals[] = { 42, 42, 69, 69, 51, 51, 12, 12, 51, 51, 12, 12, 42, 42 };
//     unsigned n = sizeof(vals)/sizeof(unsigned);

//     for (unsigned i = 0; i < n; ++i)
//       {
// 	std::cout << "taking " << vals[i] << ':' << std::endl;
// 	m.take(vals[i]);
// 	std::cout << m << std::endl;
//       }

//     for (unsigned i = 0; i < n; ++i)
//       {
// 	std::cout << "untaking " << vals[i] << ':' << std::endl;
// 	m.untake(vals[i]);
// 	std::cout << m << std::endl;
//       }

//   }




//   {

//     unsigned vals[] = { 42, 69, 51, 12, 51, 12, 42, 69, 51, 12, 51, 12, 42 };
//     unsigned n = sizeof(vals)/sizeof(unsigned);

//     for (unsigned i = 0; i < n; ++i)
//       {
// 	std::cout << "taking " << vals[i] << ':' << std::endl;
// 	m.take(vals[i]);
// 	std::cout << m << std::endl;
//       }

//     for (int i = int(n) - 1; i >= 0; --i)
//       {
// 	std::cout << "untaking " << vals[i] << ':' << std::endl;
// 	m.untake(vals[i]);
// 	std::cout << m << std::endl;
//       }

//   }


//   {

//     unsigned vals[] = { 42, 69, 51, 12, 51, 12, 42, 69, 51, 12, 51, 12, 42 };
//     unsigned n = sizeof(vals)/sizeof(unsigned);

//     for (unsigned i = 0; i < n; ++i)
//       {
// 	std::cout << "taking " << vals[i] << ':' << std::endl;
// 	m.take(vals[i]);
// 	std::cout << m << std::endl;
//       }

//     for (unsigned i = 0; i < n; ++i)
//       {
// 	std::cout << "untaking " << vals[i] << ':' << std::endl;
// 	m.untake(vals[i]);
// 	std::cout << m << std::endl;
//       }

//   }


