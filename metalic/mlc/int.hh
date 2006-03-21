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

#ifndef METALIC_INT_HH
# define METALIC_INT_HH

# include <mlc/value.hh>



/// Internal macros.

# define mlc_internal_decl_unsigned_integer(Type, TypeName)	\
template <Type val>						\
struct TypeName : public abstract::unsigned_integer		\
{								\
  typedef Type type;						\
  static const Type value = val;				\
}


# define mlc_internal_decl_signed_integer(Type, TypeName)	\
template <Type val>						\
struct TypeName : public abstract::signed_integer		\
{								\
  typedef Type type;						\
  static const Type value = val;				\
}



namespace mlc {


  namespace abstract {

    /// Abstractions for integer values as types.

    struct          integer : public value   {};
    struct unsigned_integer : public integer {};
    struct   signed_integer : public integer {};
    
  } // end of namespace mlc::abstract



  // Dedicated sub-classes for builtin types.

  mlc_internal_decl_unsigned_integer( unsigned char,  uchar_ );
  mlc_internal_decl_unsigned_integer( unsigned short, ushort_ );
  mlc_internal_decl_unsigned_integer( unsigned int,   uint_   );
  mlc_internal_decl_unsigned_integer( unsigned long,  ulong_  );

  mlc_internal_decl_signed_integer(     signed char,  schar_ );
  mlc_internal_decl_signed_integer(     signed short, short_ );
  mlc_internal_decl_signed_integer(     signed int,   int_   );
  mlc_internal_decl_signed_integer(     signed long,  long_  );


} // end of namespace mlc



#endif // ! METALIC_INT_HH
