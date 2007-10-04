#include <mln/core/image2d_b.hh>
#include <mln/core/sub_image.hh>
#include <mln/core/image_if_value.hh>
#include <mln/core/neighb2d.hh>
#include <mln/core/inplace.hh>

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
# include <mln/core/w_window2d_int.hh>
# include <mln/convert/to_window.hh>
# include <mln/core/concept/dpoint.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/window.hh>
# include <mln/pw/image.hh>
# include <mln/pw/cst.hh>
# include <mln/metal/is_a.hh>

# include <mln/core/image_if_interval.hh>

#include "chamfer.hh"


int main()
{
  using namespace mln;
  unsigned max = 51;

  image2d_b<bool> ima(9, 9);

  {
    level::fill(ima, false);
    ima.at(4,4) = true;
    const w_window2d_int& w_win = win_chamfer::mk_chamfer_3x3_int<2, 0> ();
    image2d_b<unsigned> out = chamfer(ima, w_win, max);
    debug::println(out | value::interval(0, 8));
  }

  {
    level::fill(ima, false);
    ima.at(4,4) = true;
    const w_window2d_int& w_win = win_chamfer::mk_chamfer_3x3_int<2, 3> ();
    image2d_b<unsigned> out = chamfer(ima, w_win, max);
    debug::println(out | value::interval(0, 8));
  }

  {
    level::fill(ima, false);
    ima.at(4,4) = true;
    const w_window2d_int& w_win = win_chamfer::mk_chamfer_5x5_int<4, 6, 9> ();
    image2d_b<unsigned> out = chamfer(ima, w_win, max);
    image2d_b<unsigned>::fwd_piter p(out.domain());
    for_all(p)
      out(p) = out(p) / 2;
    debug::println(out | value::interval(0, 8));
  }

}
