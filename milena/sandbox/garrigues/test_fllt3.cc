# include "fllt2.hh"
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

int main()
{

  using namespace mln;

  int ws[81] = {2,2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,2,
		2,1,1,1,1,1,1,1,2,
		2,1,2,2,1,0,0,1,2,
		2,1,2,2,1,0,0,1,2,
		2,1,2,2,1,0,0,1,2,
		2,1,1,1,1,1,1,1,2,
		2,1,1,1,1,1,1,1,2,
		2,2,2,2,2,2,2,2,2};

  w_window2d_int w_win = make::w_window2d(ws);
 image2d<int> ima = convert::to_image(w_win);
fllt::fllt(ima);


//    image2d<value::int_u8> ima = io::pgm::load("../../img/tiny.pgm");

//    image2d<int> ima_int(ima.domain());

// level::fill(ima_int, ima);
//   debug::println(ima);
//   fllt::fllt(ima_int);
}
