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
  return 0;
}
