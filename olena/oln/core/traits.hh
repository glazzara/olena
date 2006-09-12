// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_TRAITS_HH
# define OLENA_CORE_TRAITS_HH


# include <mlc/assert.hh>
# include <mlc/abort.hh>
# include <xtd/optraits.hh>
# include <stc/exact.hh>

# include <oln/core/type.hh>
# include <oln/core/case.hh>


namespace oln
{

  template <typename data> struct default_case_ < xtd::op_eq,      data > { typedef bool ret; };
  template <typename data> struct default_case_ < xtd::op_neq,     data > { typedef bool ret; };
  template <typename data> struct default_case_ < xtd::op_less,    data > { typedef bool ret; };
  template <typename data> struct default_case_ < xtd::op_leq,     data > { typedef bool ret; };
  template <typename data> struct default_case_ < xtd::op_greater, data > { typedef bool ret; };
  template <typename data> struct default_case_ < xtd::op_geq,     data > { typedef bool ret; };

} // end of namespace oln



namespace xtd
{

  static const unsigned oln_xtd_case_op_id = 2;


  template <typename name,
	    typename T>
  struct case_ < xtd::op_<name>, T,
		 oln_xtd_case_op_id > : public mlc::where_< mlc_is_a(T, oln::type) >
  {
    struct protected_ {
      typedef typename oln::switch_< xtd::op_<name>,
				     stc_to_exact(T) >::ret ret;
    };
  };


  template <typename name,
	    typename L, typename R>
  struct case_ < xtd::op_<name>, mlc::pair_<L, R>,
		 oln_xtd_case_op_id > : public mlc::where_< mlc::or_<mlc_is_a(L, oln::type),
								     mlc_is_a(R, oln::type)> >
  {
    struct protected_ {
      typedef typename oln::switch_< xtd::op_<name>,
				     mlc::pair_<stc_to_exact(L),
						stc_to_exact(R)> >::ret ret;
    };
  };


} // end of namespace xtd



#endif // ! OLENA_CORE_TRAITS_HH
