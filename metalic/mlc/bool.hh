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
# include <mlc/bexpr.hh>


namespace mlc
{

  namespace abstract {

    /*! \class mlc::abstract::boolean
    **
    ** Abstract base class for Boolean values represented as types.
    ** This class has exactly one sub-class: mlc::bool_<b> where b is
    ** either true or false.  Thanks to aliases, mlc::bool_<true> and
    ** mlc::bool_<false> are respectively mlc::true_ and mlc::false_.
    */
    
    struct boolean : public value {};

  } // end of mlc::abstract



  /*! \class mlc::bool_<b>
  **
  ** Value type corresponding to bool.
  */
  
  template <bool b>
  struct bool_ : public bexpr_is_<b>,
		 public abstract::boolean
		 
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
    ** expression, you should prefer to use the macro mlc_value(T).
    ** This macro also ensures that T is a mlc value type.
    ** 
    */
    static const bool value = b;
  };


  /*! \def mlc::true_
  **
  ** This is an alias (typedef) for mlc::bool_<true>.
  */
  typedef bool_<true>  true_;


  /*! \def mlc::false_
  **
  ** This is an alias (typedef) for mlc::bool_<false>.
  */
  typedef bool_<false> false_;


} // end of namespace mlc


#endif // ! METALIC_BOOL_HH
