# include "fllt2.hh"
# include <mln/core/image/image2d.hh>
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

  int vs[9][9] = { {2,2,2,2,2,2,2,2,2},
		   {2,2,2,2,2,2,2,2,2},
		   {2,1,1,1,1,1,1,1,2},
		   {2,1,2,2,1,0,0,1,2},
		   {2,1,2,2,1,0,0,1,2},
		   {2,1,2,2,1,0,0,1,2},
		   {2,1,1,1,1,1,1,1,2},
		   {2,1,1,1,1,1,1,1,2},
		   {2,2,2,2,2,2,2,2,2} };

  image2d<int> ima(make::image2d(vs));
  fllt::fllt(ima);

}
