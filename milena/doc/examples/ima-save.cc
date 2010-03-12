#include <mln/core/image/image2d.hh>
#include <mln/io/pbm/save.hh>
#include <mln/make/image.hh>

#include "doc/tools/data.hh"

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
  io::pbm::save(ima, MLN_DOC_DIR "/figures/ima_save.pbm");
  // \}
}
