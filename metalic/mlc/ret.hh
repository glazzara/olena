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

#ifndef METALIC_RET_HH
# define METALIC_RET_HH

# include <mlc/typedef.hh>



/*! \FIXME: what's the correct comment flag here?
**
** Since we often rely on having a typedef named "ret" in types, we offer
** a default equipment for this particular typedef.  FIXME: doc
*/

namespace mlc
{

  mlc_decl_typedef(ret);

} // end of namespace mlc



# define mlc_ret(Type) typename mlc::typedef_::ret::from_<Type>::ret


// test code

# define mlc_ret2(Type) typename mlc::typedef_::ret::from_<Type>::ret2


namespace mlc
{

  template <typename T>
  struct ret_found_in_
    : public mlc::eq_< typename mlc::typedef_::ret::from_<T>::ret2::first_elt,
		       mlc::found >
  {};

} // end of namespace mlc


#endif // ! METALIC_RET_HH
