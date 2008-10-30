#include <iostream>

#include <mln/value/int_u.hh>
#include <mln/core/image/image2d.hh>
#include "co_occurence.hh"
#include <mln/core/alias/dpoint2d.hh>

int main ()
{
  using namespace mln;
  using namespace value;

  typedef image2d< int_u<3> >  I;

  int_u<3> vs[6][5] = {

    { 3, 3, 4, 4, 4 },
    { 2, 1, 1, 1, 1 },
    { 1, 4, 4, 4, 6 },
    { 1, 4, 3, 4, 1 },
    { 7, 4, 5, 3, 1 },
    { 7, 7, 1, 1, 0 }

  };

  I ima(make::image2d(vs));

  dpoint2d d(0, 1);

  //  std::cout << co_occurence(ima, d) << std::endl;

  image2d<unsigned> co(co_occurence(ima, d));

  unsigned cpt = 0;

  mln_piter_(image2d<unsigned>) p(co.domain());
  for_all(p)
  {
    std::cout << p << " = " << co(p) << std::endl;
    cpt += co(p);
  }

  std::cout << cpt << std::endl;

}
