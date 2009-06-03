// Copyright (C) 2007, 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

/// \file tests/core/routine/exact.cc
///
/// \brief Tests on mln::exact.

#include <typeinfo>
#include <mln/core/routine/exact.hh>



struct test : mln::Object< test >
{
};


namespace mln
{

  template <typename E>
  struct Base : Object<E>
  {
    void m()
    {
      int** i = exact(this)->m_impl();
    }
    void m() const
    {
      int* i = exact(this)->m_impl();
    }
  };

  struct concrete : Base< concrete >
  {
    int** m_impl() { return 0; }
    int* m_impl() const { return 0; }
  };

}


int main()
{
  using namespace mln;

  {

    concrete c;
    Base<concrete>& b = c;
    b.m();
    const Base<concrete>& bb = c;
    bb.m();

  }


// -----------------------------------

//   test t;
//   Object<test>& t_ = t;
//   mln_assertion(typeid(exact(t_)).name() == typeid(exact(t)).name());

//   {
//     int i;
//     exact(i);  // `int' from `float'
//     exact(&i); // `int *' from `const double'
//   }
//   {
//     const int j = 0;
//     exact(j);  // `int' from `const double'
//     exact(&j); // `const int *' from `const double
//   }

//   {
//     int i;
//     int& j = i;
//     exact(j);  // `int' from `float'
//     exact(&j); // `int *' from `const double'
//   }
//   {
//     int i;
//     const int& j = i;
//     exact(j);  // `int' from `const double'
//     exact(&j); // `const int *' from `const double'
//   }

//   {
//     int* i;
//     exact(i);  // `int *' from `float'
//     exact(*i);  // `int' from `float'
//      int *const j = 0;
//     exact(j); // `int *' from `const double'
//      exact(*j); `int' from `float'
//   }

//   {
//     const int* i;
//     exact(i);  // `const int *' from `float'
//     exact(*i);  // `int' from `const double'
//     const int *const j = 0;
//     exact(j); // `const int *' from `const double'
//     exact(*j); `int' from `const double'
//   }


// -----------------------------------


//   {
//     int i;
//     exact(&i); // `int *' from `const double'
//   }
//   {
//     const int j = 0;
//     exact(j);  // `int' from `const double'
//     exact(&j); // `const int *' from `const double
//   }

//   {
//     int i;
//     int& j = i;
//     exact(&j); // `int *' from `const double'
//   }
//   {
//     int i;
//     const int& j = i;
//     exact(j);  // `int' from `const double'
//     exact(&j); // `const int *' from `const double'
//   }

//   {
//     int *const j = 0;
//     exact(j); // `int *' from `const double'
//   }

//   {
//     const int* i;
//     exact(*i);  // `int' from `const double'
//     const int *const j = 0;
//     exact(j); // `const int *' from `const double'
//     exact(*j); // `int' from `const double'
//   }


// -----------------------------------


//   {
//     int* i;
//     exact(i);
//   }

//   {
//     int i;
//     exact(i);  // `int' from `float'
//   }

//   {
//     int i;
//     int& j = i;
//     exact(j);  // `int' from `float'
//   }

//   {
//     int* i;
//     exact(i);  // `int *' from `float'
//     exact(*i);  // `int' from `float'
//      int *const j = 0;
//      exact(*j); // `int' from `float'
//   }

//   {
//     const int* i;
//     exact(i);  // `const int *' from `float'
//   }

}
