# include "fllt.hh"
# include <mln/core/image/image2d.hh>
# include <mln/core/routine/clone.hh>
# include <mln/value/int_u8.hh>
# include <mln/debug/println.hh>
# include <mln/convert/to_w_window.hh>
# include <mln/core/alias/w_window2d_int.hh>
# include <mln/convert/to_image.hh>
# include <mln/level/fill.hh>
# include <mln/io/pgm/load.hh>
# include <mln/io/pbm/load.hh>

int main()
{

  using namespace mln;

   image2d<value::int_u8> ima = io::pgm::load("../../img/lena.pgm");

   image2d<int> ima_int(ima.domain());

   level::fill(ima_int, ima);
   fllt::fllt(ima_int);
}
