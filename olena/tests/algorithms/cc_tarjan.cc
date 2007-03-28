#include <oln/core/2d/image2d.hh>
#include <oln/core/2d/window2d.hh>
#include <oln/core/2d/neighb2d.hh>

#include <oln/morpho/cc_tarjan.hh>

int main()
{
  using namespace oln;
  typedef image2d<bool> I;
  I ima(3, 3);

  ima.at(0, 0) = true;
  ima.at(0, 1) = true;
  ima.at(0, 2) = true;

  ima.at(2, 0) = true;
  ima.at(2, 1) = true;
  ima.at(2, 2) = true;

  image2d<unsigned int> out = (morpho::cc_tarjan(ima + c4)).image();

  for (unsigned i = 0; i < 3; i++)
    for (unsigned j = 0; j < 3; j++)
    {
      if (i == 0)
	assert(out.at(i, j) == 1);
      if (i == 1)
	assert(out.at(i, j) == 0);
      if (i == 2)
	assert(out.at(i, j) == 2);
    }
}
