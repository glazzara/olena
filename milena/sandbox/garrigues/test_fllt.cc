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

//   int ws[81] = {3,2,3,3,5,5,5,5,5,
// 		2,1,3,4,4,4,4,5,5,
// 		2,3,4,2,3,3,2,4,4,
// 		1,4,2,1,1,2,1,2,2,
// 		1,2,4,2,1,2,1,1,1,
// 		1,3,3,3,4,3,2,5,1,
// 		1,3,4,3,4,3,2,5,1,
// 		1,3,3,3,4,3,2,5,1,
// 		1,3,3,4,2,3,2,1,1};

//   w_window2d_int w_win = make::w_window2d(ws);
//   image2d<int> ima = convert::to_image(w_win);
//   fllt::fllt(ima);


  for (int i = 0; i < 16; ++i)
    for (int j = 0; j < 16; ++j)
      {
	std::stringstream path;
	path << "/lrde/tegucigalpa/theo/pub/mln_docs/lena_tiles/lena_" << i << "_" << j << ".pgm";
	std::cout << path.str () << std::endl;
	image2d<value::int_u8> ima = io::pgm::load(path.str());
	image2d<int> ima_int(ima.domain());
	level::fill(ima_int, ima);
	debug::println(ima_int);
	fllt::fllt(ima_int);
      }

}
