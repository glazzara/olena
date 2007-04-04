// Copyright (C) 2006, 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_INTERNAL_CATEGORY_OF_HH
# define OLN_CORE_INTERNAL_CATEGORY_OF_HH

# include <mlc/basic.hh>


# define  oln_category_of_(Type)  typename oln::internal::category_of_< Type >::ret


namespace oln
{

  // Fwd decls.
  template <typename Exact> struct Boolean;



  namespace internal
  {

    // Category_of_


    template <typename T>
    struct set_category_of_
    {
      typedef stc_type(T, category) ret;
    };

    template <>
    struct set_category_of_< bool >
    {
      typedef stc::is< Boolean > ret;
    };


    template <typename T>
    struct category_of_
    {
      typedef typename set_category_of_< mlc_basic(T) >::ret ret;
    };

    // ...




    // only_if_

    template < typename Ret,
	       typename Check_1,
	       typename Check_2 = void,
	       typename Check_3 = void,
	       typename Check_4 = void >
    struct only_if_
    {
      typedef Ret ret;
    };



  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_CATEGORY_OF_HH
