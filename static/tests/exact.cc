// Copyright (C) 2006  EPITA Research and Development Laboratory.
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#include <iostream>
#include <stc/exact.hh>

template <typename E>
struct abstraction : public stc::any<E>
{
};


template <typename E>
struct sub_abstraction : public abstraction<E>
{
};

struct itself {};


template <typename T = itself>
struct concrete_ : public sub_abstraction< concrete_<T> >
{
  void foo() const
  {
    std::cout << "foo() " << std::endl;
  }
};

typedef concrete_<> concrete;


int main()
{
  //-----------------------------------------

  {
    std::cout << stc::exact(42) << std::endl;
  }

  {
    int i = 42;
    std::cout << stc::exact(i) << std::endl;
  }

  {
    const int i = 42;
    std::cout << stc::exact(i) << std::endl;
  }


  {
    int i = 42;
    int * pi = &i;
    std::cout << pi << " = "
	      << stc::exact(pi) << std::endl;
  }

  {
    int i = 42;
    int * const pi = &i;
    std::cout << pi << " = "
	      << stc::exact(pi) << std::endl;
  }

  {
    int i = 42;
    const int * pi = &i;
    std::cout << pi << " = "
	      << stc::exact(pi) << std::endl;
  }

  {
    int i = 42;
    const int * const pi = &i;
    std::cout << pi << " = "
	      << stc::exact(pi) << std::endl;
  }

  // FIXME: Doesn't work!
  
//   {
//     int i = 42;
//     int * pi = &i;
//     int ** ppi = &pi;
//     // stc::exact(ppi);

//     const int ci = 42;
//     const int * cpi = &ci;
//     const int ** cppi = &cpi;
//     stc::exact(cppi);
//   }

  //-----------------------------------------
  {
    concrete t;
    const concrete::exact_type& tmp = stc::exact(t);
    std::cout << &t << " = "
	      << &tmp << std::endl;
  }
  {
    concrete *t = new concrete;
    std::cout << t << " = "
	      << stc::exact(t) << std::endl;
  }

  //-----------------------------------------
  {
    concrete t;
    sub_abstraction<concrete>& st = t;
    const sub_abstraction<concrete>::exact_type& tmp = stc::exact(st);
    std::cout << &t << " = "
	      << &tmp << std::endl;
    tmp.foo();
  }
  {
    sub_abstraction<concrete> *t = new concrete;
    std::cout << t << " = "
	      << stc::exact(t) << std::endl;
  }

  //-----------------------------------------
  {
    concrete t;
    abstraction<concrete>& a = t;
    const abstraction<concrete>::exact_type& tmp = stc::exact(a);
    std::cout << &t << " = "
	      << &tmp << std::endl;
  }
  {
    abstraction<concrete> *t = new concrete;
    std::cout << t << " = "
	      << stc::exact(t) << std::endl;
  }

}
