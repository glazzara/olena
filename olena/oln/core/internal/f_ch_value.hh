// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_INTERNAL_F_CH_VALUE_HH
# define OLN_CORE_INTERNAL_F_CH_VALUE_HH

# include <oln/core/internal/image_base.hh>


#define oln_ch_value_(I, T) \
oln::internal::f_ch_value_< stc_type_in_(oln, I, skeleton), T >::ret

#define oln_ch_value(I, T) \
typename oln::internal::f_ch_value_< stc_type_in(oln, I, skeleton), T >::ret


# define oln_plain_value(I, T) \
typename oln::internal::f_ch_value_< stc_type_in(oln, oln_plain(I), skeleton), T >::ret



namespace oln
{

  namespace internal
  {

    template <typename I, typename T> struct f_ch_value_;


    template <template <class> class tc,
	      typename T>
    struct f_ch_value_< tc<pl::value>,
			T >
    {
      typedef tc<T> ret;
    };


    template <template <class, class> class tcc, typename t,
	      typename T>
    struct f_ch_value_< tcc<t, pl::value>,
			T >
    {
      typedef tcc<t, T> ret;
    };


    template <template <class> class tc, typename t,
	      typename T>
    struct f_ch_value_< tc< pl::rec<t> >,
			T >
    {
      typedef tc< oln_ch_value(t, T) > ret;
    };


    template <template <class, class> class tcc, typename t1, typename t2,
	      typename T>
    struct f_ch_value_< tcc< pl::rec<t1>, t2 >,
			T >
    {
      typedef tcc< oln_ch_value(t1, T), t2 > ret;
    };


    template <template <class, class, class> class tccc, typename t1, typename t2, typename t3,
	      typename T>
    struct f_ch_value_< tccc< pl::rec<t1>, t2, t3 >,
			T >
    {
      typedef tccc< oln_ch_value(t1, T), t2, t3 > ret;
    };


  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_F_CH_VALUE_HH
