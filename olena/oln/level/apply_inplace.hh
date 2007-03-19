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

#ifndef OLN_LEVEL_APPLY_INPLACE_HH
# define OLN_LEVEL_APPLY_INPLACE_HH

# include <oln/core/concept/image.hh>
# include <oln/core/gen/fun.hh>


namespace oln
{

  namespace level
  {

    /// Fwd decls.

    template <typename F, typename I>
    void
    apply_inplace(const Function_v2v<F>& f, Mutable_Image<I>& input)

    template <typename R, typename A, typename I>
    void
    apply_inplace(R (*f)(A), Mutable_Image<I>& input);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      // Generic version.

      template <typename F, typename I>
      void
      apply_inplace_(const F& f, Mutable_Image<I>& input)
      {
        oln_piter(I) p(input.points());
        for_all(p)
          input(p) = f(input(p));
	// FIXME: input(p) = static_cast<oln_value(I)>( f_( static_cast<oln_argument(F)>(input(p)) ) );
      }


    } // end of namespace oln::level::impl


    /// Facades.

    template <typename F, typename I>
    void
    apply_inplace(const Function_v2v<F>& f, Mutable_Image<I>& input)
    {
      impl::apply_inplace_(exact(f), exact(input));
    }

    template <typename R, typename A, typename I>
    void
    apply_inplace(R (*f)(A), Mutable_Image<I>& input)
    {
      impl::apply_inplace_(functorize_v2v(fun), exact(input));
    }

# endif

  } // end of namespace oln::level

} // end of namespace oln


#endif // ! OLN_LEVEL_APPLY_INPLACE_HH
