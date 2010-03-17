#include <mln/core/image/image2d.hh>
#include <mln/io/pbm/load.hh>

#include "doc/tools/data.hh"

int main()
{
  using namespace mln;
  /* FIXME: MLN_DOC_DIR looks probably confusing in the
     documentation...  But we have to handle parallel builds.  Maybe a
     solution could be to prepare the the build dir for data? */
  // \{
  image2d<bool> ima;
  io::pbm::load(ima, MLN_DOC_DIR "/img/small.pbm");
  // \}
}
