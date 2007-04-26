#include <oln/core/2d/image2d.hh>
#include <oln/core/2d/neighb2d.hh>

#include <oln/morpho/cc_tarjan_v0.hh>

int main()
{
  using namespace oln;

  unsigned N = 128;

  image2d<bool> img(N, N);

  float tmp;

  for (unsigned i = 0; i < N; i++)
    for (unsigned j = 0; j < N; j++)
    {
      tmp = 5 * cos(i) * cos(j);
      img.at(i, j) = (tmp > 0) ? tmp : 0;
    }

  morpho::cc_tarjan(img + c4);
}
