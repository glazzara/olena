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

#ifndef MLC_VALUE_HH
# define MLC_VALUE_HH

# include <mlc/abstract/type.hh>
// # include <mlc/assert.hh>
// # include <mlc/is_a.hh>



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

  } // end of namespace mlc::abstract


  namespace internal
  {

    template <typename T>
    struct value_of_
    // FIXME: commented below to avoid circular dependances
    // : private assert_< mlc_is_a(T, mlc::abstract::value) >
    {
      static const typename T::type ret = T::value;
    };

  } // end of namespace mlc::internal


} // end of namespace mlc



/** \def mlc_value(T)
 ** \brief Returns the value of a value type.
 **
 ** Only works when \a T is a value type such as mlc::bool_<b> or
 ** mlc::int_<i>.  The result is respectively a bool value and an int
 ** value; for instance mlc_value(mlc::int_<51>) gives 51.  Please
 ** prefer using this macro to a direct call to T::value because such
 ** a direct call may not compile (read the design notes below for
 ** details).
 **
 ** Design notes: FIXME: doc
 */

# define mlc_value(T) mlc::internal::value_of_< T >::ret




#endif // ! MLC_VALUE_HH
