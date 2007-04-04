// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may
// use this file as part of a free software library without
// restriction.  Specifically, if other files instantiate templates or
// use macros or inline functions from this file, or you compile this
// file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be
// covered by the GNU General Public License.  This exception does not
// however invalidate any other reasons why the executable file might
// be covered by the GNU General Public License.

#ifndef OLN_LEVEL_LOCAL_HH
# define OLN_LEVEL_LOCAL_HH

# include <oln/core/concept/image.hh>
# include <oln/core/concept/window.hh>
# include <oln/core/concept/accumulator.hh>
# include <oln/accumulator/or.hh>
# include <oln/accumulator/and.hh>

namespace oln
{

  namespace level
  {

    template <typename A, typename I>
    typename A::result
    local(const Accumulator<A>& f, const Image<I>& input, const oln_point(I)& p);

    template <typename A, typename I, typename W>
    typename A::result
    local(const Accumulator<A>& f,
	  const Image<I>&       input,
	  const oln_point(I)&   p,
	  const Window<W>&      win);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      /// Neigborhood.

      // Generic version with neighborhood.

      template <typename A, typename I>
      typename A::result
      local_(const A& f,
	     const Image_with_Nbh<I>& input,
	     const oln_point(I)& p)
      {
 	f.init_with(input(p));
	oln_niter(I) n(p, input);
	for_all(n)
	  f(input(n));
	return f.value();
      }

      // Optimised version for OR operator with neighborhood.

      template <typename B, typename I>
      B
      local_(const oln::accumulator::or_<B>& f,
	     const Image_with_Nbh<I>&        input,
	     const oln_point(I)&             p)
      {
	f.init_with(input(p));
	if (f.value() == true)
	  return true;
	oln_niter(I) n(p, input);
	for_all(n)
	  {
	    f(input(n)); // FIXME: Change to f.take(input(n))?
	    if (f.value() == true)
	      return true;
	  }
	return f.value();
      }

      // Optimised version for AND operator with neighborhood.

      template <typename B, typename I>
      B
      local_(const accumulator::and_< B > f,
	     const Image_with_Nbh<I>& input,
	     const oln_point(I)& p)
      {
	f.init_with(input(p));
	oln_niter(I) n(p, input);
	for_all(n)
	  {
	    f(input(n)); // FIXME: Change to f.take(input(n))?
	    if (f.value() == false)
	      return false;
	  }
	return f.value();
      }

      // FIXME: Generic version with nbh given as argument?



      /// On Window.

      // Generic version with window.

      template <typename A, typename I, typename W>
      typename A::result
      local_(const A& f,
	     const Image<I>& input,
	     const oln_point(I)& p,
	     const Window<W>& win)
      {
	f.init();
	oln_qiter(W) q(p, win);
	for_all(q)
	  if (input.owns_(q))
	    f(input(q));
	return f.value();
      }

      // Optimised version for OR operator with window.

      template <typename B, typename I, typename W>
      B
      local_(const oln::accumulator::or_<B>& f,
	     const Binary_Image<I>& input,
	     const oln_point(I)& p,
	     const Window<W>& win)
      {
	f.init();
	oln_qiter(W) q(p, win);
	for_all(q)
	  {
	    if (input.owns_(q))
	      f(input(q));
	    if (f.value() == true)
	      return true;
	  }
	return f.value();
      }

      // Optimised version for AND operator with window.

      template <typename B, typename I, typename W>
      B
      local_(const accumulator::and_< B > f,
	     const Image<I>& input,
	     const oln_point(I)& p,
	     const Window<W>& win)
      {
	f.init();
	oln_qiter(W) q(p, win);
	for_all(q)
	  {
	    if (input.owns_(q))
	      f(input(q));
	    if (f.value() == false)
	      return false;
	  }
	return f.value();
      }

    } // end of namespace oln::level::impl


    // Facades.

    template <typename A, typename I>
    typename A::result
    local(const Accumulator<A>& f, const Image<I>& input, const oln_point(I)& p)
    {
      mlc::assert_< mlc_is_a(I, Image_with_Nbh) >::check();
      return impl::local_(exact(f), exact(input), p);
    }

    template <typename A, typename I, typename W>
    typename A::result
    local(const Accumulator<A>& f,
	  const Image<I>&       input,
	  const oln_point(I)&   p,
	  const Window<W>&      win)
    {
      return impl::local_(exact(f), exact(input), p, exact(win));
    }

#endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::level

} // end of namespace oln

#endif // ! OLN_LEVEL_LOCAL_HH
