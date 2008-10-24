#include <iostream>
#include <mln/core/concept/meta_fun.hh>

using namespace mln;

meta::red red; // fun-obj



template <class T>
struct rgb
{
  T red() const { return r; }
  T& red() { return r; }
  T r;
};



template <class T>
struct fun< meta::red, rgb<T> > : Function_v2v< fun< meta::red, rgb<T> > >
{
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


/*

  morpher apply(M m, I ima)
  {
    M et value(I) -> F -> nature
  }

 */



int main()
{
  typedef rgb<int> C;
  C c;
  c.r = 51;

  int r = red(c);
  std::cout << r << std::endl;


  // norm(v) = 1;


  // norm( ima(p) ) = n;

  // norm( v& ) = n;
  // v = n * v / norm(v);

  // norm( v& ) = 1;



  // ima' = apply(norm, ima)
  // ima'(p) = 1;

}
