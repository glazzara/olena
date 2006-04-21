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

#ifndef METALIC_ASSERT_HH
# define METALIC_ASSERT_HH

# include <mlc/flags.hh>



namespace mlc
{


  namespace internal
  {

    /*! \class mlc::internal::check_<bexpr, result>
    **
    ** Internal so do not use it.  This class is for use in the
    ** definition of mlc::assert_<bexpr, err>.
    **
    ** Design note: this class does not derive from abstract::type
    ** because it is used in inheritance so a ctor should exist.
    **
    ** \see mlc::assert_<bexpr, err>
    */

    template <typename bexpr, typename result>
    struct check_
    {
    };


    /*! \class mlc::internal::check_item_<i, bexpr, result>
    **
    ** Internal so do not use it.  This class is for use in the
    ** definition of mlc::multiple_assert_<..>.
    **
    ** Design note: this class does not derive from abstract::type
    ** because it is used in inheritance so a ctor should exist.
    **
    ** \see mlc::multiple_assert_<..>
    */

    template <unsigned i, typename bexpr, typename result>
    struct check_item_
    {
    };

  } // end of namespace mlc::internal



  /*! \class mlc::no_error_message
  **
  ** FIXME: doc
  */

  struct no_error_message;


  /*! \class mlc::no_bexpr
  **
  ** Internal class for use in mlc::multiple_assert_<..>.
  */
    
  struct no_bexpr
  {
    typedef mlc::dummy is_true;
  };


  /*! \class mlc::assert_<bexpr, err>
  **
  ** FIXME: this doc is partially obsolete!
  **
  ** This class is a replacement for the instruction "expr::ensure();"
  ** when there is no room for having instruction.  The typical use
  ** is to express a constraint (or several constraints) upon a
  ** parameter (or several parameters) of a templated class.
  **
  ** assert_<expr> only accepts one parameter, which has to be a
  ** Boolean expression type.  An equivalent version for a variadic
  ** list of parameters is multiple_assert_<expr1,..>
  **
  ** Sample use:
  ** 
  **   template <class T>
  **   struct foo : private virtual assert_< neq_<T, int> >
  **   { ... 
  **   };
  ** means that T can be any type but int.
  **
  **
  ** Please avoid the following code:
  **   template <class T1, class T2>
  **   struct bar : private virtual assert_< neq_<T1, int> >,
  **                private virtual assert_< neq_<T2, int> >
  **   { ... 
  **   };
  ** a better replacement is:
  **   template <class T1, class T2>
  **   struct bar : private virtual multiple_assert_< neq_<T1, int>,
  **                                              neq_<T2, int> >
  **   { ... 
  **   };
  ** see the design notes below for details.
  **
  ** Also prefer the use of multiple_assert_<expr1, expr2> than the
  ** equivalent assert_< and_<expr1, expr2> >.  Actually, the former
  ** provides better error messages since the compiler is able to
  ** say which expr is not verified, whereas the latter cannot.
  **
  **
  ** Design notes: 1) This class does not derive from abstract::type
  ** because it is used in inheritance so a ctor should exist.  2)
  ** This class relies on mlc::internal::assert_item_ to check that
  ** the expression is true.  3) When several contrains such as
  ** "private assert_<..>" appear through a hierarchy of classes or
  ** for a given class, the program may not compile because of
  ** multiple inheritance of the same base class; the solution is to
  ** systematically write "private virtual assert_<..>".
  **
  ** \see multiple_assert_<bexpr1,..>
  **
  */

  template <typename bexpr, typename err = no_error_message>
  struct assert_ :
    public virtual internal::check_<bexpr, typename bexpr::is_true>
  {
  public:
    static void check() {} 
    virtual ~assert_() {}
  protected:
    assert_() {}
  };



  /*! \class mlc::multiple_assert_<bexpr1..>
  **
  ** FIXME: this doc is partially obsolete!
  **
  ** This class is a replacement for a sequence of instructions:
  ** "expr1::ensure(); .." when there is no room for having
  ** instructions.  The typical use is to express a constraint (or
  ** several constraints) upon a parameter (or several parameters)
  ** of a templated class.
  **
  ** multiple_assert_<..> has a variadic list of parameters.  It expects
  ** at least 2 parameters and handles up to 9 parameters.  Each
  ** parameter has to be a Boolean expression type.  To check only a
  ** single expression, the appropriate tool is assert_<expr>.
  **
  **
  ** Sample use:
  ** 
  **   template <class T1, class T2>
  **   struct foo : private virtual multiple_assert_< neq_<T1, int>,
  **                                              neq_<T2, int> >
  **   { ... 
  **   };
  **
  ** Design notes: 1) This class does not derive from abstract::type
  ** because it is used in inheritance so a ctor should exist.  2)
  ** This class relies on mlc::internal::assert_item_ to check that
  ** each expression is true. 3) using "virtual" allow to encompass
  ** the multiple base class problem.
  **
  ** Limitation: no error message can be provided with this present
  ** version of multiple_assert_ so prefer using several assert_.
  **
  ** \see assert_<bexpr, err>
  */

  template <typename bexpr1,
	    typename bexpr2,
	    typename bexpr3 = no_bexpr, 
	    typename bexpr4 = no_bexpr,
	    typename bexpr5 = no_bexpr,
	    typename bexpr6 = no_bexpr, 
	    typename bexpr7 = no_bexpr,
	    typename bexpr8 = no_bexpr,
	    typename bexpr9 = no_bexpr>
  struct multiple_assert_ :
      private virtual internal::check_item_<1, bexpr1, typename bexpr1::is_true>,
      private virtual internal::check_item_<2, bexpr2, typename bexpr2::is_true>,
      private virtual internal::check_item_<3, bexpr3, typename bexpr3::is_true>,
      private virtual internal::check_item_<4, bexpr4, typename bexpr4::is_true>,
      private virtual internal::check_item_<5, bexpr5, typename bexpr5::is_true>,
      private virtual internal::check_item_<6, bexpr6, typename bexpr6::is_true>,
      private virtual internal::check_item_<7, bexpr7, typename bexpr7::is_true>,
      private virtual internal::check_item_<8, bexpr8, typename bexpr8::is_true>,
      private virtual internal::check_item_<9, bexpr9, typename bexpr9::is_true>
  {
  public:
    static void check() {} 
  protected:
    multiple_assert_() {}
  };


} // end of namespace mlc


#endif // ! METALIC_ASSERT_HH
