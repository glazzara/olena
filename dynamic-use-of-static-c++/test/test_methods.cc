#include "dyn-all.hh"
using namespace dyn::language;
#include <iostream>

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
  return 0;
}
