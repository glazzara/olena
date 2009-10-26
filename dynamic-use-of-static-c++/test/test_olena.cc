#include "dyn-all.hh"
using namespace dyn::language;

int main()
{
  dyn::include_dir(OLENA_PROTO_STL_STYLE); // FIXME: Move it into fixtures/olean-proto-stl-style
  // FIXME: Add
  dyn::include("all.hh");

  fun  iota("iota");
  fun  println_2d("println_2d");
  ctor mk_image2d_int("image2d<int>");

  var ima = mk_image2d_int(3, 3);

  println_2d(std::cout, "ima_before=", ima);
  iota(ima);
  println_2d(std::cout, "ima-after=", ima);

  std::cout << "exiting" << std::endl;
}
