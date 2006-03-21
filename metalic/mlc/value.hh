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

#ifndef METALIC_VALUE_HH
# define METALIC_VALUE_HH

# include <mlc/type.hh>
// # include <mlc/wrap.hh>



/** \def mlc_value(T)
 ** \brief Returns the value of a value type.
 **
 ** Only works when \a T is a value type such as mlc::bool_<b> or
 ** mlc::int_<i>.  The result is respectively a bool value and an int
 ** value.  Please prefer using this macro to a direct call to
 ** T::value because such a direct call may not compile (read the
 ** design notes below for details).
 **
 ** Design notes: FIXME: doc
 */

// # define mlc_value(T) mlc::wrap_<T>::value



/// Internal macros.


# define mlc_internal_decl_unsigned_(Type, TypeName)		\
namespace internal						\
{								\
  template <Type val>						\
  struct value_ <Type, val> : public abstract::unsigned_integer	\
  {								\
    typedef Type type;						\
    static const Type value = val;				\
  };								\
}								\
template <Type val>						\
struct TypeName : public internal::value_<Type, val> {}


# define mlc_internal_decl_signed_(Type, TypeName)		\
namespace internal						\
{								\
  template <Type val>						\
  struct value_ <Type, val> : public abstract::signed_integer	\
  {								\
    typedef Type type;						\
    static const Type value = val;				\
  };								\
}								\
template <Type val>						\
struct TypeName : public internal::value_<Type, val> {}




namespace mlc {


  namespace abstract {

    /*! \class mlc::abstract::value
    **
    ** Abstract base class for mlc types that represent values.
    **
    ** For instance, the types mlc::true_ and mlc::int_<51> derive
    ** from this abstraction.
    */

    struct value : public type
    {
    };

    /// Abstractions for integer values as types.

    struct          integer : public value   {};
    struct unsigned_integer : public integer {};
    struct   signed_integer : public integer {};
    
  } // end of namespace mlc::abstract


  namespace internal
  {

    /*! \class mlc::internal::value_<T, val>
    **
    ** Base class for values to be represented by types.  This class
    ** is in the internal namespace so you should not use it.  Its
    ** purpose is only to factor code for derived classes that are not
    ** internal (for instance, mlc::true_ or mlc::int_<51>).
    **
    ** Design note: this class can only be used for values that can be
    ** a parameter (mlc::internal::value_<float, 3.14f> is incorrect).
    **
    ** Parameter T is the type of the value.  Parameter val is the
    ** value itself.
    **
    ** \see mlc/bool.hh
    */
    
    template <typename T, T val>
    struct value_ : public abstract::value
    {
      /*! \typedef type
      **
      ** Gives the regular type of the value.  For instance,
      ** mlc::true_::type is bool and mlc::int_<51>::type is int.
      */
      typedef T type;

      /*! \member value
      **
      ** Gives the regular value.  For instance,
      ** mlc::true_::value is true and mlc::int_<51>::value is 51.
      */
      static const T value = val;
    };

  } // end of namespace mlc::internal




  // Dedicated sub-classes for builtin types.

  mlc_internal_decl_unsigned_( unsigned char,  uchar_ );
  mlc_internal_decl_unsigned_( unsigned short, ushort_ );
  mlc_internal_decl_unsigned_( unsigned int,   uint_   );
  mlc_internal_decl_unsigned_( unsigned long,  ulong_  );

  mlc_internal_decl_signed_(     signed char,  schar_ );
  mlc_internal_decl_signed_(     signed short, short_ );
  mlc_internal_decl_signed_(     signed int,   int_   );
  mlc_internal_decl_signed_(     signed long,  long_  );


  // char

  template <char c>
  struct char_ : public internal::value_<char, c>
  {
  };


  // Boolean values as types are defined elsewhere
  // see:  mlc/bool.hh


} // end of namespace mlc


# include <mlc/bool.hh>


#endif // ! METALIC_VALUE_HH
