#include <cassert>
#include <iostream>
#include <mln/core/image2d_b.hh>
#include <mln/core/window.hh>
#include <mln/level/fill.hh>
#include <mln/core/dpoints_pixter.hh>

int
main()
{
  using namespace mln;
  typedef image2d_b<int> I;


  I ima(20, 20);
  level::fill(ima, 5);
  I::piter f(ima.domain());
  window_<I::dpoint> set;

  set.insert(make::dpoint2d(0, -1));
  set.insert(make::dpoint2d(0, -1));
  set.insert(make::dpoint2d(1, 0));
  set.insert(make::dpoint2d(1, 0));

  for_all(f)
  {
    if (f[0] > 0 && f[1] > 0 && f[0] < 19 && f[1] < 19)
    {
      I::qixter dit(set, f, ima);

      for_all(dit)
      {
      	assert(*dit == 5);
      }
    }
  }
}
