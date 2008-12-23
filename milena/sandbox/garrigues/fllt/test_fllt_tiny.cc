# include "fllt2.hh"
# include <mln/core/image/image2d.hh>
# include <mln/core/routine/clone.hh>
# include <mln/value/int_u8.hh>
# include <mln/data/fill.hh>
# include <mln/io/pgm/load.hh>

int main()
{

  using namespace mln;

   image2d<value::int_u8> ima = io::pgm::load("../../img/tiny.pgm");

   image2d<int> ima_int(ima.domain());

   data::fill(ima_int, ima);
   fllt::fllt(ima_int);
}
