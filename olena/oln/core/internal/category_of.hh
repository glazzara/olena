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
# include <oln/core/internal/utils.hh>


# define  oln_category_of_(Type)  \
  typename oln::internal::category_of_< Type >::ret



namespace oln
{

  namespace internal
  {

    template <typename T1, typename T2>
    struct get_category_of_
      : private mlc::abort_< pair<T1,T2> > // FIXME: Add error msg.
    {
    };

    template <template <class> class C>
    struct get_category_of_< C<void>, mlc::not_found >
    {
      typedef stc::is<C> ret;
    };

    template <template <class> class C>
    struct get_category_of_< mlc::not_found, stc::is<C> >
    {
      typedef stc::is<C> ret;
    };


    // category_of_

    template <typename T>
    struct category_of_
    {
      typedef mlc_basic(T) T__;
      typedef mlc_typedef(T__, category)   ret_1;
      typedef stc_find_type(T__, category) ret_2;
      typedef typename get_category_of_<ret_1, ret_2>::ret ret;
    };



    template <class C, typename Exact>
    struct get_category_base_type_;

    template <template <class> class C, typename Exact>
    struct get_category_base_type_< stc::is<C>, Exact >
    {
      typedef C<Exact> ret;
    };

    // category_base_type_from_
    
    template <typename From, typename Exact>
    struct category_base_type_from_
    {
      typedef oln_category_of_(From) C;
      typedef typename get_category_base_type_<C, Exact>::ret ret;
    };

  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_CATEGORY_OF_HH
