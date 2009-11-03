/* Copyright (C) 2005 EPITA Research and Development Laboratory (LRDE)

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

#ifndef MY_LIB_DAMIEN_HXX
# define MY_LIB_DAMIEN_HXX

# include <iostream>
# include "damien.hh"

template <typename T>
const T* down<T>::get_t() const { return &t_; }

template <typename T>
const down<T>* down<T>::clone()
{
  return new down<T>(t_);
}

template <typename T>
down<T>
mk_down(const T& t)
{
  return t;
}

void foo(const up& a)
{
  std::cout << "foo(" << a << ")" << std::endl;
}

void foo(const int& i)
{
  std::cout << "foo(" << i << ")" << std::endl;
}

void bar(const up& a, const up& b, int c=0)
{
  std::cout << "bar(" << a << ", " << b << ", " << c << ")" << std::endl;
}

void change(up& a)
{
  a.i_++;
}

template <typename T>
void print(const T& a)
{
  std::cout << a << std::endl;
}

template <typename T>
T sqr(const T& a)
{
  return a * a;
}


#endif // ! MY_LIB_DAMIEN_HXX
