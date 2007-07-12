#include <cassert>
#include <iostream>
#include <mln/core/image2d.hh>
#include <mln/core/internal/set_of.hh>
#include <mln/level/fill.hh>
#include <mln/core/dpoints_pixter.hh>

int
main()
{
  using namespace mln;
  typedef image2d<int> I;


  I ima(20, 20);
  level::fill(ima, 5);
  I::piter f(ima.domain());
  internal::set_of_<I::dpoint> set;

  set.insert(mk_dpoint2d(-1, -1));
  set.insert(mk_dpoint2d(0, -1));
  set.insert(mk_dpoint2d(1, 0));
  set.insert(mk_dpoint2d(1, 1));

  for_all(f)
  {
    I::qixter dit(set, f, ima);

    if (f[0] > 0 && f[1] > 0 && f[0] < 19 && f[1] < 19)
      for_all(dit)
      {
      	assert(*dit == 5);
      }
  }
}
