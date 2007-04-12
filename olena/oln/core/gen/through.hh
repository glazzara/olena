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

#ifndef OLN_CORE_GEN_THROUGH_HH
# define OLN_CORE_GEN_THROUGH_HH


// "ima >> ima"  is  "for all p, (ima >> f)(p) = f(ima(p))"


namespace oln
{
  struct through;
}


# include <oln/core/internal/op_image_through_fv2v.hh>
# include <oln/core/gen/fun.hh>
# include <oln/core/gen/inplace.hh>


namespace oln
{

  // Image >> Function_v2v

  template <typename I, typename F>
  op_<const I, through, const F>
  operator >> (const Image<I>& ima, const Function_v2v<F>& f);

  template <typename I, typename F>
  op_<I, through, const F>
  operator >> (Mutable_Image<I>& ima, const Function_v2v<F>& f);

  template <typename I, typename F>
  inplace_< op_<I, through, const F> >
  operator >> (inplace_<I> ima, const Function_v2v<F>& f);

  // Image >> V -> V

  template <typename I, typename W, typename V>
  op_<const I, through, const fun_v2v_<W (*)(V)> >
  operator >> (const Image<I>& ima, W (*f)(V));

  template <typename I, typename W, typename V>
  op_<I, through, const fun_v2v_<W (*)(V)> >
  operator >> (Mutable_Image<I>& ima, W (*f)(V));

  template <typename I, typename W, typename V>
  inplace_< op_<I, through, const fun_v2v_<W (*)(V)> > >
  operator >> (inplace_<I> ima, W (*f)(V));



# ifndef OLN_INCLUDE_ONLY

  // Image >> Function_v2v

  template <typename I, typename F>
  op_<const I, through, const F>
  operator >> (const Image<I>& ima, const Function_v2v<F>& f)
  {
    op_<const I, through, const F> tmp(exact(ima), exact(f));
    return tmp;
  }

  template <typename I, typename F>
  op_<I, through, const F>
  operator >> (Mutable_Image<I>& ima, const Function_v2v<F>& f)
  {
    op_<I, through, const F> tmp(exact(ima), exact(f));
    return tmp;
  }

  template <typename I, typename F>
  inplace_< op_<I, through, const F> >
  operator >> (inplace_<I> ima, const Function_v2v<F>& f)
  {
    return (ima.unwrap() >> f).inplace();
  }

  // Image >> V -> V

  template <typename I, typename W, typename V>
  op_<const I, through, const fun_v2v_<W (*)(V)> >
  operator >> (const Image<I>& ima, W (*f)(V))
  {
    return ima >> functorize_v2v(f);
  }

  template <typename I, typename W, typename V>
  op_<I, through, const fun_v2v_<W (*)(V)> >
  operator >> (Mutable_Image<I>& ima, W (*f)(V))
  {
    return ima >> functorize_v2v(f);
  }

  template <typename W, typename V, typename I>
  inplace_< op_<I, through, const fun_v2v_<W (*)(V)> > >
  operator >> (inplace_<I> ima, W (*f)(V))
  {
    return (ima.unwrap() >> f).inplace();
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_THROUGH_HH
