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

#include "my_lib/damien.hh"

#include "dyn-all.hh"

using namespace dyn::language;

namespace dyn
{
  fun down("mk_down");

  template <typename T>
  data mk_down(const T& t)
  {
    fun ctor(std::string("down< ") + mlc_name_of(t) + " >");
    return ctor(t);
  }
}

int main()
{
  dyn::include_dir(DYN_FIXTURES);
  dyn::include("my_lib/damien.hxx");

  down<char>               a('x');
  down<int>                b(10);
  std::string              bar("bar"); 
  down<std::string>        c(bar); // c("bar"); // it's worst
  down<down<std::string> > d(c);
  down<int>                e(20);

  var d2 = d;

  std::cout << d2 << std::endl;

  fun  dyn_foo("foo");
  fun  dyn_change("change");
  ctor mk_down_char("down<char>");
  ctor mk_down_int("down<int>");
  meth dyn_print_noarg("print_noarg");
  meth dyn_get_i("get_i");
  meth dyn_get_t("get_t");
  method dyn_clone("clone"); // Same as meth

  var f = mk_down_char('x');

  std::cout << mlc_name_of(f) << std::endl;

  var g = mk_down_int(44);
  var h = dyn::mk_down(e);

  var j = dyn::down(46);

  meth j_print_noarg = j.method("print_noarg");

  // std::cout is not printable
  // but a data containing std::cout yes
  var dyn_std_cout = std::cout;
  std::cout << dyn_std_cout << std::endl;

  for ( int i = 0; i < 5; ++i )
  {
    std::cout << "*** Turn " << i << " ***" << std::endl;
    dyn_print_noarg(a);

    dyn_foo(a);

    dyn_foo(b);
    dyn_foo(c);
    dyn_foo(d);
    dyn_foo(e);
    dyn_foo(42);

    dyn_print_noarg(f);

    dyn_foo(g);

    dyn_change(c);

    var x1 = dyn_get_i(b);
    std::cout << "dyn_get_i(b) => " << x1 << std::endl;
    var x2 = *dyn_get_t(d);
    std::cout << "dyn_get_t(d) => " << x2 << std::endl;

    var x3 = *dyn_clone(a);
    std::cout << "*clone(a) => " << x3 << std::endl;

    j.send("print_noarg");

    j.fake_method(a).send("print_noarg");
    h.fake_method(a).send("print_noarg");

    j_print_noarg();
  }

  std::cout << "exiting" << std::endl;
}
