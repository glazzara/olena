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

#include <algorithm>

#include "my_lib/lib.hh"

#include "dyn-all.hh"

using namespace dyn::language;

int main()
{
  dyn::include_dir(DYN_FIXTURES);
  dyn::include("my_lib/lib.hh");

  echo("Test containers");
  std::vector<int> v(4);
  std::generate(v.begin(), v.end(), rand);
  std::vector<int>::iterator it;
  stl_each(v, it)
    echo(*it);
  it = v.begin();

  meth begin("begin");
  meth end("end");
  var dv = v;
  var dit = it;
  echo(dv[2]);
  v[2] = 51;
  echo(v[2]);
  echo(dv[2]);
  dv[2] = 42;
  int dv_2 = dv[2];
  echo(dv_2);
  assert(dv_2 == 42);
  std::cout << "v[0] => " << *dit << std::endl;
  var my_begin = *begin(dv);
  int my_begin_i = *begin(dv);
  int my_begin_i2 = my_begin;
  int ref_begin_i = *v.begin();
  int ref_begin_i2 = v[0];
  echo("my_begin: " << my_begin);
  echo("my_begin_i: " << my_begin_i);
  echo("my_begin_i2: " << my_begin_i2);
  echo("ref_begin_i: " << ref_begin_i);
  echo("ref_begin_i2: " << ref_begin_i2);
  assert(my_begin_i == ref_begin_i);
  const std::vector<int>::iterator& tmp1b(begin(dv));
  const std::vector<int>::iterator& tmp2b(v.begin());
  const std::vector<int>::iterator& tmp3b(v.begin());
  var tmp4b = v.begin();
  assert((bool)(tmp3b == tmp2b));
  echo("a: " << dv[4]);
  echo("b: " << *begin(dv));
  echo("c: " << *begin(v));
  echo("e: " << *tmp1b);
  echo("f: " << *tmp2b);
  assert((bool)(*tmp1b == *tmp2b));
  assert((bool)(tmp1b == tmp2b));
  assert((bool)(begin(dv) == v.begin()));
  assert((bool)(ref_begin_i2 == my_begin_i2));

  // end
  int my_end_i = *end(dv);
  int ref_end_i = *v.end();
  echo(my_end_i);
  echo(ref_end_i);
  assert(my_end_i == ref_end_i);
  const std::vector<int>::iterator& tmp1(end(dv));
  const std::vector<int>::iterator& tmp2(v.end());
  const std::vector<int>::iterator& tmp3(v.end());
  var tmp4 = v.end();
  assert((bool)(tmp3 == tmp2));

  echo("a: " << dv[4]);
  echo("b: " << *end(dv));
  echo("c: " << *end(v));
  echo("e: " << *tmp1);
  echo("f: " << *tmp2);
  assert((bool)(*tmp1 == *tmp2));
  assert((bool)(tmp1 == tmp2));
  assert((bool)(end(dv) == v.end()));
  int i = 0;
  // stl_each(v, i)
  for (dit = begin(dv); v.end() != dit && i < 20; ++dit, ++i)
    std::cout << i << ": " << *dit << std::endl;

  assert(i == 4);

  echo("exiting");
}
