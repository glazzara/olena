#include "dyn-all.hh"

using namespace dyn::language;

int main()
{
  dyn::include_dir(MILENA_DIR);
  dyn::include("mln/core/image/image2d.hh");
  dyn::include("mln/data/fill.hh");
  dyn::include("mln/debug/iota.hh");
  dyn::include("mln/debug/println.hh");

  ctor mk_image2d_int("mln::image2d<int>");
  fun fill("mln::data::fill");
  fun iota("mln::debug::iota");
  fun println("mln::debug::println");

  var ima = mk_image2d_int(3, 3);
  fill(ima, 0);
  println("ima (before) =", ima);
  iota(ima);
  println("ima (after) =", ima);
}
