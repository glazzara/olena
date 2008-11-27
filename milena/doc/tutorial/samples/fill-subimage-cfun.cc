#include <mln/essential/2d.hh>
#include <doc/tutorial/tools/sample_utils.hh>
// \{
bool row_oddity(mln::point2d p)
{
  return p.row() % 2;
}
// \}
int main()
{
  using namespace mln;
  using value::rgb8;
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

  // Create a black image from ima.
  // Fill sites being part of component 2 with red.
  // \{
  image2d<rgb8> ima2;
  initialize(ima2, ima);
  level::fill(ima2, literal::black);

  level::fill((ima2 | row_oddity).rw(), literal::red);
  // \}

  doc::ppmsave(ima2, "fill-subimage-cfun");
}
