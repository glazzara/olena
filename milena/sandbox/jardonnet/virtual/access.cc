#include <iostream>

#include "access.hh"
#include <mln/value/all.hh>
#include <mln/core/image/image2d.hh>
#include <mln/fun/x2x/all.hh>
#include <mln/debug/iota.hh>
#include <mln/algebra/vec.hh>
#include <mln/io/pgm/all.hh>

template < template <class> class N,
           typename I, typename T>
void
test1(const I& input, I& output,
      const mln::Function_x2x<T>& t_)
{
  N<I> interp(input);
  T t = exact(t_);
  
  mln_piter(I) p(output.domain());
  for_all(p)
  {
    mln::algebra::vec<2,float> v = mln::point2d(p);
    output(p) = interp(t.inv()(v));
  }
}


int main()
{
  using namespace mln;

  //initialization
  image2d< value::rgb<8> > input(512,512,100);
  mln::io::pgm::load(input, "./lena.pgm");
  image2d< value::rgb<8> > output(512,512);
  
  //transformation
  fun::x2x::translation<2,float> t(make::vec(3,4));
  fun::x2x::rotation<2,float> r(1.57, make::vec(0,1));

  test1<interpolation::nearest_neighbor>(input, output, t);
  mln::io::pgm::save(output,"./out.pgm");
}
  
