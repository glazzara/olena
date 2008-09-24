#include <iostream>

#include "access.hh"
#include <mln/value/all.hh>
#include <mln/core/image/image2d.hh>
#include <mln/fun/x2x/all.hh>
#include <mln/debug/iota.hh>
#include <mln/algebra/vec.hh>
#include <mln/io/ppm/all.hh>
#include <mln/border/adjust.hh>
#include <mln/border/fill.hh>
#include <mln/literal/all.hh>
#include <mln/core/image/interpolated.hh>

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
    mln::algebra::vec<2,float> tip = t.inv()(v);
    output(p) = interp(tip);
  }
}


template <typename I, typename O, typename T>
void
test2(const I& interp, O& output,
      const mln::Function_x2x<T>& t_)
{
  T t = exact(t_);

  mln_piter(I) p(output.domain());
  for_all(p)
  {
    mln::algebra::vec<2,float> v = mln::point2d(p);
    mln::algebra::vec<2,float> tip = t.inv()(v);
    output(p) = interp(tip);
  }
}


int main()
{
  using namespace mln;

  //initialization
  image2d< value::rgb<8> > input;
  io::ppm::load(input, "./lena.ppm");
  image2d< value::rgb<8> > output(input.domain());

  // adjust border
  border::adjust(input, 100);
  //border::fill(input, literal::black);

  interpolated< image2d< value::rgb<8> >, interpolation::bilinear>
    interp(input);

  //border::adjust(interp, 20);

  //transformation
  fun::x2x::translation<2,float> t(make::vec(20,20));
  fun::x2x::rotation<2,float> r(0.12, make::vec(0,1));

  test1<interpolation::bilinear>(input, output, compose(r,t));
  //test2(interp, output, compose(r,t));
  mln::io::ppm::save(output,"./out.ppm");
}
