/* Copyright (C) 2005, 2009 EPITA Research and Development Laboratory (LRDE)

   This file is part of Olena.

   Olena is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation, version 2 of the License.

   Olena is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Olena.  If not, see <http://www.gnu.org/licenses/>.

   As a special exception, you may use this file as part of a free
   software project without restriction.  Specifically, if other files
   instantiate templates or use macros or inline functions from this
   file, or you compile this file and link it with other files to produce
   an executable, this file does not by itself cause the resulting
   executable to be covered by the GNU General Public License.  This
   exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License.  */

// part of generic lib

#ifndef MY_LIB_LIB_HH
# define MY_LIB_LIB_HH

# include <cmath>
# include <iostream>
# include <sstream> // Warning does not work without me
# include <vector>

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

inline
void foo1()
{
  std::cout << "foo1()" << std::endl;
}

inline
int* foo2()
{
  static int i = 42;
  std::cout << "foo2() => &42" << std::endl;
  return &i;
}

inline
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

# define stl_each(c, i) for (i = c.begin(); i != c.end(); ++i)

# define echo(i) std::cout << i << std::endl

} // end of my_lib

#endif // ! MY_LIB_LIB_HH
