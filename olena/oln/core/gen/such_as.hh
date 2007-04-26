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

#ifndef OLN_CORE_GEN_SUCH_AS_HH
# define OLN_CORE_GEN_SUCH_AS_HH


namespace oln
{
  struct such_as;
}


# include <oln/core/internal/op_pset_such_as_fp2b.hh>
# include <oln/core/internal/op_image_such_as_fp2b.hh>
# include <oln/core/gen/fun.hh>
# include <oln/core/gen/inplace.hh>


namespace oln
{

  // Point_Set | Function_p2b

  template <typename S, typename F>
  op_<const S, such_as, const F>
  operator | (const Point_Set<S>& pset, const Function_p2b<F>& f);


  // Point_Set | P -> B

  template <typename S, typename B, typename P>
  op_<const S, such_as, const fun_p2b_<B (*)(P)> >
  operator | (const Point_Set<S>& lhs, B (*f)(P));


  // Image | Function_p2b

  // FIXME: Do it!
//   template <typename I, typename F>
//   set_trait_< Image, I, pipe_id, Function_p2b, F >
//   {
//     typedef op_<I, such_as, F> ret;
//   };

  template <typename I, typename F>
  op_<const I, such_as, const F>
  operator | (const Image<I>& ima, const Function_p2b<F>& f);

  template <typename I, typename F>
  op_<I, such_as, const F>
  operator | (Mutable_Image<I>& ima, const Function_p2b<F>& f);

  template <typename I, typename F>
  inplace_< op_<I, such_as, const F> >
  operator | (inplace_<I> ima, const Function_p2b<F>& f);


  // Image | P -> B

  template <typename I, typename B, typename P>
  op_<const I, such_as, const fun_p2b_<B (*)(P)> >
  operator | (const Image<I>& ima, B (*f)(P));

  template <typename I, typename B, typename P>
  op_<I, such_as, const fun_p2b_<B (*)(P)> >
  operator | (Mutable_Image<I>& ima, B (*f)(P));

  template <typename I, typename B, typename P>
  inplace_< op_<I, such_as, const fun_p2b_<B (*)(P)> > >
  operator | (inplace_<I> ima, B (*f)(P));


  // Image | Binary_Image

  template <typename I, typename J>
  op_<const I, such_as, const fun_p2b_< Binary_Image<J> > >
  operator | (const Image<I>& ima, const Binary_Image<J>& f_ima_b);

  template <typename I, typename J>
  op_<I, such_as, const fun_p2b_< Binary_Image<J> > >
  operator | (Mutable_Image<I>& ima, const Binary_Image<J>& f_ima_b);

  template <typename I, typename J>
  inplace_< op_<I, such_as, const fun_p2b_< Binary_Image<J> > > >
  operator | (inplace_<I> ima, const Binary_Image<J>& f_ima_b);




# ifndef OLN_INCLUDE_ONLY

  // Point_Set | Function_p2b

  template <typename S, typename F>
  op_<const S, such_as, const F>
  operator | (const Point_Set<S>& pset, const Function_p2b<F>& f)
  {
    typedef mlc_basic(oln_argument(F)) P_;
    mlc::assert_< mlc_is_a(P_, Point) >::check(); // FIXME: Add err msg.
    mlc::assert_equal_< P_, oln_point(S) >::check();
    op_<const S, such_as, const F> tmp(exact(pset), exact(f));
    return tmp;
  }

  // Point_Set | P -> B

  template <typename S, typename B, typename P>
  op_<const S, such_as, const fun_p2b_<B (*)(P)> >
  operator | (const Point_Set<S>& pset, B (*f)(P))
  {
    return pset | functorize_p2b(f);
  }

  // Image | Function_p2b

  template <typename I, typename F>
  op_<const I, such_as, const F>
  operator | (const Image<I>& ima, const Function_p2b<F>& f)
  {
    typedef mlc_basic(oln_argument(F)) P_;
    mlc::assert_< mlc_is_a(P_, Point) >::check(); // FIXME: Add err msg.
    mlc::assert_equal_< P_, oln_point(I) >::check();
    op_<const I, such_as, const F> tmp(exact(ima), exact(f));
    return tmp;
  }

  template <typename I, typename F>
  op_<I, such_as, const F>
  operator | (Mutable_Image<I>& ima, const Function_p2b<F>& f)
  {
    typedef mlc_basic(oln_argument(F)) P_;
    mlc::assert_< mlc_is_a(P_, Point) >::check(); // FIXME: Add err msg.
    mlc::assert_equal_< P_, oln_point(I) >::check();
    op_<I, such_as, const F> tmp(exact(ima), exact(f));
    return tmp;
  }

  template <typename I, typename F>
  inplace_< op_<I, such_as, const F> >
  operator | (inplace_<I> ima, const Function_p2b<F>& f)
  {
    return (ima.unwrap() | f).inplace();
  }

  // Image | P -> B

  template <typename I, typename B, typename P>
  op_<const I, such_as, const fun_p2b_<B (*)(P)> >
  operator | (const Image<I>& ima, B (*f)(P))
  {
    return ima | functorize_p2b(f);
  }

  template <typename I, typename B, typename P>
  op_<I, such_as, const fun_p2b_<B (*)(P)> >
  operator | (Mutable_Image<I>& ima, B (*f)(P))
  {
    return ima | functorize_p2b(f);
  }

  template <typename I, typename B, typename P>
  inplace_< op_<I, such_as, const fun_p2b_<B (*)(P)> > >
  operator | (inplace_<I> ima, B (*f)(P))
  {
    return (ima.unwrap() | f).inplace();
  }

  // Image | Binary_Image

  template <typename I, typename J>
  op_<const I, such_as, const fun_p2b_< Binary_Image<J> > >
  operator | (const Image<I>& ima, const Binary_Image<J>& f)
  {
    precondition(f.points() >= ima.points());
    mlc::assert_equal_< oln_point(I), oln_point(J) >::check();
    op_<const I, such_as, const fun_p2b_< Binary_Image<J> > > tmp(exact(ima), exact(f));
    return tmp;
  }

  template <typename I, typename J>
  op_<I, such_as, const fun_p2b_< Binary_Image<J> > >
  operator | (Mutable_Image<I>& ima, const Binary_Image<J>& f)
  {
    precondition(f.points() >= ima.points());
    mlc::assert_equal_< oln_point(I), oln_point(J) >::check();
    op_<I, such_as, const fun_p2b_< Binary_Image<J> > > tmp(exact(ima), exact(f));
    return tmp;
  }

  template <typename I, typename J>
  inplace_< op_<I, such_as, const fun_p2b_< Binary_Image<J> > > >
  operator | (inplace_<I> ima, const Binary_Image<J>& f)
  {
    return (ima.unwrap() | f).inplace();
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_SUCH_AS_HH
