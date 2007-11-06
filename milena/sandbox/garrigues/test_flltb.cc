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
# include <mln/io/pgm/save.hh>
# include <mln/io/pbm/load.hh>
# include <sstream>


int main()
{

  using namespace mln;
  using typename value::int_u8;

//   int vs[3][6] = { {0, 0, 0, 1, 1, 1},
// 		   {0, 1, 0, 1, 0, 1},
// 		   {0, 0, 0, 1, 1, 1} };

  int vs[11][5] = { {164, 164, 171, 171, 172},
		    {162, 164, 171, 171, 172},
		    {171, 164, 171, 171, 171},
		    {164, 171, 171, 164, 164},
		    {164, 171, 171, 171, 164},
		    {171, 164, 171, 164, 164},
		    {171, 171, 171, 164, 164},
		    {172, 171, 171, 164, 164},
		    {172, 164, 171, 162, 162},
		    {164, 171, 164, 156, 162},
		    {164, 164, 162, 162, 156} };



  image2d<int> ima(make::image2d(vs));
  image2d<int_u8> out(ima.domain());

  level::fill(out, ima);
  io::pgm::save(out, "ima.pgm ");
  fllt::fllt(ima);

}
