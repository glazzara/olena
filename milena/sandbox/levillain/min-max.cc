#include <iostream>

#include <mln/value/int_u8.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/accu/stat/min_max.hh>

#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>

#include <mln/core/var.hh>

int main()
{
  using namespace mln;
  using mln::value::int_u8;

  image2d<int_u8> ima(5, 5);
  debug::iota(ima);
  /* ima =

     1  2  3  4  5
     6  7  8  9  10
     11 12 13 14 15
     16 17 18 19 20
     21 22 23 24 25 */

  // A region delimited by a box (could also be something more complicated).
  point2d x(1, 1);
  point2d y(3, 4);
  box2d region(x, y);

  // These lines are not really portable, because of `mln_VAR'...
  mln_VAR(sub_ima, ima | region);
  mln_piter_(sub_ima_t) p(sub_ima.domain());
  /* ...and are a shortcut for this (portable) code:

     typedef sub_image< image2d<int_u8>, box2d > sub_ima_t;
     mln_piter_(sub_ima_t) p((ima | region).domain());

  */
  accu::stat::min_max<int_u8> accu;
  for_all(p)
    accu.take(ima(p));
  std::cout << "min = " << accu.first() << std::endl;
  std::cout << "max = " << accu.second() << std::endl;
}
