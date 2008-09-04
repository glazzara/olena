#include <mln/core/image2d_b.hh>
#include <mln/core/image/sub_image.hh>
#include <mln/core/neighb2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/level/fill.hh>
#include <mln/level/stretch.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pgm/save.hh>
#include <mln/core/mesh_image.hh>
# include <mln/labeling/base.hh>
# include <mln/debug/println.hh>
# include <mln/core/window2d.hh>
# include <mln/convert/to_window.hh>
# include <mln/core/concept/dpoint.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/window.hh>
# include <mln/pw/image.hh>
# include <mln/pw/cst.hh>
# include <mln/metal/is_a.hh>

using namespace mln;

namespace mln
{

}

int
main (void)
{
  image2d_b<int_u8> in = io::pbm::load("l.pgm");
  util::graph<void> g;

  mln_bkd_pixter(const image2d_b<int_u8>) p(ima.input);


}
