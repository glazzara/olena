#include <oln/core/2d/image2d.hh>
#include <oln/core/2d/window2d.hh>
#include <oln/core/2d/neighb2d.hh>

#include <oln/morpho/cc_tarjan.hh>

#include <oln/debug/print.hh>


template <typename I>
void set(I& ima,
	 int i,
	 int j)
{
  oln_point(I) p(i, j);
  ima(p) = true;
}

int main()
{
  using namespace oln;
  typedef image2d<bool> I;
  I ima(3, 3);

  set(ima, 0, 0);
  set(ima, 0, 1);
  set(ima, 0, 2);

  set(ima, 2, 0);
  set(ima, 2, 1);
  set(ima, 2, 2);

  I out = morpho::cc_tarjan(ima + c4);

//   for (unsigned i; i < 3; i++)
//     for (unsigned j; j < 3; j++)
//     {
//       if (i == 0)
// 	assert(ima(i, j) == 1);
//       if (i == 1)
// 	assert(ima(i, j) == 0);
//       if (i == 2)
// 	assert(ima(i, j) == 2);
//     }
}
