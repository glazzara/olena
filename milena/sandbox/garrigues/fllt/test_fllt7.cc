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
# include <mln/io/pgm/save.hh>
# include <mln/io/pbm/load.hh>
# include <sstream>


int main()
{

  using namespace mln;
  using typename value::int_u8;

  int ws[49] = {101, 101, 101, 191, 204, 255, 191,
                101, 101, 191, 204, 204, 204, 204,
                101, 191, 191, 204, 255, 204, 191,
                204, 204, 204, 191, 191, 191,  76,
                191, 191, 204, 191, 101, 101,  76,
                204, 204, 191, 204, 101,  76,  76,
                191, 191, 191, 101,  76, 101,   0};


  int vs[5][5] = { {100, 200, 200, 200, 100},
                   {100, 200, 255, 200, 100},
                   {100, 200, 200, 200, 100},
                   {100, 200, 255, 200, 100},
                   {100, 200, 200, 200, 100} };

  image2d<int> ima(make::image2d(vs));
  image2d<int_u8> out(ima.domain());

  level::fill(out, ima);
  io::pgm::save(out, "ima.pgm ");
  fllt::fllt(ima);

}
