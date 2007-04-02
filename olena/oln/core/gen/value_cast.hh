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

#ifndef OLN_CORE_GEN_VALUE_CAST_HH
# define OLN_CORE_GEN_VALUE_CAST_HH

# include <oln/core/gen/fun.hh>


// FIXME: Separate defs and decls.



namespace oln
{


  // -----------------------------  casted_fp2v_<F, V>


  // Fwd decl.
  template <typename F, typename V> struct casted_fp2v_;

  // Category.
  namespace internal
  {
    template <typename F, typename V>
    struct set_category_of_< casted_fp2v_<F, V> >
    {
      typedef stc::is< Function_p2v > ret;
    };
  }


  // Class.
  template <typename F, typename V>
  struct casted_fp2v_ : public Function_p2v< casted_fp2v_<F, V> >
  {
    typedef oln_arg_of_(F) argument;
    typedef oln_res_of_(F) f_result;
    typedef V result;

    casted_fp2v_(const F& f) : f_(f) {}

    result operator()(argument arg) const
    {
      return static_cast<V>(this->f_(arg));
    }

  private:
    F f_;
  };


  // value_cast

  template <typename V, typename F>
  casted_fp2v_<F, V> value_cast(const Function_p2v<F>& f)
  {
    // FIXME: Check that the cast "F::result -> V" is OK.
    casted_fp2v_<F, V> tmp(exact(f));
    return tmp;
  }


  // FIXME: Add casted_fv2v_<F,V>?


} // end of namespace oln


#endif // ! OLN_CORE_GEN_VALUE_CAST_HH
