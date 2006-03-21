// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006 EPITA Research and Development Laboratory
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

#ifndef METALIC_BOOL_HH
# define METALIC_BOOL_HH

# include <mlc/value.hh>
# include <mlc/flags.hh>


/*! \def mlc_bool(Bexpr)
**
** Macro that retrieves a Boolean value from a Boolean expression type.
** Its result is either true or false.
*/
# define mlc_bool(Bexpr)  mlc::internal::get_bool<Bexpr>::value




namespace mlc
{

  namespace abstract {

    /*! \class mlc::abstract::boolean
    **
    ** Abstract base class for Booleans represented as types.  This
    ** class has two important sub-classes: mlc::true_ and
    ** mlc::false_.
    **
    ** Any mlc Boolean expression type derives from either mlc::true_
    ** or mlc::false_ so derives from mlc::abstract::boolean.
    */
    
    struct boolean : public value {};

  } // end of mlc::abstract


  // fwd decl
  template <bool> struct bool_;


  namespace internal
  {

    /*! \class mlc::get_bool<T>
    **
    ** Internal class so do not use it.  This class gives access to a
    ** Boolean value from a Boolean expression type; it is used in the
    ** mlc_bool(T) macro.
    **
    ** \see mlc_bool(T)
    */
    template <typename T>
    struct get_bool
    {
      static const bool value = T::bool_value_;
    };


    /*! \class mlc::value_<bool, b>
    **
    ** Specialization of value_<T, val> for the Boolean case.
    **
    ** Design notes: 1) This specialization is defined so that mlc
    ** Booleans derive from mlc::abstract::boolean.  2) This
    ** specialization conforms to the interface of the generic version
    ** of mlc::internal::value_.
    */

    template <bool b>
    struct value_ <bool, b> : public abstract::boolean
    {
      /*! \typedef type
      **
      ** Gives the regular type, that is bool, corresponding to this
      ** class.
      */
      typedef bool type;

      /*! \member value
      **
      ** Gives the regular value of the Boolean represented by this
      ** class.  The result is either true or false.
      **
      ** This member is provided for any mlc value class such as
      ** mlc::int_<51> and mlc::char_<'c'>.  However, to access the
      ** regular value of a type T that you know to be a mlc Boolean
      ** expression, you should prefer to use the macro mlc_bool(T).
      ** This macro also ensures that T really is an expression.
      ** 
      */
      static const bool value = b;

      /*! \typedef eval
      **
      ** Returns mlc::true_ or mlc::false_.
      **
      ** Since Boolean expression types derive from either mlc::true_
      ** or mlc::false_, these expression types are never exactly
      ** mlc::true_ nor mlc::false_.  This typedef thus allows for the
      ** expression evaluation.
      **
      ** Please note that, however, we usually do not need expression
      ** evaluation.  The most common use of a Boolean expression is
      ** to check that it is verified (true) and, for that, we provide
      ** "assert_<bexpr, errmsg>::check()".  For instance:
      ** assert_< or_<mlc_is_a(T, int), mlc_is_a(T, unsigned)> >::check();
      ** ensures that T is int or unsigned without using ::eval.
      **
      ** \see mlc::true_, mlc::false_, mlc::assert_<bexpr, errmsg>.
      */
      typedef bool_<b> eval;

    private:

      /*! \member bool_value_
      **
      ** This member is redundant with the member 'value'.  It is used
      ** by the mlc_bool(T) macro to ensure that T actually derives
      ** from mlc::true_ or mlc::false_.
      */
      static const bool bool_value_ = b;

      template <typename T> friend class get_bool;
    };


  } // end of namespace mlc::internal



  /*! \class mlc::bool_<true>
  **
  ** Specialization of mlc::bool_<b> for b set to true.  This type
  ** is also known as mlc::true_.  Every Boolean expression types
  ** derive either from this type or from mlc::false_.
  **
  ** Design notes: 1) This specialization is defined so that mlc
  ** Booleans derive from mlc::abstract::boolean.  2) This
  ** specialization conforms to the interface of the generic version
  ** of mlc::internal::value_.
  **
  ** \see mlc::bool_<false>
  */

  template <>
  struct bool_<true> : public internal::value_<bool, true>
  {

    // FIXME: doc
    using internal::value_<bool, true>::value;
    using internal::value_<bool, true>::eval;

    /*! \typedef is_true
    **
    ** You should not use this typedef.
    **
    ** This typedef is inherited in every Boolean expression types
    ** that derive from mlc::true_.  This typedef is not provided in
    ** mlc::false_.  The type returned by this typedef has no meaning
    ** (and thus no significant value).  Static checks via
    ** "mlc::assert_<bexpr, err>" rely on the presence or absence of
    ** this typedef.
    **
    ** \see mlc::assert_<bexpr, err>
    */
    typedef dummy is_true;

  };

  typedef bool_<true>  true_;


  /*! \class mlc::bool_<false>
  **
  ** Specialization of mlc::bool_<b> for b set to false.  This type
  ** is also known as mlc::false_.  Every Boolean expression types
  ** derive either from this type or from mlc::true_.
  **
  ** Conversely to mlc::true_, this class does not feature the typedef
  ** "is_true".  So, when a Boolean expression type, say Expr, is
  ** evaluated to false, the static checks via "assert_<Expr>"
  ** do not compile.
  **
  ** Design notes: 1) This specialization is defined so that mlc
  ** Booleans derive from mlc::abstract::boolean.  2) This
  ** specialization conforms to the interface of the generic version
  ** of mlc::internal::value_.
  **
  ** \see mlc::bool_<true>, mlc::assert_<bexpr, errmsg>
  */

  template <>
  struct bool_<false> : public internal::value_<bool, false>
  {
    // FIXME: doc
  private:
    typedef internal::value_<bool, false> super_type;
  public:
    using super_type::value;
    using super_type::eval;
  };

  typedef bool_<false> false_;


} // end of namespace mlc


# include <mlc/logic.hh>


#endif // ! METALIC_BOOL_HH
