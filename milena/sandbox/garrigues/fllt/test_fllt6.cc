# include "fllt.hh"
# include <mln/core/image2d.hh>
# include <mln/core/clone.hh>
# include <mln/value/int_u8.hh>
# include <mln/debug/println.hh>
# include <mln/convert/to_w_window.hh>
# include <mln/core/w_window2d_int.hh>
# include <mln/convert/to_image.hh>
# include <mln/level/fill.hh>
# include <mln/io/pgm/load.hh>
# include <mln/io/pbm/load.hh>
# include <sstream>


int main()
{

  using namespace mln;

  int ws[25] = {1,1,1,  1,1,
		1,255,1,  1,  1,
		1,1,1,1,  1,
		1,1,1,  255,  1,
		1,1,1,  1,  1};
    w_window2d_int w_win = make::w_window2d(ws);
    image2d<int> ima = convert::to_image(w_win);
    fllt::fllt(ima);
}
