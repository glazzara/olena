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

#ifndef OLN_CORE_GEN_PW_VALUE_HH
# define OLN_CORE_GEN_PW_VALUE_HH

# include <oln/core/abstract/image.hh>
# include <xtd/abstract/meta_nary_fun.hh>
# include <xtd/math.hh>



namespace xtd
{

  // Fwd decl.
  template <typename I>
  class pw_value_type;


  template <typename I, typename A>
  struct res_< pw_value_type<I>, A >
  {
    typedef oln_rvalue(I) ret;
  };


  template <typename I>
  class pw_value_type : public xtd::abstract::meta_nary_fun_< 1, pw_value_type<I> >
  {
  public:

    pw_value_type(const I& ima);

    template <typename P>
    oln_rvalue(I) impl_calc(const P& p) const;

  protected:

    const I ima_;
  };


# ifndef OLN_INCLUDE_ONLY

  template <typename I>
  template <typename A>
  oln_rvalue(I)
  pw_value_type<I>::impl_calc(const A& a) const
  {
    mlc::assert_< mlc::or_< mlc_is_a(A, oln::abstract::point),
                            mlc_is_a(A, oln::abstract::iterator_on_points) > >::check();
    return ima_(a);
  }

  template <typename I>
  pw_value_type<I>::pw_value_type(const I& ima)
    : ima_(ima)
  {
  }

# endif


} // end of namespace xtd


namespace oln
{
  
  template <typename I>
  xtd::m1expr_< xtd::pw_value_type<I>, xtd::arg_<1> >
  pw_value(const abstract::image<I>& ima)
  {
    xtd::pw_value_type<I> pwv(ima.exact());
    using xtd::_1; 
    return pwv(_1); // expects one argument (for instance a point) or an expression :)
  }

} // end of namespace oln


#endif // ! OLN_CORE_GEN_PW_VALUE_HH
