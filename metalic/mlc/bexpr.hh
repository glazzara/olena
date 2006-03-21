// Copyright (C) 2006  EPITA Research and Development Laboratory
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

#ifndef METALIC_BEXPR_HH
# define METALIC_BEXPR_HH

# include <mlc/type.hh>
# include <mlc/flags.hh>



namespace mlc
{


  /// Forward declarations for Booleans value type and their aliases.
  /// \{
  template <bool b> struct bool_;
  typedef bool_<true>  true_;
  typedef bool_<false> false_;
  /// \}



  namespace abstract {

    /*! \class mlc::abstract::bexpr
    **
    ** Abstract base class for mlc Boolean expression types.
    **
    ** When you define a new class for a Boolean expression type, you
    ** should not directly derive from this class from fom its
    ** subclass: bexpr_is_<b>.
    **
    ** \see bexpr_is_<b>
    */

    struct bexpr : public type
    {
      // typedef void eval;
    };

  } // end of namespace mlc::abstract



  /*! \class mlc::bexpr_is_<b>
  **
  ** Base class for any class of mlc Boolean expression types.
  ** When you define a new class for a Boolean expression type, you
  ** should directly derive from this class.
  **
  ** This class provides the typedef 'eval' which evaluates to either
  ** mlc::true_ or mlc::false_ the expression.
  **
  ** \see mlc::bexpr_is_<true>, mlc::bexpr_is_<false>
  */

  template <bool b> struct bexpr_is_;


  /*! \class mlc::bexpr_is_<true>
  **
  ** Specialization of mlc::bexpr_is_<b> when b is 'true'.  This class
  ** provides the typedef member 'is_true' to every Boolean expression
  ** type that evaluates to true_.
  ** 
  ** \see mlc::bexpr_is_<b>, mlc::bexpr_is_<false>
  */

  template <>
  struct bexpr_is_ < true > : public abstract::bexpr
  {
    typedef mlc::true_ eval;

    /*! \typedef is_true
    **
    ** This member is inherited in every Boolean expression type that
    ** evaluates to true_.  Conversely, such member does not appear in
    ** bexpr_is_<false> so it does not exist in every Boolean
    ** expression type that evaluates to false_.
    */
    typedef mlc::dummy is_true;
  };


  /*! \class mlc::bexpr_is_<false>
  **
  ** Specialization of mlc::bexpr_is_<b> when b is 'false'.  This class
  ** does not provide the typedef member 'is_true' to every Boolean expression
  ** type that evaluates to false_.
  ** 
  ** \see mlc::bexpr_is_<b>, mlc::bexpr_is_<true>
  */

  template <>
  struct bexpr_is_ < false > : public abstract::bexpr
  {
    typedef mlc::false_ eval;
  };




  namespace internal
  {

    /// \class mlc::internal::bool_of_<bexpr>
    /// \brief Returns the bool value corresponding to a Boolean expression type.
    /// \note Internal class, don't use it directly.

    template <typename bexpr>
    struct bool_of_
    // FIXME: commented below to avoid circular dependances
    // : private assert_< mlc_is_a(bexpr, mlc::abstract::bexpr) >
    {
      // 1) the bexpr evaluates to either mlc::true_ or mlc::false_
      //    so the expected result comes from the value of b in
      //    mlc::bool_<b>
      // 2) retrieving b is performed by the specialization of
      //    internal::bool_of_ provided few lines below
      static const bool ret = internal::bool_of_<typename bexpr::eval>::ret;
    };

    /// \class mlc::internal::bool_of_< mlc::bool_<b> >
    /// \brief Specialization of mlc::internal::bool_of_<bexpr>
    /// \note Internal class, don't use it directly.
    /// \see mlc::internal::bool_of_<bexpr>
    template <bool b>
    struct bool_of_ < mlc::bool_<b> >
    {
      static const bool ret = b;
    };

  } // end of namespace mlc::internal


} // end of namespace mlc




/** \def mlc_bool(Bexpr)
 **
 ** Returns the bool value corresponding to a Boolean expression type.
 **
 ** \note Value retrieval is performed by
 ** mlc::internal::bool_of_<Bexpr>
 */

# define mlc_bool(Bexpr) mlc::internal::bool_of_<Bexpr>::ret



#endif // ! METALIC_BEXPR_HH
