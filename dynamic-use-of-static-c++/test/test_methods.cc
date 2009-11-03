/* Copyright (C) 2005, 2009 EPITA Research and Development Laboratory (LRDE)

   This file is part of Olena.

   Olena is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation, version 2 of the License.

   Olena is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Olena.  If not, see <http://www.gnu.org/licenses/>.

   As a special exception, you may use this file as part of a free
   software project without restriction.  Specifically, if other files
   instantiate templates or use macros or inline functions from this
   file, or you compile this file and link it with other files to produce
   an executable, this file does not by itself cause the resulting
   executable to be covered by the GNU General Public License.  This
   exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License.  */

#include <iostream>

#include "dyn-all.hh"

using namespace dyn::language;

int main()
{
  dyn::include_dir(DYN_FIXTURES);

  std::cout << "Test methods" << std::endl;
  ctor mk_down_int("down<int>", "my_lib/damien.hh");
  ctor mk_boo("my_lib::boo", "my_lib/lib.hh");
  meth print("print", "my_lib/lib.hh:my_lib/damien.hh");

  var v1 = mk_down_int(42);
  print(v1, std::cout);
  std::cout << std::endl;

  val v2 = mk_boo(std::cout);
  print(v2, 42, "foo");
  print(v2, "bar", 51);
  print(v2, 16, 64);
  print(v2, 2, "21");

  std::cout << "exiting" << std::endl;
}
