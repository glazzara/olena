// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#include <mlc/is_a.hh>
#include <mlc/case.hh>


struct test;


mlc_case_equipment_for_namespace(client);


namespace client
{

  template <class T>
  struct case_ <test, T, 1> : public mlc::where_< mlc_is_a(T, short) >
  {
    typedef float ret;
  };

  template <class T>
  struct case_ <test, T, 2> : public mlc::where_< mlc_is_a(T, int) >
  {
    typedef double ret;
  };

//   // ok

  template <class T>
  struct case_ <test, T, 3> : public mlc::where_< mlc_is_a(T, char) >
  {
    typedef int ret;
  };

//   // ok

//   template <class T>
//   struct case_ <test, T, 3> : public mlc::where_< mlc_is_a(T, char) >
//   {
//     typedef mlc::not_found ret;
//   };

//   // ok

//   template <class T>
//   struct case_ <test, T, 3> : public mlc::where_< mlc_is_a(T, char) >
//   {
//     struct protected_ {
//       typedef mlc::not_found ret;
//     };
//   };






//   // ko

//   template <class T>
//   struct case_ <test, T, 0> : public mlc::where_< mlc_is_a(T, char) >
//   {
//     typedef int ret;
//   };

//   template <class T>
//   struct case_ <test, T, 3> : public mlc::where_< mlc_is_a(T, char) >
//   {
//   };

//   template <class T>
//   struct case_ <test, T, 3> : public mlc::where_< mlc_is_a(T, char) >
//   {
//     struct protected_ {
//     };
//     typedef int ret;
//   };

//   template <class T>
//   struct case_ <test, T, 3> : public mlc::where_< mlc_is_a(T, char) >
//   {
//     struct protected_ {
//     };
//   };

//   template <class T>
//   struct case_ <test, T, 3>
//   {
//     typedef int ret;
//   };

//   template <class T>
//   struct default_case_ <test, T> : public mlc::where_< mlc::true_ >
//   {
//     typedef int ret;
//   };

//   template <class T>
//   struct default_case_ <test, T>
//   {
//   };

//   template <class T>
//   struct default_case_ <test, T> : public mlc::where_< mlc_is_a(T, char) >
//   {
//     typedef int ret;
//   };

//   template <class T>
//   struct default_case_ <test, T>
//   {
//     struct protected_ {
//       typedef int ret;
//     }
//     typedef int ret;
//   };

//   template <class T>
//   struct default_case_ <test, T>
//   {
//     struct protected_ {
//     }
//   };


} // end of namespace client



template <class T>
void print()
{
  int tmp;
}



int main()
{
  print< client::switch_<test, char>::ret >();
}
