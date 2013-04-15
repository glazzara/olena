// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_MORPHO_RANK_FILTER_HH
# define MLN_MORPHO_RANK_FILTER_HH

/// \file
///
/// Morphological rank filter.
///
/// \todo Rely on the same mechanism as erosion/dilation.

# include <mln/morpho/includes.hh>
# include <mln/accu/transform_line.hh>
# include <mln/convert/to_p_array.hh>



namespace mln
{

  namespace morpho
  {

    /*! \brief Morphological rank_filter.
     *
     * \ingroup mlndatafilter
     */
    template <typename I, typename W>
    mln_concrete(I)
    rank_filter(const Image<I>& input, const Window<W>& win, unsigned k);


# ifndef MLN_INCLUDE_ONLY


    // Tests.


    namespace internal
    {

      template <typename I, typename W>
      inline
      void
      rank_filter_tests(const Image<I>& input_, const Window<W>& win_, unsigned k)
      {
	const I& input = exact(input_);
	const W& win = exact(win_);

	mln_precondition(input.is_valid());
	mln_precondition(! win.is_empty());
	(void) input;
	(void) win;
	(void) k;
      }

    } // end of namespace mln::morpho::internal



    // Implementations.


    namespace impl
    {

      namespace generic
      {

	template <typename I, typename W>
	inline
	mln_concrete(I)
	rank_filter(const Image<I>& input_, const Window<W>& win_, unsigned k)
	{
	  mln_trace("morpho::impl::generic::rank_filter");

	  internal::rank_filter_tests(input_, win_, k);

	  const I& input = exact(input_);
	  const W& win = exact(win_);

	  mln_concrete(I) output;
	  initialize(output, input);

	  accu::stat::rank<mln_value(I)> accu(k);
	  extension::adjust_fill(input, geom::delta(win) + 1, accu);
	  mln_piter(I) p(input.domain());
	  mln_qiter(W) q(win, p);
	  for_all(p)
	  {
	    accu.init();
	    for_all(q)
	      if (input.has(q))
		accu.take(input(q));
	    //else
	    //  accu.take(mln_value(I)(literal::zero));
	    output(p) = accu;
	  }

	  return output;
	}

      } // end of namespace mln::morpho::impl::generic


      template <typename I, typename W>
      inline
      mln_concrete(I)
      rank_filter_line(const Image<I>& input, const Window<W>& win, unsigned k, unsigned dir)
      {
	mln_trace("morpho::impl::rank_filter_line");

	internal::rank_filter_tests(input, win, k);

	accu::stat::rank<mln_value(I)> accu(k);
	extension::adjust_fill(input, geom::delta(win) + 1, accu);
	mln_concrete(I) output = accu::transform_line(accu, input, exact(win).length(), dir);

	return output;
      }


      template <typename I, typename W>
      inline
      mln_concrete(I)
      rank_filter_directional(const Image<I>& input, const Window<W>& win, unsigned k, unsigned dir)
      {
	mln_trace("morpho::impl::rank_filter_directional");

	internal::rank_filter_tests(input, win, k);

	accu::stat::rank<mln_value(I)> accu(k);
	extension::adjust_fill(input, geom::delta(win) + 1, accu);
	mln_concrete(I) output = accu::transform_directional(accu, input, win, dir);

	return output;
      }


    } // end of namespace mln::morpho::impl



    // Dispatch.


    namespace internal
    {

      template <typename I, typename M, unsigned i, typename C>
      inline
      mln_concrete(I)
      rank_filter_dispatch(const Image<I>& input, const win::line<M, i, C>& win, unsigned k)
      {
	return impl::rank_filter_line(input, win, k, i);
      }

      template <typename I>
      inline
      mln_concrete(I)
      rank_filter_dispatch(const Image<I>& input, const win::rectangle2d& win, unsigned k)
      {
	if (win.height() <= 3 && win.width() <= 3)
	  return impl::generic::rank_filter(input, win, k);
	else
	  if (win.height() < win.width())
	    return impl::rank_filter_directional(input, win, k, 1);
	  else
	    return impl::rank_filter_directional(input, win, k, 0);
      }

      template <typename I, typename W>
      inline
      mln_concrete(I)
      rank_filter_dispatch(const Image<I>& input, const Window<W>& win, unsigned k)
      {
	return impl::generic::rank_filter(input, win, k);
      }

    } // end of namespace mln::morpho::internal



    // Facades.


    template <typename I, typename W>
    inline
    mln_concrete(I)
    rank_filter(const Image<I>& input, const Window<W>& win, unsigned k)
    {
      mln_trace("morpho::rank_filter");

      mln_precondition(exact(input).is_valid());
      mln_precondition(! exact(win).is_empty());

      mln_concrete(I) output = internal::rank_filter_dispatch(exact(input), exact(win), k);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_RANK_FILTER_HH
