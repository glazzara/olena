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

#ifndef MLN_FUN_C_HH
# define MLN_FUN_C_HH

/*! \file mln/fun/c.hh
 *
 * \brief FIXME.
 */

# include <mln/fun/internal/selector.hh>
# include <mln/metal/unqualif.hh>


namespace mln
{

  namespace fun
  {
    
    template <typename F> struct C;


    // FIXME: Doc!
    template <typename R, typename A>
    struct C< R (*)(A) >
      :
      fun::internal::selector_< R, A, C<R(*)(A)> >::ret
    {
      C(R (*f)(A));
      typedef R result;
      R operator()(const mlc_unqualif(A)& a) const;
    protected:
      R (*f_)(A);
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename R, typename A>
    inline
    C<R(*)(A)>::C(R (*f)(A))
      : f_(f)
    {
    }

    template <typename R, typename A>
    inline
    R
    C<R(*)(A)>::operator()(const mlc_unqualif(A)& a) const
    {
      return f_(a);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_P2B_C_HH
