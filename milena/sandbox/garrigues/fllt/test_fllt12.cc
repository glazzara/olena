# include "fllt2.hh"
# include <mln/core/image/image2d.hh>
# include <mln/core/routine/clone.hh>
# include <mln/value/int_u8.hh>
# include <mln/debug/println.hh>
# include <mln/convert/to_w_window.hh>
# include <mln/core/alias/w_window2d_int.hh>
# include <mln/convert/to_image.hh>
# include <mln/level/fill.hh>
# include <mln/io/pgm/load.hh>
# include <mln/io/pgm/save.hh>
# include <mln/io/pbm/load.hh>
# include <sstream>


int main()
{

  using namespace mln;
  using typename value::int_u8;

  int vs[4][5] = { {4, 4, 2, 2, 2},
		   {4, 3, 1, 2, 2},
		   {4, 1, 1, 4, 2},
		   {4, 1, 1, 1, 2} };

  image2d<int> ima(make::image2d(vs));
  fllt::fllt(ima);
}
