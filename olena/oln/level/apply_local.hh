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

#ifndef OLN_LEVEL_APPLY_LOCAL_HH
# define OLN_LEVEL_APPLY_LOCAL_HH

# include <oln/core/concept/image.hh>
# include <oln/core/concept/accumulator.hh>
# include <oln/core/internal/f_ch_value.hh>
# include <oln/level/local.hh>


namespace oln
{

  namespace level
  {

    /// Fwd decl.

    template <typename F, typename I>
    oln_plain_value(I, typename F::result)
    apply_local(const Accumulator<F>& f, const Image<I>& input);

    template <typename F, typename I, typename W>
    oln_plain_value(I, typename F::result)
    apply_local(const Accumulator<F>& f, const Image<I>& input, const Window<W>& win);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      // Generic version.

      template <typename F, typename I>
      oln_plain_value(I, typename F::result)
	apply_local_(const Accumulator<F>&    f,
		     const Image_with_Nbh<I>& input)
      {
	oln_plain_value(I, typename F::result) output;
	prepare(output, with, input);
        oln_piter(I) p(input.points());
        for_all(p)
	  output(p) = level::local(f, input, p);
        return output;
      }

      template <typename F, typename I, typename W>
      oln_plain_value(I, typename F::result)
      apply_local_(const Accumulator<F>& f,
		   const Image<I>&       input,
		   const Window<W>&      win)
      {
	oln_plain_value(I, typename F::result) output;
	prepare(output, with, input);
        oln_piter(I) p(input.points());
        for_all(p)
	  output(p) = level::local(f, input, p, win);
        return output;
      }

    } // end of namespace oln::level::impl


    // Facade.

    template <typename F, typename I>
    oln_plain_value(I, typename F::result)
    apply_local(const Accumulator<F>& f, const Image<I>& input)
    {
      mlc::assert_< mlc_is_a(I, Image_with_Nbh) >::check();
      return impl::apply_local_(exact(f), exact(input));
    }

    template <typename F, typename I, typename W>
    oln_plain_value(I, typename F::result)
    apply_local(const Accumulator<F>& f, const Image<I>& input, const Window<W>& win)
    {
      return impl::apply_local_(exact(f), exact(input), exact(win));
    }

# endif

  } // end of namespace oln::level

} // end of namespace oln


#endif // ! OLN_LEVEL_APPLY_LOCAL_HH
