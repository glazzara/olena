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
    ** Abstract base class for types that are only flags.
    **
    ** Example: mlc::undefined is a mlc::abstract::flag.
    */
    
    struct flag : public type {};

  } // end of namespace mlc::abstract


  /*! \class mlc::undefined
  **
  ** Flag class to state that a type is undefined, that is, declared
  ** but not defined.
  **
  ** Sample use: you want to define a trait to get the signed version
  ** of a type T.  For that, you write:
  **
  **   template <typename T>
  **   struct signed_type_of
  **   {
  **     typedef mlc::undefined ret;
  **   };
  **
  ** which means that the return type (ret) is not defined by default.
  ** Put differently, this trait is just declared.  So far, the
  ** type "signed_type_of<unsigned short>::ret" is "mlc::undefined".
  ** 
  ** Other flags are mlc::unknown, mlc::none, and mlc::not_found.
  */
  struct undefined : public abstract::flag {};

  struct unknown   : public abstract::flag {};

  struct none      : public abstract::flag {};


  /*! \class mlc::not_found
  **
  ** Flag class to state that a type is not found (for use in the
  ** context of trying to retrieve a type).
  **
  ** Design issue: this flag is used in some meta-code to handle the
  ** case of an "absent" typedef.  For instance it is used in the
  ** meta-code behind mlc_typedef:
  ** mlc_typedef(std::vector<int>, value_type) is int, whereas
  ** mlc_typedef(std::vector<int>, walue_type) is mlc::not_found.
  ** 
  ** Other flags are mlc::undefined, mlc::unknown, and mlc::none.
  **
  ** \see mlc_typedef
  */
  struct not_found : public abstract::flag {};


  // FIXME: add corresponding is_* types


} // end of namespace mlc


#endif // ! METALIC_FLAGS_HH
