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

#ifndef OLN_CORE_INTERNAL_IMAGE_OPS_HH
# define OLN_CORE_INTERNAL_IMAGE_OPS_HH

# include <oln/core/internal/f_ch_value.hh>
# include <oln/core/internal/op_image_restricted_to_pset.hh>
# include <oln/core/internal/op_image_such_as_fp2b.hh>
# include <oln/core/internal/op_fp2v_over_pset.hh>
# include <oln/core/gen/literal.hh>



namespace oln
{
  // FIXME: when L is Image the "non-const" version should feature Mutable_Image


  // Image | Point_Set    ( ima restricted_to pset )

  oln_decl_op_restricted_to(Image, Point_Set);
  oln_decl_inplace_image_op(restricted_to, |, Point_Set);


  // Image | Function_p2b  (     ima such_as "f : p -> b"
  //                         is  ima restricted_to (ima.points such_as f) )
  oln_decl_op_such_as(Image, Function_p2b);
  oln_decl_inplace_image_op(such_as, |, Function_p2b);



  // Function_p2v / Point_Set   ( ima defined by: "f : p -> v" over pset )

  oln_decl_op_over(Function_p2v, Point_Set);




  // Specialization "f : P -> V over Point_Set".

  template <typename V, typename P, typename S>
  op_<const fun_p2v_<V (*)(P)>, over, const S>
  operator / (V (*f)(P), const Point_Set<S>& pts)
  {
    typedef oln_strip_(P) P_;
    mlc::assert_< mlc_is_a(P_, Point) >::check(); // FIXME: Add err msg.
    mlc::assert_equal_< P_, oln_point(S) >::check();
    op_<const fun_p2v_<V (*)(P)>, over, const S> tmp(f, exact(pts));
    return tmp;
  }

  // Specialisation "literal T over Point_Set".

  template <typename V, typename S>
  op_<const lit_p2v_<oln_point(S), V>, over, const S>
  operator / (const literal_<V>& value, const Point_Set<S>& pts)
  {
    lit_p2v_<oln_point(S), V> lit(value);
    op_<const lit_p2v_<oln_point(S), V>, over, const S> tmp(lit, exact(pts));
    return tmp;
  }



  // Specialization "Image such_as f : P -> B".

  template <typename I, typename B, typename P>
  op_<const I, such_as, const fun_p2b_<B (*)(P)> >
  operator | (const Image<I>& ima, B (*f)(P))
  {
    typedef oln_strip_(P) P_;
    mlc::assert_< mlc_is_a(P_, Point) >::check(); // FIXME: Add err msg.
    mlc::assert_equal_< P_, oln_point(I) >::check();
    op_<const I, such_as, const fun_p2b_<B (*)(P)> > tmp(exact(ima), f);
    return tmp;
  }

  template <typename I, typename B, typename P>
  op_<I, such_as, const fun_p2b_<B (*)(P)> >
  operator | (Mutable_Image<I>& ima, B (*f)(P))
  {
    typedef oln_strip_(P) P_;
    mlc::assert_< mlc_is_a(P_, Point) >::check(); // FIXME: Add err msg.
    mlc::assert_equal_< P_, oln_point(I) >::check();
    op_<I, such_as, const fun_p2b_<B (*)(P)> > tmp(exact(ima), f);
    return tmp;
  }

  template <typename I, typename B, typename P>
  inplace_< op_<I, such_as, const fun_p2b_<B (*)(P)> > >
  operator | (inplace_<I> ima, B (*f)(P))
  {
    return inplace(ima.unwrap() | f);
  }


  // Specialization "Image such_as ima : P -> B".

  template <typename I, typename J>
  op_<const I, such_as, const fun_p2b_< Binary_Image<J> > >
  operator | (const Image<I>& ima, const Binary_Image<J>& f_ima_b)
  {
    precondition(f_ima_b.points() >= ima.points());
    mlc::assert_equal_< oln_point(I), oln_point(J) >::check();
    op_<const I, such_as, const fun_p2b_< Binary_Image<J> > > tmp(exact(ima), f_ima_b);
    return tmp;
  }

  template <typename I, typename J>
  op_<I, such_as, const fun_p2b_< Binary_Image<J> > >
  operator | (Mutable_Image<I>& ima, const Binary_Image<J>& f_ima_b)
  {
    precondition(f_ima_b.points() >= ima.points());
    mlc::assert_equal_< oln_point(I), oln_point(J) >::check();
    op_<I, such_as, const fun_p2b_< Binary_Image<J> > > tmp(exact(ima), f_ima_b);
    return tmp;
  }

  template <typename I, typename J>
  inplace_< op_<I, such_as, const fun_p2b_< Binary_Image<J> > > >
  operator | (inplace_<I> ima, const Binary_Image<J>& f_ima_b)
  {
    return inplace(ima.unwrap() | f_ima_b);
  }


} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_IMAGE_OPS_HH
