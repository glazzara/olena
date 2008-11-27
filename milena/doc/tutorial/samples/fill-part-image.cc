#include <mln/essential/2d.hh>
int main()
{
  using namespace mln;
  using value::int_u8;
  using value::label8;


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
  level::fill((ima | arr).rw(), 0);

  debug::println((ima | arr));

  mln_VAR(ima2, ima | arr);
  // We do not need to call "rw()" here.
  level::fill(ima2, 0);
  // \}

  debug::println(ima2);
}
