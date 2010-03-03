#include <mln/core/image/image2d.hh>
#include <mln/io/pbm/save.hh>
#include <mln/make/image.hh>
int main()
{
  using namespace mln;

  bool vals[5][5] = {
    {1, 0, 1, 0, 0},
    {0, 1, 0, 1, 0},
    {1, 0, 1, 0, 0},
    {0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0}
  };
  image2d<bool> ima = make::image(vals);

  // \{
  /* FIXME: Hard-coded paths are bad!  Use something like
     milena/tests/data.hh.in instead.  */
  io::pbm::save(ima, "figures/ima_save.pbm");
  // \}
}
