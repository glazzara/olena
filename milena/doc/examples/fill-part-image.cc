#include <mln/core/image/image2d.hh>
#include <mln/core/var.hh>
#include <mln/core/site_set/p_array.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>

#include <mln/make/image.hh>

#include <mln/data/fill.hh>

#include <mln/debug/println.hh>

int main()
{
  using namespace mln;
  using value::int_u8;
  using value::label_8;


  // \{
  bool vals[6][5] = {
      {0, 1, 1, 0, 0},
      {0, 1, 1, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 1, 0, 1, 0},
      {1, 0, 1, 1, 1},
      {1, 0, 0, 0, 0}
  };
  image2d<bool> ima = make::image(vals);
  // \}

  // \{
  p_array<point2d> arr;

  // We add two points in the array.
  arr.append(point2d(0, 1));
  arr.append(point2d(4, 0));

  // We restrict the image to the sites
  // contained in arr and fill these ones
  // with 0.
  // We must call "rw()" here.
  data::fill((ima | arr).rw(), 0);

  debug::println((ima | arr));

  mln_VAR(ima2, ima | arr);
  // We do not need to call "rw()" here.
  data::fill(ima2, 0);
  // \}

  debug::println(ima2);
}
