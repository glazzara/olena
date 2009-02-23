#include <iostream>
#include <mln/core/image/image2d.hh>
#include "mean.hh"

int main()
{
  typedef mln::image2d<int> I;
  mln::morpho::attribute::mean<I> m;

  m.take (2);
  m.take (3);
  m.take (4);

  std::cout << m.to_result () << std::endl;

  m.set_value (6);
  m.take (3);
  m.take (9);

  std::cout << m.to_result () << std::endl;
}
