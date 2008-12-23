#include <mln/core/image2d_b.hh>
#include <mln/core/image/sub_image.hh>
#include <mln/core/image_if_value.hh>


#include <mln/data/fill.hh>
# include <mln/debug/println.hh>
# include <mln/core/alias/w_window2d_int.hh>
# include <mln/core/alias/w_window2d_float.hh>
# include <mln/core/image_if_interval.hh>

# include <mln/opt/at.hh>

# include "win_chamfer.hh"
# include "chamfer.hh"

int main()
{
  using namespace mln;
  unsigned max = 51;

  image2d_b<bool> ima(9, 9);

  {
    data::fill(ima, false);
    opt::at(ima, 4,4) = true;
    const w_window2d_int& w_win = win_chamfer::mk_chamfer_3x3_int<2, 0> ();
    image2d_b<unsigned> out = geom::chamfer(ima, w_win, max);
    debug::println(out | value::interval(0, 8));
  }

  {
    data::fill(ima, false);
    opt::at(ima, 4,4) = true;
    const w_window2d_int& w_win = win_chamfer::mk_chamfer_3x3_int<2, 3> ();
    image2d_b<unsigned> out = geom::chamfer(ima, w_win, max);
    debug::println(out | value::interval(0, 8));
  }

  {
    data::fill(ima, false);
    opt::at(ima, 4,4) = true;
    const w_window2d_int& w_win = win_chamfer::mk_chamfer_5x5_int<4, 6, 9> ();
    image2d_b<unsigned> out = geom::chamfer(ima, w_win, max);
    image2d_b<unsigned>::fwd_piter p(out.domain());
    for_all(p)
      out(p) = out(p) / 2;
    debug::println(out | value::interval(0, 8));
  }

}
