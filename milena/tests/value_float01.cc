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

#include <iostream>
#include <mln/value/float01_8.hh>
#include <mln/value/float01_16.hh>



float fi(int x) { return 0.5; }
int ii(int x) { return 1; }

float fd(double x) { return 0.5; }
int id(double x) { return 1; }

int main()
{
  using namespace mln::value;

  float01_8  a(0.5);
  float01_16 b(0.5);

  assert(approx_equal(b,a));

  std::cout << b << std::endl;
  b = b + 0.2;
  std::cout << b << std::endl;
  b = b - 0.2;
  std::cout << b << std::endl;
  b = b * 1.5;
  std::cout << b << std::endl;
  b = b / 4.6;
  std::cout << b << std::endl;

  b = b / 3;
  std::cout << b << std::endl;
  b = b * 1;
  std::cout << b << std::endl;

  a = fi(a);
  a = ii(a);
  a = fd(a);
  a = id(a);

  b = a;
  a = b;
  b = 0.34;
  std::cout << b << std::endl;
  b = 0;
  std::cout << b << std::endl;
  b = 1;
  std::cout << b << std::endl;
}
