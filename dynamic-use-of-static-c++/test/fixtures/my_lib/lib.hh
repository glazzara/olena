// part of generic lib

#include <cmath>
#include <iostream>
#include <sstream> // Warning does not work without me
#include <vector>

template <class T>
struct u
{
  u(T& x) : x_(x) {}
  T& x_;
};

template <class T, class V>
struct t
{
  t<T, V>(const T& x, const V& y) : x_(x), y_(y) {}
  const T& x_, y_;
};

template <class T>
std::ostream& operator<< (std::ostream& ostr, const u<T>& x)
{
  return ostr << "u< " << x.x_ << " >";
}

template <class T>
std::istream& operator>> (std::istream& istr, u<T>& x)
{
  return istr >> x.x_;
}

template <class T, class V>
std::ostream& operator<< (std::ostream& ostr, const t<T, V>& x)
{
  return ostr << "t< " << x.x_ << ", " << x.y_ << " >";
}

void foo1()
{
  std::cout << "foo1()" << std::endl;
}

int* foo2()
{
  static int i = 42;
  std::cout << "foo2() => &42" << std::endl;
  return &i;
}

int foo2b()
{
  std::cout << "foo2b() => 42" << std::endl;
  return 42;
}

template <class T>
const T* foo3(T t1, const T t2, const T& t3)
{
  std::cout << "foo3(" << t1 << ", " << t2 << ", " << t3 << ")" << std::endl;
  return &t3;
}

namespace my_lib
{
  namespace x
  {
    template <template<class, class> class T, template<class> class U >
    U<char>*** foo4(U<float> a, T< T<char, char>, U<const int> > b)
    {
      std::cout << "foo4(" << a << ", " << b << ")" << std::endl;
    }
  } // end of x

  template <class T>
  T sqr(const T& t)
  {
    std::cout << "sqr(" << t << ") => " << t * t << std::endl;
    return t * t;
  }

  template <class T>
  float power(float f, const T& t)
  {
    std::cout << "power(" << f << "," << t << ")" << std::endl;
    return std::exp(t * std::log(f));
  }

  struct boo
  {
    boo(std::ostream& ostr) : ostr_(ostr) {}
    template <typename T1, typename T2>
    void print(const T1& a, const T2& b)
    {
      ostr_ << "<[" << a << ", " << b << "]>" << std::endl;
    }
    std::ostream& ostr_;
  };

#define stl_each(c, i) for (i = c.begin(); i != c.end(); ++i)

#define echo(i) std::cout << i << std::endl

} // end of my_lib
