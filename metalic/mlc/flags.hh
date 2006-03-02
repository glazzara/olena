// Copyright (C) 2005, 2006 EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef METALIC_FLAGS_HH
# define METALIC_FLAGS_HH

# include <mlc/type.hh>


namespace mlc {


  namespace abstract {

    /*! \class mlc::abstract::flag
    **
    ** Abstract base class for types that are flags.  A flag type
    ** expresses a particular property translated by the name of the
    ** type.  For instance, mlc::undefined is a flag type which
    ** means "declared but not defined".
    **
    ** Flag types have no interface; they only serve as types.
    **
    ** The flags provided in mlc are: dummy, locked, none, not_found,
    ** undefined, and unknown.
    */
    
    struct flag : public type {};

  } // end of namespace mlc::abstract


  /*! \class mlc::dummy
  **
  ** Flag class to value a type when one knows that this value does
  ** not matter and will not be used.
  **
  ** The dummy type appears typically in some part of meta-program
  ** that are ignored.  In mlc_typedef_onlyif, for instance, dummy is
  ** returned when the guard value is false.  When defining a
  ** pattern matching meta-program, if a clause is not a match, the
  ** returned type for this clause is evaluated into dummy.
  */
  struct dummy : public abstract::flag {};


  /*! \class mlc::locked
  **
  ** Flag class whose purpose is to state and ensure that a type
  ** should not be re-defined (or specialized) by the client.
  **
  ** Let us consider a type that should not be re-defined or
  ** specialized by the client.  Such a type is defined having
  ** mlc::locked as base class.  Inserting the static check
  ** mlc_is_a(type, locked) then ensures that this type is untouched.
  **
  ** Sample use: First a type (set_foo) is declared for the user to
  ** specialize it:
  **   
  **   template <typename T> struct set_foo;
  **   
  ** and the access to this specialization (handled by the type
  ** get_foo), for use in a meta-program, is written once for all.
  ** The access code should not be re-defined by the client so it is
  ** tagged as locked:
  **
  **   template <typename T>
  **   struct get_foo : public mlc::locked,
  **   		        public set_foo<T> {
  **     using set_foo<T>::ret;
  **   };
  **
  ** The meta-program that reads the value of get_foo<T>::ret also
  ** checks that get_foo<T> derives from mlc::locked.  That thus
  ** ensures that the client has not confuse set_foo with get_foo.
  */
  struct locked : public abstract::flag {};


  /*! \class mlc::none
  **
  ** Flag class to state that there is no type corresponding to a
  ** request.
  **
  ** Sample use 1: In the context of a variadic parameter list, none
  ** is the default value for every parameter after the first one.
  **
  ** With:
  **   template <typename P1,
  **             typename P2 = none,
  **             typename P3 = none>
  **   struct foo {
  **     typedef P1 param1;
  **     typedef P2 param2;
  **     typedef P3 param3;
  **   };
  **
  ** instantiated into foo<int, float>, we thus have param3 set to
  ** none.
  **
  ** Sample use 2: The type none can be used as a trait value.  For
  ** instance:
  **
  **   template<>
  **   struct super_type_of <int> {
  **     typedef none ret;
  **   };
  */
  struct none : public abstract::flag {};


  /*! \class mlc::not_found
  **
  ** Flag class to state that a type is not found.  This class is for
  ** use in the context of trying to retrieve a type.  More precisely,
  ** this flag type should only appear as a return of a piece of
  ** meta-program.
  **
  ** Design issue: this flag is used in some meta-code to handle the
  ** case of an "absent" typedef.  For instance it is used in the
  ** meta-code behind mlc_typedef:
  ** mlc_typedef(std::vector<int>, value_type) is int, whereas
  ** mlc_typedef(std::vector<int>, walue_type) is mlc::not_found.
  **
  ** \see mlc_typedef
  */
  struct not_found : public abstract::flag {};


  /*! \class mlc::found
  **
  ** The class corresponding to "not mlc::not_found".
  **
  ** \see mlc::not_found
  */
  struct found : public abstract::flag {};


  /*! \class mlc::undefined
  **
  ** Flag class to state that a type is undefined, that is, declared
  ** but not defined.
  **
  ** Sample use: When you want to define a trait to get the signed
  ** version of a type T, you write:
  **
  **   template <typename T>
  **   struct signed_type_of
  **   {
  **     typedef mlc::undefined ret;
  **   };
  **
  ** meaning that the return type (ret) is not defined by default.
  ** Put differently, this trait is just declared.  So far, the trait
  ** value signed_type_of<unsigned short>::ret is mlc::undefined.  If
  ** it happens that this trait value is not specialized in some
  ** following code, this value thus remains mlc::undefined, which
  ** clearly denotes an error.
  ** 
  ** \see mlc::abstract::flag.
  */
  struct undefined : public abstract::flag {};


  /*! \class mlc::unknown
  **
  ** Flag class to state that a result is yet unknown.
  ** 
  ** Sample use: This flag can be used in a meta-program to express
  ** that a sub-program fails to give the expected result.  For
  ** instance in:
  **
  **   template <class T>
  **   struct foo {
  **     typedef sub_program_1 ret1;
  **     typedef sub_program_2 ret2;
  **     typedef typename if_< neq_<ret1, unknown>,
  **                           ret1,
  **                           ret2 >::ret ret;
  **   };
  */
  struct unknown : public abstract::flag {};


  // FIXME: Doc.
  struct not_ok : public abstract::flag {};


} // end of namespace mlc


#endif // ! METALIC_FLAGS_HH
