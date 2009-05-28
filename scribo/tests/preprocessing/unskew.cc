#include <mln/core/image/image2d.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>

#include <scribo/preprocessing/unskew.hh>


int main(int argc, char *argv[])
{
  (void) argc;
  using namespace mln;

  image2d<bool> ima;
  io::pbm::load(ima, argv[1]);

  io::pbm::save(scribo::preprocessing::unskew(ima), "tr.pbm");
}
