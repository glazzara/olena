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

#ifndef MLN_FUN_PW_VALUE_HH
# define MLN_FUN_PW_VALUE_HH

/*! \file mln/fun/pw_value.hh
 *
 * \brief FIXME.
 */

# include <mln/core/concept/function.hh>
# include <mln/core/concept/image.hh>
# include <mln/value/props.hh>



namespace mln
{

  // Fwd decl.
  namespace fun { template <typename I> struct pw_value; }



  // FIXME: Doc!
  
  template <typename I>
  fun::pw_value<I> pw_value(const Image<I>& ima);
  


  namespace fun
  {

    // FIXME: Move!

    namespace internal
    {

      template <typename K, typename E>
      struct select_function_ : Function_p2v<E>
      {};

      template <typename E>
      struct select_function_< value::binary_kind, E > : Function_p2b<E>
      {};

    } // end of namespace mln::fun::internal


    // FIXME: Doc!

    template <typename I>
    struct pw_value : public internal::select_function_< mln_value_kind(I), pw_value<I> >
    {
      typedef mln_value(I) result;
      pw_value(const I& ima);
      mln_rvalue(I) operator()(const mln_psite(I)& p) const;
    protected:
      const I& ima_;
    };



# ifndef MLN_INCLUDE_ONLY

    // fun::pw_value<I>

    template <typename I>
    pw_value<I>::pw_value(const I& ima)
      : ima_(ima)
    {
    }

    template <typename I>
    mln_rvalue(I)
      pw_value<I>::operator()(const mln_psite(I)& p) const
    {
      mln_precondition(ima_.owns_(p));
      return ima_(p);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::fun

# ifndef MLN_INCLUDE_ONLY

  // pw_value

  template <typename I>
  fun::pw_value<I>
  pw_value(const Image<I>& ima)
  {
    mln_precondition(exact(ima).has_data());
    fun::pw_value<I> tmp(exact(ima));
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_FUN_PW_VALUE_HH
