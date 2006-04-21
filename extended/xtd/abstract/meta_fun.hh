// Copyright (C) 2002, 2005, 2006  EPITA Research and Development Laboratory
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

#ifndef EXTENDED_ABSTRACT_META_FUN_HH
# define EXTENDED_ABSTRACT_META_FUN_HH

# include <xtd/abstract/fun.hh>


namespace xtd
{


  namespace ERROR
  {
    struct YOU_SHOULD_NOT_DERIVE_DIRECTLY_FROM_xtd_meta_fun_BUT_FROM_xtd_meta_nary_fun_;

  } // end of namespace xtd::ERROR



  namespace abstract
  {


    /*! \class xtd::abstract::meta_fun_<E>
    **
    ** Abstract base class for meta functions.  Parameter E is the
    ** exact type of the function.
    */

    template <typename E>
    class meta_fun_

      : public fun_<E>
    {
    protected:
      meta_fun_()
      {
	// FIXME: mlc_is_a does not work with unsigned parameter...
	// 	mlc::assert_< mlc_is_a(E, xtd::abstract::meta_nary_fun_),
	// 	              xtd::ERROR::YOU_SHOULD_NOT_DERIVE_DIRECTLY_FROM_xtd_meta_fun_BUT_FROM_xtd_meta_nary_fun_
	// 	            >::check();
      }
    };


  } // end of namespace xtd::abstract

} // end of namespace xtd



#endif // ! EXTENDED_ABSTRACT_META_FUN_HH
