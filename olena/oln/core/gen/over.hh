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

#ifndef OLN_CORE_GEN_OVER_HH
# define OLN_CORE_GEN_OVER_HH


namespace oln
{
  struct over;
}


# include <oln/core/internal/op_fp2v_over_pset.hh>
# include <oln/core/gen/fun.hh>
# include <oln/core/gen/constant.hh>
# include <oln/core/gen/value.hh>


namespace oln
{

  // Function_p2v / Point_Set

  template <typename F, typename S>
  op_<const F, over, const S>
  operator / (const Function_p2v<F>& f, const Point_Set<S>& pset);


  // (P -> V) / Point_Set

  template <typename V, typename P, typename S>
  op_<const fun_p2v_<V (*)(P)>, over, const S>
  operator / (V (*f)(P), const Point_Set<S>& pset);
  

  // V / Point_Set

  template <typename V, typename S>
  op_<const constant_p2v_<oln_point(S), V>, over, const S>
  operator / (const value_<V>& v, const Point_Set<S>& pset);



# ifndef OLN_INCLUDE_ONLY

  // Function_p2v / Point_Set

  template <typename F, typename S>
  op_<const F, over, const S>
  operator / (const Function_p2v<F>& f, const Point_Set<S>& pset)
  {
    typedef mlc_basic(oln_argument(F)) P_;
    mlc::assert_< mlc_is_a(P_, Point) >::check(); // FIXME: Add err msg.
    mlc::assert_equal_< P_, oln_point(S) >::check();
    op_<const F, over, const S> tmp(exact(f), exact(pset));
    return tmp;
  }

  // P -> V / Point_Set

  template <typename V, typename P, typename S>
  op_<const fun_p2v_<V (*)(P)>, over, const S>
  operator / (V (*f)(P), const Point_Set<S>& pset)
  {
    return functorize_p2v(f) / pset;
  }
  
  // V / Point_Set

  template <typename V, typename S>
  op_<const constant_p2v_<oln_point(S), V>, over, const S>
  operator / (const value_<V>& v, const Point_Set<S>& pset)
  {
    constant_p2v_<oln_point(S), V> v_(v);
    op_<const constant_p2v_<oln_point(S), V>, over, const S> tmp(v_, exact(pset));
    return tmp;
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_OVER_HH
