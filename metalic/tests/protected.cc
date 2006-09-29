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

#include <mlc/switch.hh>
#include <mlc/cmp.hh>
#include <mlc/typedef.hh>
#include <mlc/assert.hh>
#include <mlc/implies.hh>


mlc_case_equipment_for_namespace(client);
mlc_decl_typedef(protected_);

template <typename T>
void reveal()
{
  T tmp;
  // Dummy usage.
  tmp = tmp;
}



namespace client
{

  struct A
  {
    typedef bool typedef_in_A_only;
  };

  struct B
  {
    typedef char typedef_in_B_only;
  };



  // simple_test

  struct simple_test;

  template <class T>
  struct case_ <simple_test, T, 1> : public mlc::where_< mlc_eq(T, A) >
  {
    typedef A ret;
  };

  template <class T>
  struct case_ <simple_test, T, 2> : public mlc::where_< mlc_eq(T, B) >
  {
    typedef B ret;
  };


  // test_ok

  struct test_ok;

  template <class T>
  struct case_ <test_ok, T, 1> : public mlc::where_< mlc_eq(T, A) >
  {
    typedef typename A::typedef_in_A_only ret;
  };

  template <class T>
  struct case_ <test_ok, T, 2> : public mlc::where_< mlc_eq(T, B) >
  {
    typedef typename B::typedef_in_B_only ret;
  };



  // test_KO

  struct test_KO;

  template <class T>
  struct case_ <test_KO, T, 1> : public mlc::where_< mlc_eq(T, A) >
  {
    typedef typename T::typedef_in_A_only ret;
  };

  template <class T>
  struct case_ <test_KO, T, 2> : public mlc::where_< mlc_eq(T, B) >
  {
    typedef typename T::typedef_in_B_only ret;
  };


  // test_soluce

  struct test_soluce;

  template <class T>
  struct case_ <test_soluce, T, 1> : public mlc::where_< mlc_eq(T, A) >
  {
    struct protected_ {
      typedef typename T::typedef_in_A_only ret;
    };
  };

  template <class T>
  struct case_ <test_soluce, T, 2> : public mlc::where_< mlc_eq(T, B) >
  {
    // typedef mlc::dummy ret;
    struct protected_ {
      typedef typename T::typedef_in_B_only ret;
    };
  };



} // end of namespace client


struct ERROR_USE_EITHER_PROTECT_OR_RET_IN_A_CASE;



int main()
{

  using namespace client;

  typedef  switch_<simple_test, B>::ret  type;
  reveal<type>();
  

  // typedef  switch_<test, B>    ::ret  type;
  //  which is equiv to:
  // typedef  case_<test, B>::ret ::ret  type;

//   typedef case_<test_soluce, B>::ret ::protect::ret type;




//   typedef case_<test_soluce, B>::ret         the_case;
//   typedef mlc_typedef_(the_case, protected_) the_case_protected;
//   typedef mlc_ret_(the_case)                 the_case_ret;

//   mlc::assert_< mlc::xor_< mlc_is_found(the_case_protected),
//                            mlc_is_found(the_case_ret) >,
//                 ERROR_USE_EITHER_PROTECT_OR_RET_IN_A_CASE
//               >::check();

//   typedef mlc::if_< mlc::eq_< the_case_protected, mlc::not_found >,
//                     the_case_ret,
//                     mlc_ret_(the_case_protected) >::ret result;


//   reveal<result>();
}
