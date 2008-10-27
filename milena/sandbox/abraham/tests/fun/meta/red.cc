#include <mln/fun/meta/red.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/thru.hh>

namespace mln
{

  template <class T>
  struct rgb
  {
    T red() const { return r; }
    T& red() { return r; }
    T r;
  };

  template <class T>
  struct function< meta::red< rgb<T> > > : public Function_v2w_w2v<function< meta::red < rgb<T> > > >
  {
    typedef rgb<T> value;

    typedef T result;
    T read(const rgb<T>& c)
    {
      std::cout << "read red rgb<T>" << std::endl;
      return c.red();
    }

    typedef T& lresult;
    T& write(rgb<T>& c)
    {
      std::cout << "write red rgb<T>" << std::endl;
      return c.red();
    }
  };
}

int main ()
{
  typedef mln::rgb<int> C;
  mln::image2d<C> i(3, 2, 0);
  C c;
  c.r = 51;
  i(mln::point2d(0,0)) = c;
  c.r = 23;
  i(mln::point2d(0,1)) = c;
  c.r = 43;
  i(mln::point2d(1,0)) = c;
  c.r = 0;
  i(mln::point2d(1,1)) = c;
  c.r = 65;
  i(mln::point2d(2,0)) = c;
  c.r = 1;
  i(mln::point2d(2,1)) = c;

  mln::thru<mln::meta::red <C>, mln::image2d<C> > out(i);

  mln_piter_(mln::image2d<C>) p(i.domain());

  for_all (p)
    std::cout << out(p) << std::endl;


}

