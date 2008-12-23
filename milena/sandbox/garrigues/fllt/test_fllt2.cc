# include "fllt.hh"
# include <mln/core/image/image2d.hh>
# include <mln/core/routine/clone.hh>
# include <mln/value/int_u8.hh>
# include <mln/debug/println.hh>
# include <mln/convert/to_w_window.hh>
# include <mln/core/alias/w_window2d_int.hh>
# include <mln/convert/to_image.hh>
# include <mln/data/fill.hh>
# include <mln/io/pgm/load.hh>
# include <mln/io/pbm/load.hh>

int main()
{

  using namespace mln;

  int ws[81] = {5,5,5,5,5,5,5,5,5,
		5,5,5,5,5,5,5,5,5,
		5,5,5,5,5,5,8,8,5,
	        5,5,1,1,1,5,8,8,5,
		5,5,1,1,1,5,8,8,5,
		5,5,1,1,1,5,8,8,5,
		5,5,5,5,5,5,8,8,5,
		5,5,5,5,5,5,5,5,5,
		5,5,5,5,5,5,5,5,5};

  w_window2d_int w_win = make::w_window2d(ws);
  image2d<int> ima = convert::to_image(w_win);

  fllt_tree(point2d, int) t = fllt::fllt(ima);
  fllt::draw_tree(ima, t);
}
