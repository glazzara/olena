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

#ifndef OLN_CORE_GEN_APPLIED_ON_HH
# define OLN_CORE_GEN_APPLIED_ON_HH


// "f << ima"  is  "for all p, (f << ima)(p) = f(ima(p))"


namespace oln
{
  struct applied_on;
}


# include <oln/core/internal/op_fv2v_applied_on_image.hh>
# include <oln/core/gen/fun.hh>
# include <oln/core/gen/inplace.hh>


namespace oln
{

  // Function_v2v << Image

  template <typename F, typename I>
  op_<const F, applied_on, const I>
  operator << (const Function_v2v<F>& f, const Image<I>& ima);

  template <typename F, typename I>
  op_<const F, applied_on, I>
  operator << (const Function_v2v<F>& f, Mutable_Image<I>& ima);

  template <typename F, typename I>
  inplace_< op_<const F, applied_on, I> >
  operator << (const Function_v2v<F>& f, inplace_<I> ima);

  // V -> V << Image

  template <typename W, typename V, typename I>
  op_<const fun_v2v_<W (*)(V)>, applied_on, const I>
  operator << (W (*f)(V), const Image<I>& ima);

  template <typename W, typename V, typename I>
  op_<const fun_v2v_<W (*)(V)>, applied_on, I>
  operator << (W (*f)(V), Mutable_Image<I>& ima);

  template <typename W, typename V, typename I>
  inplace_< op_<const fun_v2v_<W (*)(V)>, applied_on, I> >
  operator << (W (*f)(V), inplace_<I> ima);



# ifndef OLN_INCLUDE_ONLY

  // Function_v2v << Image

  template <typename F, typename I>
  op_<const F, applied_on, const I>
  operator << (const Function_v2v<F>& f, const Image<I>& ima)
  {
    op_<const F, applied_on, const I> tmp(exact(f), exact(ima));
    return tmp;
  }

  template <typename F, typename I>
  op_<const F, applied_on, I>
  operator << (const Function_v2v<F>& f, Mutable_Image<I>& ima)
  {
    op_<const F, applied_on, I> tmp(exact(f), exact(ima));
    return tmp;
  }

  template <typename F, typename I>
  inplace_< op_<const F, applied_on, I> >
  operator << (const Function_v2v<F>& f, inplace_<I> ima)
  {
    return (f << ima.unwrap()).inplace();
  }

  // V -> V << Image

  template <typename W, typename V, typename I>
  op_<const fun_v2v_<W (*)(V)>, applied_on, const I>
  operator << (W (*f)(V), const Image<I>& ima)
  {
    return functorize_v2v(f) << ima;
  }

  template <typename W, typename V, typename I>
  op_<const fun_v2v_<W (*)(V)>, applied_on, I>
  operator << (W (*f)(V), Mutable_Image<I>& ima)
  {
    return functorize_v2v(f) << ima;
  }

  template <typename W, typename V, typename I>
  inplace_< op_<const fun_v2v_<W (*)(V)>, applied_on, I> >
  operator << (W (*f)(V), inplace_<I> ima)
  {
    return (f << ima.unwrap()).inplace();
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_APPLIED_ON_HH
